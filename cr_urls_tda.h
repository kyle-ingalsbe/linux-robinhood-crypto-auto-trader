#ifndef CR_URLS_TDA_H_INCLUDED
#define CR_URLS_TDA_H_INCLUDED
// this needs to be tested.
/*
Original author: jmfernandes
URL: https://github.com/jmfernandes/robin_stocks/blob/master/robin_stocks/tda/urls.py
Licensed under MIT License
*/
#include <string>
#include <stdexcept>
#include <regex>
#include <map>
#include <iostream>

class AutoName {
public:
    static std::string generate_next_value(const std::string& name) {
        return name;
    }
};

class Version {
public:
    enum Type {
        v1,
        v2
    };

    static std::string to_string(Type version) {
        switch (version) {
            case v1: return "v1";
            case v2: return "v2";
            default: throw std::invalid_argument("Invalid version");
        }
    }
};

class URLS {
private:
    static constexpr const char* BASE_URL = "https://api.tdameritrade.com";

    URLS() = delete; // Prevent instantiation

    static std::string get_base_url(Version::Type version) {
        return std::string(BASE_URL) + "/" + Version::to_string(version) + "/";
    }

public:
    static std::string get_endpoint(const std::string& url) {
        std::regex base_url_regex(BASE_URL, std::regex_constants::icase);
        if (std::regex_search(url, base_url_regex)) {
            return std::regex_replace(url, base_url_regex, "");
        } else {
            throw std::invalid_argument("The URL has the wrong base.");
        }
    }

    // accounts.py
    static std::string account(const std::string& id) {
        return get_base_url(Version::v1) + "accounts/" + id;
    }

    static std::string accounts() {
        return get_base_url(Version::v1) + "accounts";
    }

    static std::string transaction(const std::string& id, const std::string& transaction) {
        return get_base_url(Version::v1) + "accounts/" + id + "/transactions/" + transaction;
    }

    static std::string transactions(const std::string& id) {
        return get_base_url(Version::v1) + "accounts/" + id + "/transactions";
    }

    // authentication.py
    static std::string oauth() {
        return get_base_url(Version::v1) + "oauth2/token";
    }

    // markets.py
    static std::string markets() {
        return get_base_url(Version::v1) + "marketdata/hours";
    }

    static std::string market(const std::string& market) {
        return get_base_url(Version::v1) + "marketdata/" + market + "/hours";
    }

    static std::string movers(const std::string& index) {
        return get_base_url(Version::v1) + "marketdata/" + index + "/movers";
    }

    // orders.py
    static std::string orders(const std::string& account_id) {
        return get_base_url(Version::v1) + "accounts/" + account_id + "/orders";
    }

    static std::string order(const std::string& account_id, const std::string& order_id) {
        return get_base_url(Version::v1) + "accounts/" + account_id + "/orders/" + order_id;
    }

    // stocks.py
    static std::string instruments() {
        return get_base_url(Version::v1) + "instruments";
    }

    static std::string instrument(const std::string& cusip) {
        return get_base_url(Version::v1) + "instruments/" + cusip;
    }

    static std::string quote(const std::string& ticker) {
        return get_base_url(Version::v1) + "marketdata/" + ticker + "/quotes";
    }

    static std::string quotes() {
        return get_base_url(Version::v1) + "marketdata/quotes";
    }

    static std::string price_history(const std::string& ticker) {
        return get_base_url(Version::v1) + "marketdata/" + ticker + "/pricehistory";
    }

    static std::string option_chains() {
        return get_base_url(Version::v1) + "marketdata/chains";
    }
};
#endif