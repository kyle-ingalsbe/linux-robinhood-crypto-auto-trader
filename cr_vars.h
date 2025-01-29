


  /*#define date_label_txt 0
    #define date_label 1
    #define coins_label_txt 2
    #define coins_label 3
    #define price_label_txt 4
    #define price_label 5
    #define high_label_txt 6
    #define high_label 7
    #define low_label_txt 8
    #define low_label 9
    #define status_label_txt 10
    #define status_label 11
    #define cash_label_txt 12
    #define cash_label 13
    #define dsign_label 14
    #define dsign_label_2 15
    #define dsign_label_3 16
    #define dsign_label_4 17
    #define dsign_label_5 18
    #define dsign_label_6 19
    #define width_label 20
*/
#ifndef CR_VARS_H_INCLUDED
#define CR_VARS_H_INCLUDED

#include <string>
#include <vector>

using namespace std;


struct DatabaseSettings {
    string username;
    string password;
    string dbname;
    string dburl;
} db_settings;


const string coin_names[] = {
    "Aave (AAVEUSD)", "Avalanche (AVAXUSD)", "Bitcoin (BTCUSD)", "Bitcoin Cash (BCHUSD)",
    "Chainlink (LINKUSD)", "Compound (COMPUSD)", "Dogecoin (DOGEUSD)", "Ethereum (ETHUSD)",
    "Litecoin (LTCUSD)", "Pepe (PEPEUSD)", "Shiba Inu (SHIBUSD)", "Solana (SOLUSD)",
    "Uniswap (UNIUSD)", "XRP (XRPUSD)", "Tezos (XTZUSD)"
};
//*/

int real_coins = 0;
float current_price = 0.0;
float pannic_sell = 0.0;// the number to pannic sell at.

const int window_width = 950;
const int window_height = 500;

string *hidden_password;
#endif