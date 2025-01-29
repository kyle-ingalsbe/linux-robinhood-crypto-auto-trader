#ifndef CR_HELPER_H_INCLUDED
#define CR_HELPER_H_INCLUDED
// this needs to be tested.
/*
Original author: jmfernandes
URL: https://github.com/jmfernandes/robin_stocks/blob/master/robin_stocks/robinhood/helper.py
Licensed under MIT License
*/


#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>


// Global variables
bool LOGGED_IN = false;
std::ostream* OUTPUT = &std::cout;

// Function to set the login state
void set_login_state(bool logged_in) {
    LOGGED_IN = logged_in;
}

// Function to set the global output stream
void set_output(std::ostream& output) {
    OUTPUT = &output;
}

// Function to get the global output stream
std::ostream& get_output() {
    return *OUTPUT;
}

// Login-required decorator
template <typename Func>
auto login_required(Func func) {
    return [func](auto&&... args) {
        if (!LOGGED_IN) {
            throw std::runtime_error("This function can only be called when logged in");
        }
        return func(std::forward<decltype(args)>(args)...);
    };
}

// Convert None to String decorator
template <typename Func>
auto convert_none_to_string(Func func) {
    return [func](auto&&... args) {
        auto result = func(std::forward<decltype(args)>(args)...);
        return result.empty() ? "" : result;
    };
}

// Mocked request_get function for demonstration
std::map<std::string, std::string> request_get(
    const std::string& url,
    const std::string& mode = "",
    const std::map<std::string, std::string>& payload = {}
) {
    // Simulate a request. This should be replaced with an actual HTTP request handler.
    std::map<std::string, std::string> response;
    response["id"] = "mock_id";
    response["tradable_chain_id"] = "mock_chain_id";
    return response;
}

// Mocked filter_data function
std::string filter_data(const std::map<std::string, std::string>& data, const std::string& key) {
    auto it = data.find(key);
    return it != data.end() ? it->second : "";
}

// Functions to fetch IDs
std::string id_for_stock(const std::string& symbol) {
    std::string processed_symbol = symbol;
    for (auto& c : processed_symbol) c = std::toupper(c);
    processed_symbol.erase(processed_symbol.find_last_not_of(" \t\n\r") + 1);

    std::string url = "https://api.robinhood.com/instruments/";
    std::map<std::string, std::string> payload = { {"symbol", processed_symbol} };
    auto data = request_get(url, "indexzero", payload);

    return filter_data(data, "id");
}

std::string id_for_chain(const std::string& symbol) {
    std::string processed_symbol = symbol;
    for (auto& c : processed_symbol) c = std::toupper(c);
    processed_symbol.erase(processed_symbol.find_last_not_of(" \t\n\r") + 1);

    std::string url = "https://api.robinhood.com/instruments/";
    std::map<std::string, std::string> payload = { {"symbol", processed_symbol} };
    auto data = request_get(url, "indexzero", payload);

    auto it = data.find("tradable_chain_id");
    return it != data.end() ? it->second : "";
}

std::string id_for_group(const std::string& symbol) {
    std::string chain_id = id_for_chain(symbol);
    std::string url = "https://api.robinhood.com/options/chains/" + chain_id + "/";
    auto data = request_get(url);

    return data["underlying_instruments"][0];
}

std::string id_for_option(
    const std::string& symbol,
    const std::string& expirationDate,
    const std::string& strike,
    const std::string& optionType
) {
    std::string processed_symbol = symbol;
    for (auto& c : processed_symbol) c = std::toupper(c);

    std::string chain_id = id_for_chain(processed_symbol);
    std::map<std::string, std::string> payload = {
        {"chain_id", chain_id},
        {"expiration_dates", expirationDate},
        {"strike_price", strike},
        {"type", optionType},
        {"state", "active"}
    };

    std::string url = "https://api.robinhood.com/options/instruments/";
    auto data = request_get(url, "pagination", payload);

    std::vector<std::map<std::string, std::string>> listOfOptions;
    for (const auto& item : data) {
        if (item.second == expirationDate) {
            listOfOptions.push_back(item);
        }
    }

    if (listOfOptions.empty()) {
        get_output() << "Getting the option ID failed. Perhaps the expiration date or strike price is wrong." << std::endl;
        return "";
    }

    return listOfOptions[0]["id"];
}


// Rounds a price to an appropriate decimal place
double round_price(double price) {
    if (price <= 1e-2) {
        return std::round(price * 1e6) / 1e6;
    } else if (price < 1e0) {
        return std::round(price * 1e4) / 1e4;
    } else {
        return std::round(price * 1e2) / 1e2;
    }
}

// Filters data to extract values for a given key
template <typename T>
T filter_data(const T& data, const std::string& info) {
    if (data.empty()) {
        return T();
    }

    if constexpr (std::is_same<T, std::vector<std::map<std::string, std::string>>>::value) {
        std::vector<std::string> results;
        for (const auto& item : data) {
            auto it = item.find(info);
            if (it != item.end()) {
                results.push_back(it->second);
            }
        }
        return results;
    } else if constexpr (std::is_same<T, std::map<std::string, std::string>>::value) {
        auto it = data.find(info);
        if (it != data.end()) {
            return it->second;
        }
    }

    throw std::invalid_argument("Key not found in dictionary: " + info);
}

// Converts inputs to a set and list
std::vector<std::string> inputs_to_set(const std::vector<std::string>& inputSymbols) {
    std::set<std::string> symbolsSet;
    std::vector<std::string> symbolsList;

    for (const auto& symbol : inputSymbols) {
        std::string upperSymbol = symbol;
        std::transform(upperSymbol.begin(), upperSymbol.end(), upperSymbol.begin(), ::toupper);
        if (symbolsSet.insert(upperSymbol).second) {
            symbolsList.push_back(upperSymbol);
        }
    }

    return symbolsList;
}

// Makes a GET request and returns the document content
std::string request_document(const std::string& url, const std::map<std::string, std::string>& payload = {}) {
    // Simulate the GET request (requires a proper HTTP library like cURL or Boost.Beast for real requests)
    std::ostringstream simulatedResponse;
    simulatedResponse << "GET request sent to URL: " << url;

    if (!payload.empty()) {
        simulatedResponse << " with parameters: ";
        for (const auto& param : payload) {
            simulatedResponse << param.first << "=" << param.second << "&";
        }
    }

    return simulatedResponse.str();
}



////////////////========================================


#include <iostream>
#include <string>
#include <unordered_map>
#include <curl/curl.h>
#include <json/json.h>

CURL* session;  // Global session object

// Helper function to handle cURL response
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Function to perform GET request
Json::Value request_get(const std::string& url, const std::string& dataType = "regular",
                        const std::unordered_map<std::string, std::string>& payload = {}, bool jsonify_data = true) {
    std::string data;
    Json::Value jsonData;

    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Append payload to URL if necessary
        if (!payload.empty()) {
            std::string params = "?";
            for (const auto& p : payload) {
                params += p.first + "=" + p.second + "&";
            }
            params.pop_back();  // Remove the trailing "&"
            curl_easy_setopt(curl, CURLOPT_URL, (url + params).c_str());
        }

        if (jsonify_data) {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        }

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK && jsonify_data) {
            Json::CharReaderBuilder reader;
            std::istringstream s(data);
            std::string errs;
            if (!Json::parseFromStream(reader, s, &jsonData, &errs)) {
                std::cerr << "Error parsing JSON: " << errs << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }

    // Filter the data based on dataType
    if (dataType == "results") {
        return jsonData["results"];
    } else if (dataType == "pagination") {
        // Handling pagination (recursive loading of next pages) - Implement this as needed
    } else if (dataType == "indexzero") {
        if (jsonData.isMember("results") && jsonData["results"].size() > 0) {
            return jsonData["results"][0];
        }
    }

    return jsonData;
}

// Function to perform POST request
Json::Value request_post(const std::string& url, const std::unordered_map<std::string, std::string>& payload = {},
                         int timeout = 16, bool json = false, bool jsonify_data = true) {
    std::string data;
    Json::Value jsonData;

    CURL* curl = curl_easy_init();
    if (curl) {
        struct curl_slist* headers = nullptr;
        if (json) {
            headers = curl_slist_append(headers, "Content-Type: application/json");
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if (json) {
            // Convert payload to JSON string if needed
            Json::StreamWriterBuilder writer;
            std::string payloadJson = Json::writeString(writer, payload);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payloadJson.c_str());
        } else {
            // Convert payload to URL encoded format
            std::string encodedPayload;
            for (const auto& p : payload) {
                encodedPayload += p.first + "=" + p.second + "&";
            }
            encodedPayload.pop_back();
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, encodedPayload.c_str());
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK && jsonify_data) {
            Json::CharReaderBuilder reader;
            std::istringstream s(data);
            std::string errs;
            if (!Json::parseFromStream(reader, s, &jsonData, &errs)) {
                std::cerr << "Error parsing JSON: " << errs << std::endl;
            }
        }

        curl_easy_cleanup(curl);
    }

    return jsonData;
}

// Function to perform DELETE request
std::string request_delete(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string data;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error in DELETE request: " << curl_easy_strerror(res) << std::endl;
            data = "Error";
        }
        curl_easy_cleanup(curl);
    }
    return data;
}

// Function to update session headers (optional in C++)
void update_session(const std::string& key, const std::string& value) {
    // Implement if session headers need to be maintained manually
    std::cerr << "Session header updated: " << key << " = " << value << std::endl;
}

// Error handling functions
std::string error_argument_not_key_in_dictionary(const std::string& keyword) {
    return "Error: The keyword \"" + keyword + "\" is not a key in the dictionary.";
}

std::string error_ticker_does_not_exist(const std::string& ticker) {
    return "Warning: \"" + ticker + "\" is not a valid stock ticker. It is being ignored";
}

std::string error_must_be_nonzero(const std::string& keyword) {
    return "Error: The input parameter \"" + keyword + "\" must be an integer larger than zero and non-negative";
}



#endif