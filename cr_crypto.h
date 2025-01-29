#ifndef CR_HELPER_H_INCLUDED
#define CR_HELPER_H_INCLUDED
// this needs to be tested.
/*
Original author: jmfernandes
URL: https://github.com/jmfernandes/robin_stocks/blob/master/robin_stocks/robinhood/crypto.py
Licensed under MIT License
*/



#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <optional>
#include <sstream>
#include <algorithm>


using namespace std;

// Simulate external functions
std::string crypto_account_url() {
    return "https://nummus.robinhood.com/accounts/";
}

std::string crypto_holdings_url() {
    return "https://nummus.robinhood.com/holdings/";
}

std::string crypto_currency_pairs_url() {
    return "https://nummus.robinhood.com/currency_pairs/";
}

// Simulate request_get
std::vector<std::map<std::string, std::string>> request_get(const std::string& url, const std::string& type) {
    // Placeholder function for making HTTP GET requests
    std::vector<std::map<std::string, std::string>> data;
    if (type == "indexzero" || type == "results" || type == "pagination") {
        data = {
            {{"id", "1"}, {"symbol", "BTC"}, {"name", "Bitcoin"}, {"tradability", "tradable"}},
            {{"id", "2"}, {"symbol", "ETH"}, {"name", "Ethereum"}, {"tradability", "tradable"}}
        };
    }
    return data;
}

// Filter data
template <typename T>
T filter_data(const T& data, const std::optional<std::string>& info) {
    if (!data.size()) {
        return data;
    }

    if constexpr (std::is_same<T, std::vector<std::map<std::string, std::string>>>::value) {
        if (!info.has_value()) {
            return data;
        }

        std::vector<std::string> filtered;
        for (const auto& item : data) {
            auto it = item.find(info.value());
            if (it != item.end()) {
                filtered.push_back(it->second);
            }
        }
        return filtered;
    } else if constexpr (std::is_same<T, std::map<std::string, std::string>>::value) {
        if (!info.has_value()) {
            return data;
        }

        auto it = data.find(info.value());
        if (it != data.end()) {
            return it->second;
        }
    }

    return T();
}

// Load crypto profile
std::map<std::string, std::string> load_crypto_profile(const std::optional<std::string>& info = std::nullopt) {
    auto url = crypto_account_url();
    auto data = request_get(url, "indexzero");
    if (data.empty()) {
        return {};
    }
    return filter_data(data[0], info);
}

// Get crypto positions
std::vector<std::map<std::string, std::string>> get_crypto_positions(const std::optional<std::string>& info = std::nullopt) {
    auto url = crypto_holdings_url();
    auto data = request_get(url, "pagination");
    return filter_data(data, info);
}

// Get crypto currency pairs
std::vector<std::map<std::string, std::string>> get_crypto_currency_pairs(const std::optional<std::string>& info = std::nullopt) {
    auto url = crypto_currency_pairs_url();
    auto data = request_get(url, "results");
    return filter_data(data, info);
}

// Get crypto info
std::map<std::string, std::string> get_crypto_info(const std::string& symbol, const std::optional<std::string>& info = std::nullopt) {
    auto url = crypto_currency_pairs_url();
    auto data = request_get(url, "results");
    auto it = std::find_if(data.begin(), data.end(), [&symbol](const auto& item) {
        return item.at("symbol") == symbol;
    });

    if (it != data.end()) {
        return filter_data(*it, info);
    } else {
        return {};
    }
}



// Mock implementations for API calls and helper functions.
std::string get_crypto_info(const std::string& symbol, const std::string& key) {
    // Replace with actual implementation
    return "mock_crypto_id";
}

std::string crypto_quote_url(const std::string& id) {
    // Replace with actual implementation
    return "mock_quote_url";
}

std::string crypto_historical_url(const std::string& id) {
    // Replace with actual implementation
    return "mock_historical_url";
}

std::unordered_map<std::string, std::string> request_get(const std::string& url, const std::string& type = "regular", const std::unordered_map<std::string, std::string>& payload = {}) {
    // Replace with actual implementation
    return {};
}

std::vector<std::unordered_map<std::string, std::string>> filter_data(const std::unordered_map<std::string, std::string>& data, const std::string& info) {
    // Replace with actual implementation
    return {};
}

// Cache for crypto symbol-to-ID mapping
std::unordered_map<std::string, std::string> SYMBOL_TO_ID_CACHE;

std::string get_crypto_id(const std::string& symbol) {
    if (SYMBOL_TO_ID_CACHE.find(symbol) != SYMBOL_TO_ID_CACHE.end()) {
        return SYMBOL_TO_ID_CACHE[symbol];
    }

    std::string id = get_crypto_info(symbol, "id");
    if (!id.empty()) {
        SYMBOL_TO_ID_CACHE[symbol] = id;
    }
    return id;
}

std::unordered_map<std::string, std::string> get_crypto_quote(const std::string& symbol, const std::string& info = "") {
    std::string id = get_crypto_info(symbol, "id");
    std::string url = crypto_quote_url(id);
    std::unordered_map<std::string, std::string> data = request_get(url);
    return filter_data(data, info);
}

std::unordered_map<std::string, std::string> get_crypto_quote_from_id(const std::string& id, const std::string& info = "") {
    std::string url = crypto_quote_url(id);
    std::unordered_map<std::string, std::string> data = request_get(url);
    return filter_data(data, info);
}

std::vector<std::unordered_map<std::string, std::string>> get_crypto_historicals(const std::string& symbol, const std::string& interval = "hour", const std::string& span = "week", const std::string& bounds = "24_7", const std::string& info = "") {
    std::vector<std::string> interval_check = {"15second", "5minute", "10minute", "hour", "day", "week"};
    std::vector<std::string> span_check = {"hour", "day", "week", "month", "3month", "year", "5year"};
    std::vector<std::string> bounds_check = {"24_7", "extended", "regular", "trading"};

    if (std::find(interval_check.begin(), interval_check.end(), interval) == interval_check.end()) {
        std::cerr << "ERROR: Invalid interval value.\n";
        return {};
    }

    if (std::find(span_check.begin(), span_check.end(), span) == span_check.end()) {
        std::cerr << "ERROR: Invalid span value.\n";
        return {};
    }

    if (std::find(bounds_check.begin(), bounds_check.end(), bounds) == bounds_check.end()) {
        std::cerr << "ERROR: Invalid bounds value.\n";
        return {};
    }

    if ((bounds == "extended" || bounds == "trading") && span != "day") {
        std::cerr << "ERROR: 'extended' and 'trading' bounds can only be used with a span of 'day'.\n";
        return {};
    }

    std::string id = get_crypto_info(symbol, "id");
    std::string url = crypto_historical_url(id);

    std::unordered_map<std::string, std::string> payload = {
        {"interval", interval},
        {"span", span},
        {"bounds", bounds}
    };

    std::unordered_map<std::string, std::string> data = request_get(url, "regular", payload);
    std::vector<std::unordered_map<std::string, std::string>> histData;

    std::string cryptoSymbol = data["symbol"];
    for (const auto& subitem : data["data_points"]) {
        std::unordered_map<std::string, std::string> entry = subitem;
        entry["symbol"] = cryptoSymbol;
        histData.push_back(entry);
    }

    return filter_data(histData, info);
}



#endif