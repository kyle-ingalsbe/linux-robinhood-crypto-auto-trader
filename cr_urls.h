#ifndef CR_URLS_H_INCLUDED
#define CR_URLS_H_INCLUDED
// this needs to be tested.
/*
Original author: jmfernandes
URL: https://github.com/jmfernandes/robin_stocks/blob/master/robin_stocks/robinhood/urls.py
Licensed under MIT License
*/

#include <string>



// Login
std::string login_url() {
    return "https://api.robinhood.com/oauth2/token/";
}

std::string challenge_url(const std::string& challenge_id) {
    return "https://api.robinhood.com/challenge/" + challenge_id + "/respond/";
}

// Profiles
std::string account_profile_url(const std::string& account_number = "") {
    if (!account_number.empty()) {
        return "https://api.robinhood.com/accounts/" + account_number;
    } else {
        return "https://api.robinhood.com/accounts/?default_to_all_accounts=true";
    }
}

std::string basic_profile_url() {
    return "https://api.robinhood.com/user/basic_info/";
}

std::string investment_profile_url() {
    return "https://api.robinhood.com/user/investment_profile/";
}

std::string portfolio_profile_url(const std::string& account_number = "") {
    if (!account_number.empty()) {
        return "https://api.robinhood.com/portfolios/" + account_number;
    } else {
        return "https://api.robinhood.com/portfolios/";
    }
}

std::string security_profile_url() {
    return "https://api.robinhood.com/user/additional_info/";
}

std::string user_profile_url() {
    return "https://api.robinhood.com/user/";
}

std::string portfolio_historicals_url(const std::string& account_number) {
    return "https://api.robinhood.com/portfolios/historicals/" + account_number + "/";
}
/*
// Stocks
std::string earnings_url() {
    return "https://api.robinhood.com/marketdata/earnings/";
}

std::string events_url() {
    return "https://api.robinhood.com/options/events/";
}

std::string fundamentals_url() {
    return "https://api.robinhood.com/fundamentals/";
}

std::string historicals_url() {
    return "https://api.robinhood.com/quotes/historicals/";
}

std::string instruments_url() {
    return "https://api.robinhood.com/instruments/";
}

std::string news_url(const std::string& symbol) {
    return "https://api.robinhood.com/midlands/news/" + symbol + "/?";
}

std::string quotes_url() {
    return "https://api.robinhood.com/quotes/";
}

std::string ratings_url(const std::string& symbol) {
    return "https://api.robinhood.com/midlands/ratings/" + symbol + "/";
}

std::string splits_url(const std::string& symbol) {
    return "https://api.robinhood.com/instruments/" + symbol + "/splits/";
}

// Account
std::string phoenix_url() {
    return "https://phoenix.robinhood.com/accounts/unified";
}

std::string positions_url(const std::string& account_number = "") {
    if (!account_number.empty()) {
        return "https://api.robinhood.com/positions/?account_number=" + account_number;
    } else {
        return "https://api.robinhood.com/positions/";
    }
}

std::string banktransfers_url(const std::string& direction = "") {
    if (direction == "received") {
        return "https://api.robinhood.com/ach/received/transfers/";
    } else {
        return "https://api.robinhood.com/ach/transfers/";
    }
}

std::string cardtransactions_url() {
    return "https://minerva.robinhood.com/history/transactions/";
}

std::string unifiedtransfers_url() {
    return "https://bonfire.robinhood.com/paymenthub/unified_transfers/";
}

std::string daytrades_url(const std::string& account) {
    return "https://api.robinhood.com/accounts/" + account + "/recent_day_trades/";
}

std::string dividends_url() {
    return "https://api.robinhood.com/dividends/";
}

std::string documents_url() {
    return "https://api.robinhood.com/documents/";
}

std::string withdrawal_url(const std::string& bank_id) {
    return "https://api.robinhood.com/ach/relationships/" + bank_id + "/";
}

std::string linked_url(const std::string& id = "", bool unlink = false) {
    if (unlink) {
        return "https://api.robinhood.com/ach/relationships/" + id + "/unlink/";
    }
    if (!id.empty()) {
        return "https://api.robinhood.com/ach/relationships/" + id + "/";
    } else {
        return "https://api.robinhood.com/ach/relationships/";
    }
}

std::string margin_url() {
    return "https://api.robinhood.com/margin/calls/";
}

std::string margininterest_url() {
    return "https://api.robinhood.com/cash_journal/margin_interest_charges/";
}

std::string notifications_url(bool tracker = false) {
    if (tracker) {
        return "https://api.robinhood.com/midlands/notifications/notification_tracker/";
    } else {
        return "https://api.robinhood.com/notifications/devices/";
    }
}


std::string referral_url() {
    return "https://api.robinhood.com/midlands/referral/";
}

std::string stockloan_url() {
    return "https://api.robinhood.com/accounts/stock_loan_payments/";
}

std::string interest_url() {
    return "https://api.robinhood.com/accounts/sweeps/";
}

std::string subscription_url() {
    return "https://api.robinhood.com/subscription/subscription_fees/";
}

std::string wiretransfers_url() {
    return "https://api.robinhood.com/wire/transfers";
}

std::string watchlists_url(const std::string& name = "", bool add = false) {
    if (!name.empty()) {
        return "https://api.robinhood.com/midlands/lists/items/";
    } else {
        return "https://api.robinhood.com/midlands/lists/default/";
    }
}

// Markets
std::string currency_url() {
    return "https://nummus.robinhood.com/currency_pairs/";
}

std::string markets_url() {
    return "https://api.robinhood.com/markets/";
}

std::string market_hours_url(const std::string& market, const std::string& date) {
    return "https://api.robinhood.com/markets/" + market + "/hours/" + date + "/";
}

std::string movers_sp500_url() {
    return "https://api.robinhood.com/midlands/movers/sp500/";
}

std::string get_100_most_popular_url() {
    return "https://api.robinhood.com/midlands/tags/tag/100-most-popular/";
}

std::string movers_top_url() {
    return "https://api.robinhood.com/midlands/tags/tag/top-movers/";
}

std::string market_category_url(const std::string& category) {
    return "https://api.robinhood.com/midlands/tags/tag/" + category + "/";
}

// Options
std::string aggregate_url(const std::string& account_number = "") {
    if (!account_number.empty()) {
        return "https://api.robinhood.com/options/aggregate_positions/?account_numbers=" + account_number;
    } else {
        return "https://api.robinhood.com/options/aggregate_positions/";
    }
}

std::string chains_url(const std::string& symbol) {
    return "https://api.robinhood.com/options/chains/" + symbol + "/";
}

std::string option_historicals_url(const std::string& id) {
    return "https://api.robinhood.com/marketdata/options/historicals/" + id + "/";
}

std::string option_instruments_url(const std::string& id = "") {
    if (!id.empty()) {
        return "https://api.robinhood.com/options/instruments/" + id + "/";
    } else {
        return "https://api.robinhood.com/options/instruments/";
    }
}

std::string option_orders_url(
    const std::string& orderID = "",
    const std::string& account_number = "",
    const std::string& start_date = ""
) {
    std::string url = "https://api.robinhood.com/options/orders/";
    if (!orderID.empty()) {
        url += orderID + "/";
    }
    std::string query;
    if (!account_number.empty()) {
        query += "account_numbers=" + account_number;
    }
    if (!start_date.empty()) {
        if (!query.empty()) {
            query += "&";
        }
        query += "updated_at[gte]=" + start_date;
    }
    if (!query.empty()) {
        url += "?" + query;
    }
    return url;
}

std::string option_positions_url(const std::string& account_number = "") {
    if (!account_number.empty()) {
        return "https://api.robinhood.com/options/positions/?account_numbers=" + account_number;
    } else {
        return "https://api.robinhood.com/options/positions/";
    }
}

std::string marketdata_options_url() {
    return "https://api.robinhood.com/marketdata/options/";
}


// Pricebook
std::string marketdata_quotes_url(const std::string& id) {
    return "https://api.robinhood.com/marketdata/quotes/" + id + "/";
}

std::string marketdata_pricebook_url(const std::string& id) {
    return "https://api.robinhood.com/marketdata/pricebook/snapshots/" + id + "/";
}
*/
// Crypto
std::string order_crypto_url() {
    return "https://nummus.robinhood.com/orders/";
}

std::string crypto_account_url() {
    return "https://nummus.robinhood.com/accounts/";
}

std::string crypto_currency_pairs_url() {
    return "https://nummus.robinhood.com/currency_pairs/";
}

std::string crypto_quote_url(const std::string& id) {
    return "https://api.robinhood.com/marketdata/forex/quotes/" + id + "/";
}

std::string crypto_holdings_url() {
    return "https://nummus.robinhood.com/holdings/";
}

std::string crypto_historical_url(const std::string& id) {
    return "https://api.robinhood.com/marketdata/forex/historicals/" + id + "/";
}

std::string crypto_orders_url(const std::string& orderID = "") {
    if (!orderID.empty()) {
        return "https://nummus.robinhood.com/orders/" + orderID + "/";
    } else {
        return "https://nummus.robinhood.com/orders/";
    }
}

std::string crypto_cancel_url(const std::string& id) {
    return "https://nummus.robinhood.com/orders/" + id + "/cancel/";
}

// Orders
std::string cancel_url(const std::string& url) {
    return "https://api.robinhood.com/orders/" + url + "/cancel/";
}

std::string option_cancel_url(const std::string& id) {
    return "https://api.robinhood.com/options/orders/" + id + "/cancel/";
}

std::string orders_url(
    const std::string& orderID = "",
    const std::string& account_number = "",
    const std::string& start_date = ""
) {
    std::string url = "https://api.robinhood.com/orders/";
    if (!orderID.empty()) {
        url += orderID + "/";
    }

    std::string query;
    if (!account_number.empty()) {
        query += "account_numbers=" + account_number;
    }
    if (!start_date.empty()) {
        if (!query.empty()) {
            query += "&";
        }
        query += "updated_at[gte]=" + start_date;
    }

    if (!query.empty()) {
        url += "?" + query;
    }

    return url;
}

#endif