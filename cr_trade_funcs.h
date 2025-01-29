#ifndef CR_TRADE_FUNCS_H_INCLUDED
#define CR_TRADE_FUNCS_H_INCLUDED

#include <iostream>
#include <string>
#include <stdexcept>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <chrono>
#include <thread>
#include <ctime>
#include "cr_vars.h"

using namespace std;
using namespace sql;


// Global variables
string coinName = "DOGEUSD";
double buyPrice = 0.0;
double sellPrice = 0.0;
double realCash = 1000.0;
double realCoins = 0.0;
double coinsToReserve = 0.0;
string lastAction = "sell";
string buyMode = "buy";

// Extract the crypto symbol from a string
string rbGetCryptoSymbol(const string &selectedItem) {
    size_t start = selectedItem.find('(');
    size_t end = selectedItem.find(')');
    if (start != string::npos && end != string::npos) {
        return selectedItem.substr(start + 1, end - start - 1) + "USD";
    }
    return "";
}

// Connect to MariaDB
Connection* connectToMariaDB() {
    try {
        Driver *driver = get_driver_instance();
        Connection *conn = driver->connect(
            "tcp://" + dbConfig.host + ":3306",
            dbConfig.user,
            dbConfig.password
        );
        conn->setSchema(dbConfig.database);
        return conn;
    } catch (SQLException &e) {
        cerr << "Error connecting to MariaDB: " << e.what() << endl;
        return nullptr;
    }
}

// Get price from the database (stub function)
double getCoinPriceFromDB(int coinId, Connection* conn) {
    return 0.0;  // Stub: Replace with actual implementation
}

// Simulate fetching price from another source
double getRbCoinPrice(const string &which) {
    try {
        if (which == "buy") {
            return 0.075;  // Replace with actual logic
        } else if (which == "sell") {
            return 0.080;  // Replace with actual logic
        }
        return -2.0;
    } catch (const exception &e) {
        cerr << "Error fetching price: " << e.what() << endl;
        return -2.0;
    }
}

// Simulate price fetch with fallback
double getPriceAttempt2(const string &which) {
    try {
        double price = getRbCoinPrice(which);
        if (price > 0.0) {
            return price * 0.006;
        } else {
            return getRbCoinPrice(which);
        }
    } catch (const exception &e) {
        cerr << "Error fetching price: " << e.what() << endl;
        this_thread::sleep_for(chrono::seconds(1));
        return getRbCoinPrice(which);
    }
    return -2.0;
}

// Main loop for trading logic
void mainLoop() {
    Connection* conn = connectToMariaDB();
    if (!conn) return;

    try {
        unique_ptr<Statement> stmt(conn->createStatement());
        while (true) {
            string query = "SELECT coinid, rob_buy, rob_sell, date, time FROM crypto_prices WHERE coin = '" + coinName + "' ORDER BY date DESC, time DESC LIMIT 1;";
            ResultSet* res = stmt->executeQuery(query);

            if (res->next()) {
                int coinId = res->getInt("coinid");
                double robBuy = res->getDouble("rob_buy");
                double robSell = res->getDouble("rob_sell");
                string dateStr = res->getString("date");
                string timeStr = res->getString("time");

                // Parse date and time
                tm priceTime = {};
                strptime((dateStr + " " + timeStr).c_str(), "%Y-%m-%d %H:%M:%S", &priceTime);
                auto priceTimePoint = chrono::system_clock::from_time_t(mktime(&priceTime));
                auto currentTime = chrono::system_clock::now();

                if (currentTime - priceTimePoint > chrono::minutes(1)) {
                    robBuy = getPriceAttempt2("buy");
                    robSell = getPriceAttempt2("sell");
                }

                // Buying logic
                if (buyMode == "buy") {
                    cout << "Buy mode active." << endl;
                    if (robBuy > 0 && buyPrice >= robBuy) {
                        double usableCash = realCash - (realCash * 0.011);
                        if (usableCash > 2 && lastAction == "sell") {
                            cout << "Executing buy order." << endl;
                            lastAction = "buy";
                        }
                    }
                }
                // Selling logic
                else if (buyMode == "sell") {
                    cout << "Sell mode active." << endl;
                    if (robSell > 0 && sellPrice <= robSell && lastAction == "buy") {
                        cout << "Executing sell order." << endl;
                        lastAction = "sell";
                    }
                } else {
                    cerr << "Invalid buy mode: " << buyMode << endl;
                }
            } else {
                cout << "No recent data found. Fetching from external source." << endl;
                double robBuy = getPriceAttempt2("buy");
                double robSell = getPriceAttempt2("sell");
            }
            this_thread::sleep_for(chrono::seconds(10));
        }
    } catch (SQLException &e) {
        cerr << "Error executing query: " << e.what() << endl;
    } catch (const exception &e) {
        cerr << "General error: " << e.what() << endl;
    }

    delete conn;
}

// Entry point
int main() {
    mainLoop();
    return 0;
}
#endif