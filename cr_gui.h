/**
 * GUI
 *
 * this file makes the main gui window
 *
 * note - this file may not contain components like the button functions,
 * or the menu
 *
 */

#ifndef CR_GUI_H_INCLUDED
#define CR_GUI_H_INCLUDED

#include <gtk-3.0/gtk/gtk.h>
#include <iostream>
#include <string>
#include <ctime>
#include "cr_vars.h"
#include "cr_btns.h"
#include "cr_core_gui.h"
#include "cr_db.h"
#include "cr_pwd.h"
#include "cr_settings.h"


using namespace std;





//extern const string coin_names[];
/*
const string coin_names[] = {
    "Aave (AAVEUSD)", "Avalanche (AVAXUSD)", "Bitcoin (BTCUSD)", "Bitcoin Cash (BCHUSD)",
    "Chainlink (LINKUSD)", "Compound (COMPUSD)", "Dogecoin (DOGEUSD)", "Ethereum (ETHUSD)",
    "Litecoin (LTCUSD)", "Pepe (PEPEUSD)", "Shiba Inu (SHIBUSD)", "Solana (SOLUSD)",
    "Uniswap (UNIUSD)", "XRP (XRPUSD)", "Tezos (XTZUSD)"
};
*/


void main_window(int argc, char *argv[]) {


    gtk_init(&argc, &argv);


    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    "label.red { color: red; font-size: 1.2em; }"
                                    "label.green { color: green;  font-size: 1.2em;}"
                                    "label.blue { color: blue; }",
                                    -1, // Length of CSS string (-1 means null-terminated)
                                    NULL);


    // Main Window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CR Trader");
    gtk_window_set_default_size(GTK_WINDOW(window), window_width, window_height);

    // Main Vertical Box
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Menu Bar
    GtkWidget *menubar = gtk_menu_bar_new();

    // Connect Menu
    GtkWidget *connect_menu = gtk_menu_item_new_with_label("Connect");
    GtkWidget *connect_submenu = gtk_menu_new();
    GtkWidget *connect_item = gtk_menu_item_new_with_label("Login");
    g_signal_connect(connect_item, "activate", G_CALLBACK(on_connect_clicked), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(connect_submenu), connect_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(connect_menu), connect_submenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), connect_menu);

    // Database Menu
    create_database_menu(menubar);


    create_password_menu(menubar);
    create_settings_menu(menubar);
/*
    GtkWidget *database_menu = gtk_menu_item_new_with_label("Database");
    GtkWidget *database_submenu = gtk_menu_new();
    GtkWidget *database_configure_item = gtk_menu_item_new_with_label("Configure");
    GtkWidget *database_test_item = gtk_menu_item_new_with_label("Test");
    g_signal_connect(database_configure_item, "activate", G_CALLBACK(on_database_configure_clicked), NULL);
    g_signal_connect(database_test_item, "activate", G_CALLBACK(on_database_test_clicked), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(database_submenu), database_configure_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(database_submenu), database_test_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(database_menu), database_submenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), database_menu);
*/
    // About Menu
    GtkWidget *about_menu = gtk_menu_item_new_with_label("About");
    g_signal_connect(about_menu, "activate", G_CALLBACK(on_about_clicked), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), about_menu);

    gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);

    // Panes
    GtkWidget *pane_main = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    GtkWidget *pane_top = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);

    GtkWidget *pane_a = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *pane_a_u = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *pane_b = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *pane_b_u = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *pane_c = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    // Pane A with uniform widget sizes and padding
    GtkWidget *combo_box_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *combo_label = gtk_label_new("Coin:");
    GtkWidget *coin_name_label = gtk_label_new(" N/A ");
    //GtkWidget *combo = gtk_combo_box_text_new();



    //gtk_widget_set_size_request(combo, 200, -1); // Set consistent width for combo box
    set_label_stats(combo_label, 75);
    set_label_stats(coin_name_label, 200);
    //gtk_widget_set_size_request(combo_label, 100, -1); // Set consistent width for combo box
    //gtk_widget_set_halign(combo_label, GTK_ALIGN_START); // Left-align label
    //gtk_label_set_xalign(GTK_LABEL(combo_label), 0.0);


    /*for (const string &coin : coin_names) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), coin.c_str());
    }

    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    */
    gtk_box_pack_start(GTK_BOX(combo_box_row), combo_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(combo_box_row), coin_name_label, TRUE, TRUE, 0);

    GtkWidget *sell_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *sell_label = gtk_label_new("Sell (High):");
    GtkWidget *sell_price_label = gtk_label_new("$");
    //GtkWidget *sell_tb = gtk_entry_new();
    //gtk_widget_set_size_request(sell_price_label, 300, -1); // Set consistent width for entry
    gtk_box_pack_start(GTK_BOX(sell_row), sell_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(sell_row), sell_price_label, TRUE, TRUE, 0);
    set_label_stats(sell_label, 75);
    set_label_stats(sell_price_label, 200);

    GtkWidget *buy_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *buy_label = gtk_label_new("Buy (Low):");
    GtkWidget *buy_price_label = gtk_label_new("$");
    //GtkWidget *buy_tb = gtk_entry_new();
    //gtk_widget_set_size_request(buy_price_label, 300, -1); // Set consistent
    //width for entry

    gtk_box_pack_start(GTK_BOX(buy_row), buy_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(buy_row), buy_price_label, TRUE, TRUE, 0);
    set_label_stats(buy_label, 75);
    set_label_stats(buy_price_label, 200);

    GtkWidget *info_box_label = gtk_label_new("Info:");
    GtkWidget *info_box = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(info_box, 400, 200); // Fixed size for info box
    set_label_stats(info_box_label, 100);


    gtk_container_set_border_width(GTK_CONTAINER(pane_a), 15); // 15px padding around
    gtk_container_set_border_width(GTK_CONTAINER(pane_b), 15); // 15px padding around
    gtk_container_set_border_width(GTK_CONTAINER(pane_c), 25); // 15px padding around



    // Pack rows into pane_a
    gtk_box_pack_start(GTK_BOX(pane_a), combo_box_row, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_a), sell_row, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_a), buy_row, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_a), info_box_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_a), info_box, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pane_a_u), pane_a, TRUE, TRUE, 0);





    // Pane B
    GtkWidget *date_label_txt = gtk_label_new("Date: ");
    GtkWidget *date_label = gtk_label_new(get_current_date().c_str());
    GtkWidget *coins_label_txt = gtk_label_new("# of Coins: ");
    GtkWidget *coins_label = gtk_label_new("0");
    GtkWidget *price_label_txt = gtk_label_new("Current Price:");
    GtkWidget *price_label = gtk_label_new("0.0");
    GtkWidget *high_label_txt = gtk_label_new("Day High:");
    GtkWidget *high_label = gtk_label_new("0.0");
    GtkWidget *low_label_txt = gtk_label_new("Day Low:");
    GtkWidget *low_label = gtk_label_new("0.0");
    GtkWidget *status_label_txt = gtk_label_new("Status: ");
    GtkWidget *status_label = gtk_label_new("Disconnected");
    GtkWidget *cash_label_txt = gtk_label_new("Cash:");
    GtkWidget *cash_label = gtk_label_new("0.0");
    GtkWidget *dsign_label = gtk_label_new("$");
    GtkWidget *dsign_label_2 = gtk_label_new("$");
    GtkWidget *dsign_label_3 = gtk_label_new("$");
    GtkWidget *dsign_label_4 = gtk_label_new("$");
    GtkWidget *dsign_label_5 = gtk_label_new("$");
    GtkWidget *dsign_label_6 = gtk_label_new("$");
    GtkWidget *width_label = gtk_label_new(" ");

    set_label_class(status_label, "red", provider);// make the text red
    set_label_class(cash_label, "blue", provider);// make the text red

    set_label_stats(width_label, 640);// this is specifically for the width of the label

    set_label_stats(date_label_txt, 100);
    set_label_stats(date_label, 300);

    set_label_stats(dsign_label, 8);
    set_label_stats(dsign_label_2, 8);
    set_label_stats(dsign_label_3, 8);
    set_label_stats(dsign_label_4, 8);
    set_label_stats(dsign_label_5, 8);
    set_label_stats(dsign_label_6, 8);

    set_label_stats(coins_label_txt, 100);
    set_label_stats(coins_label, 300);

    set_label_stats(price_label_txt, 100);
    set_label_stats(price_label, 300);

    set_label_stats(high_label_txt, 100);
    set_label_stats(high_label, 300);

    set_label_stats(low_label_txt, 100);
    set_label_stats(low_label, 300);

    set_label_stats(status_label_txt, 100);
    set_label_stats(status_label, 300);

    set_label_stats(cash_label_txt, 100);
    set_label_stats(cash_label, 300);

    GtkWidget *pane_b_row_1 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *pane_b_row_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *pane_b_row_3 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *pane_b_row_4 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *pane_b_row_5 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *pane_b_row_6 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *pane_b_row_7 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    gtk_box_pack_start(GTK_BOX(pane_b_row_1), date_label_txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_1), date_label, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pane_b_row_2), coins_label_txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_2), dsign_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_2), coins_label, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pane_b_row_3), price_label_txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_3), dsign_label_2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_3), price_label, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pane_b_row_4), high_label_txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_4), dsign_label_4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_4), high_label, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pane_b_row_5), low_label_txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_5), dsign_label_5, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_5), low_label, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pane_b_row_6), status_label_txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_6), status_label, TRUE, TRUE, 0);

    gtk_box_pack_start(GTK_BOX(pane_b_row_7), cash_label_txt, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_7), dsign_label_6, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b_row_7), cash_label, TRUE, TRUE, 0);


    gtk_box_pack_start(GTK_BOX(pane_b),pane_b_row_1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b),pane_b_row_2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b),pane_b_row_3, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b),pane_b_row_4, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b),pane_b_row_5, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b),pane_b_row_6, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b),pane_b_row_7, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(pane_b), width_label, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(pane_b_u), pane_b, TRUE, TRUE, 0);
    //GtkStyleContext *context = gtk_widget_get_style_context(pane_b);
    //set_widget_class(pane_b, "pane", provider);
    //gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    change_color(pane_a_u, "#FFFFFF");
    change_color(pane_b_u, "#FFFFFF");

    // Pane C
    GtkWidget *start_button = gtk_button_new_with_label("Start");
    GtkWidget *stop_button = gtk_button_new_with_label("Stop");
    GtkWidget *update_button = gtk_button_new_with_label("Update");

    g_signal_connect(start_button, "clicked", G_CALLBACK(start_button_clicked), (gpointer)"Start");
    g_signal_connect(stop_button, "clicked", G_CALLBACK(stop_button_clicked), (gpointer)"Stop");
    g_signal_connect(update_button, "clicked", G_CALLBACK(update_button_clicked), (gpointer)"Update");

    // making a grid to space out the buttons
    int btn_spacing = 50;
    GtkWidget *grid = gtk_grid_new();
    //gtk_container_set_border_width(GTK_CONTAINER(grid), 50);

    GtkWidget *width_label_1 = gtk_label_new(" ");
    GtkWidget *width_label_2 = gtk_label_new(" ");
    GtkWidget *width_label_3 = gtk_label_new(" ");
    GtkWidget *width_label_4 = gtk_label_new(" ");
    set_label_stats(width_label_1, btn_spacing);
    set_label_stats(width_label_2, btn_spacing);
    set_label_stats(width_label_3, btn_spacing);
    set_label_stats(width_label_4, btn_spacing);

    gtk_grid_attach(GTK_GRID(grid), width_label_1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), start_button, 1, 0, 1, 1); // Button 1 in column 2
    gtk_grid_attach(GTK_GRID(grid), width_label_2, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), stop_button, 3, 0, 1, 1); // Button 2 in column 4
    gtk_grid_attach(GTK_GRID(grid), width_label_3, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), update_button, 5, 0, 1, 1); // Button 3 in column 6
    gtk_grid_attach(GTK_GRID(grid), width_label_4, 6, 0, 1, 1);

    for (int i = 0; i < 7; ++i) {
        gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE); // Make all columns homogeneous
    }

    gtk_box_pack_start(GTK_BOX(pane_c), grid, TRUE, TRUE, 0);

    gtk_paned_pack1(GTK_PANED(pane_top), pane_a_u, TRUE, FALSE);
    gtk_paned_pack2(GTK_PANED(pane_top), pane_b_u, TRUE, FALSE);

    gtk_paned_pack1(GTK_PANED(pane_main), pane_top, TRUE, FALSE);
    gtk_paned_pack2(GTK_PANED(pane_main), pane_c, TRUE, FALSE);

    gtk_box_pack_start(GTK_BOX(vbox), pane_main, TRUE, TRUE, 0);

    // Show all widgets
    gtk_widget_show_all(window);

    // Signal for closing the window
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_main();

    //return 0;
}
#endif


/*
how to make the widget compression loop to make the gui with less code.

 std::vector<GtkWidget*> widgets;

    // Unique names for the widgets
    const char* widget_names[] = {"WidgetOne", "WidgetTwo", "WidgetThree", "WidgetFour", "WidgetFive"};

    const char* widget_text[] = {"coin", "buy", "sell", "$$", "..."};

    // Create 5 widgets with unique names
    for (int i = 0; i < 5; ++i) {

        // Create a new label widget
        GtkWidget* widget = gtk_label_new(widget_text[i]);

        // Set a unique name for the widget
        gtk_widget_set_name(widget, widget_names[i]);

        // Add widget to the vector
        widgets.push_back(widget);

       // gtk_widget_destroy(widget);
    }



*/