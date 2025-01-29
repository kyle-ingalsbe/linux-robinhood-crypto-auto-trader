#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <mariadb/mysql.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <cjson/cJSON.h>
#include <math.h>

#define MAX_COINS 8

// API and database configuration
const char *BASE_URL = "https://api.gemini.com/v1/pubticker/";
const char *COINS_S1[] = {"AAVEUSD", "AVAXUSD", "BTCUSD", "BCHUSD", "LINKUSD", "COMPUSD", "DOGEUSD"};
const char *COINS_S2[] = {"ETHUSD", "LTCUSD", "PEPEUSD", "SHIBUSD", "SOLUSD", "UNIUSD", "XRPUSD", "XTZUSD"};

const char *DB_HOST = "localhost";
const char *DB_USER = "artwork";
const char *DB_PASS = "a";
const char *DB_NAME = "cdata";

// Structure to hold API response
struct Memory {
    char *response;
    size_t size;
};


// Define ANSI color codes for text
#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT "\033[0;31m"
#define BLUE_TEXT "\033[0;34m"
#define CYAN_TEXT "\033[0;36m"
#define BRIGHT_MAGENTA "\033[95m"
#define RESET_TEXT "\033[0m"

float ask;
float bid;
float last;
// robinhood profit is the amount of $ they increase %age wise on top of the asking price.
float robinhood_profit = .006;

//CryptoData url_crypto_data[1];

void print_progress(int count, const char *color) {
    int percentage = round((100.0f / 15.0f) * (float)count);
    printf("\r%sWaiting: %d%%" RESET_TEXT, color, percentage );
    fflush(stdout); // Ensure the output is displayed immediately
}

// Write callback for curl to store response data
size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response, mem->size + total_size + 1);
    if (!ptr) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, total_size);
    mem->size += total_size;
    mem->response[mem->size] = '\0';

    return total_size;
}

// Function to fetch API data
char *fetch_data(const char *url) {
    CURL *curl;
    CURLcode res;
    struct Memory chunk = {NULL, 0};

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return NULL;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        free(chunk.response);
        chunk.response = NULL;
    }

    curl_easy_cleanup(curl);
    return chunk.response;
}

// Function to parse and convert JSON values
float parse_float(const char *json, const char *key) {
    char *pos = strstr(json, key);
    if (!pos) return -1.0;

    pos = strchr(pos, ':');
    if (!pos) return -1.0;

    return strtof(pos + 1, NULL);
}

void parse_and_store_json(const char *json) {
    cJSON *root = cJSON_Parse(json);
    if (!root) {
        fprintf(stderr, "Failed to parse JSON: %s\n", cJSON_GetErrorPtr());
        return;
    }

    // Extract values from JSON
    cJSON *ask_p = cJSON_GetObjectItem(root, "ask");
    cJSON *bid_p = cJSON_GetObjectItem(root, "bid");
    cJSON *last_p = cJSON_GetObjectItem(root, "last");

    if (cJSON_IsString(ask_p) && cJSON_IsString(bid_p) && cJSON_IsString(last_p)) {
        ask = strtof(ask_p->valuestring, NULL);
        bid = strtof(bid_p->valuestring, NULL);
        last = strtof(last_p->valuestring, NULL);
    } else {
        fprintf(stderr, "Invalid JSON format\n");
    }

    cJSON_Delete(root);
}

// Function to log prices to the terminal and database
void log_prices(const char **coins, int coin_count) {
    MYSQL *conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0)) {
        fprintf(stderr, "Database connection failed: %s\n", mysql_error(conn));
        return;
    }

    for (int i = 0; i < coin_count; i++) {
        char url[256];
        snprintf(url, sizeof(url), "%s%s", BASE_URL, coins[i]);

        char *data = fetch_data(url);
        if (!data) {
            fprintf(stderr, "Failed to fetch data for %s\n", coins[i]);
            continue;
        }
        else{
            parse_and_store_json(data);
            free(data);
        }

        float buy_price = ask;
        // robinhood always takes a % for the price of the crypto.
        float rob_buy_price = (ask * robinhood_profit ) + ask;
        float sell_price = bid;
        float rob_sell_price = bid - (bid * robinhood_profit);
        float last_bid_price = last;

        if (buy_price < 0 || sell_price < 0 ) {
            fprintf(stderr, "Invalid data for %s\n", coins[i]);
            free(data);
            continue;
        }

        char query[512];
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char date[16], time_str[16];
        strftime(date, sizeof(date), "%Y-%m-%d", t);
        strftime(time_str, sizeof(time_str), "%H:%M:%S", t);

        snprintf(query, sizeof(query),
                 "INSERT INTO crypto_prices (coin_id, buy, sell, last_bid, rob_buy, rob_sell,  date, time) "
                 "VALUES ('%s', %.8f, %.8f, %.8f, %.8f, %.8f, '%s', '%s');",
                 coins[i], buy_price, sell_price, last_bid_price,rob_buy_price, rob_sell_price, date, time_str);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        } else {
            printf("%s %s - " RED_TEXT "Buy: %.8f, " GREEN_TEXT "Sell: %.8f, " BLUE_TEXT "Robin Buy: %.8f, " BRIGHT_MAGENTA "Robin Sell %.8f\n" RESET_TEXT, time_str, coins[i], buy_price, sell_price,rob_buy_price, rob_sell_price);
        }

        //free(data);
        usleep(100000);  // Sleep for 10ms
    }

    mysql_close(conn);
}
/*
            CREATE TABLE IF NOT EXISTS crypto_prices (
                id INT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
                buy DECIMAL(18, 8),
                sell DECIMAL(18, 8),
                last_bid DECIMAL(18, 8),
                rob_buy DECIMAL(18, 8),
                rob_sell DECIMAL(18, 8),
                date DATE,
                time TIME,
                coin_id VARCHAR(10)
            )

*/


// Worker thread function
void *fetcher_thread(void *arg) {
    int sleep_runner = 16;
    int prgs=0;
    while (1) {
        switch (sleep_runner) {
            case 16:
                printf("\n");
                log_prices(COINS_S1, 7);
                sleep_runner++;
                prgs=0;
                break;

            case 32:
                printf("\n");
                log_prices(COINS_S2, 8);
                sleep_runner = 1;
                prgs=0;
                break;

            default:
                sleep(1);
                sleep_runner++;
                prgs++;
                print_progress(prgs, RED_TEXT);
                break;
        }
    }

    return NULL;
}

int main() {
    pthread_t thread;

    if (pthread_create(&thread, NULL, fetcher_thread, NULL) != 0) {
        fprintf(stderr, "Failed to create thread\n");
        return EXIT_FAILURE;
    }

    pthread_join(thread, NULL);
    return EXIT_SUCCESS;
}
