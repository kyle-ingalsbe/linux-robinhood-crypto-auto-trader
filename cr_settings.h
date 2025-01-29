#ifndef CR_SETTINGS_H_INCLUDED
#define CR_SETTINGS_H_INCLUDED

#include <gtk-3.0/gtk/gtk.h>
#include <string>
#include <iostream>
#include "cr_core_gui.h"
#include "cr_pwd.h"

using namespace std;

string global_sell, global_buy, global_panic;

void on_save_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = (GtkEntry **)data;

    /*
    GtkWidget *password_dialog = gtk_dialog_new_with_buttons(
        "Password", NULL, static_cast<GtkDialogFlags>(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        "_OK", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(password_dialog));
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(password_entry), "Enter password");
    gtk_container_add(GTK_CONTAINER(content_area), password_entry);
    gtk_widget_show_all(password_dialog);


    if (gtk_dialog_run(GTK_DIALOG(password_dialog)) == GTK_RESPONSE_OK) {
        const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
        if (string(password) == "your_password") { // Replace "your_password" with your desired password
            global_sell = gtk_entry_get_text(entries[0]);
            global_buy = gtk_entry_get_text(entries[1]);
            global_panic = gtk_entry_get_text(entries[2]);
            cout << "Data saved:" << endl;
            cout << "Sell: " << global_sell << endl;
            cout << "Buy: " << global_buy << endl;
            cout << "Panic: " << global_panic << endl;
            gtk_widget_destroy(password_dialog);
        } else {
            GtkWidget *error_dialog = gtk_message_dialog_new(
                NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK,
                "Incorrect password");
            gtk_dialog_run(GTK_DIALOG(error_dialog));
            gtk_widget_destroy(error_dialog);
        }
    }//*/
    //gtk_widget_destroy(password_dialog);
}



void open_settings_window(GtkWidget *widget, gpointer data) {

    if (ask_for_password(widget, data)){
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "New Window");
    gtk_container_set_border_width(GTK_CONTAINER(window), 15);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Row 1: Coin Name
    GtkWidget *label_coin = gtk_label_new("Coin Name:");
    GtkWidget *combo_coin = gtk_combo_box_text_new();
    for (const string &coin : coin_names) {
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_coin), coin.c_str());
    }
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_coin), 0);
    gtk_grid_attach(GTK_GRID(grid), label_coin, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), combo_coin, 1, 0, 1, 1);

    // Row 2: Sell (high)
    GtkWidget *label_sell = gtk_label_new("Sell (High):");
    GtkWidget *entry_sell = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_sell), "Enter sell price");
    gtk_grid_attach(GTK_GRID(grid), label_sell, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_sell, 1, 1, 1, 1);

    // Row 3: Buy (low)
    GtkWidget *label_buy = gtk_label_new("Buy (Low):");
    GtkWidget *entry_buy = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_buy), "Enter buy price");
    gtk_grid_attach(GTK_GRID(grid), label_buy, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_buy, 1, 2, 1, 1);

    // Row 4: Panic Sell
    GtkWidget *label_panic = gtk_label_new("Panic Sell:");
    GtkWidget *entry_panic = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_panic), "Enter panic sell price");
    gtk_widget_set_tooltip_markup(label_panic,
        "<span foreground='black' background='white' font='12'>"
        "Panic sell is when the price reaches a low point and you no longer want to lose money. "
        "It immediately sells the coin."
        "</span>");
    gtk_grid_attach(GTK_GRID(grid), label_panic, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_panic, 1, 3, 1, 1);

    // Row 5: Filler Label
    GtkWidget *filler_label = gtk_label_new(" ");
    gtk_grid_attach(GTK_GRID(grid), filler_label, 0, 4, 2, 1);

    // Row 6: Buttons
    GtkWidget *button_save = gtk_button_new_with_label("Save");
    GtkWidget *button_cancel = gtk_button_new_with_label("Cancel");
    GtkEntry *entries[] = {GTK_ENTRY(entry_sell), GTK_ENTRY(entry_buy), GTK_ENTRY(entry_panic)};
    g_signal_connect(button_save, "clicked", G_CALLBACK(on_save_clicked), entries);
    g_signal_connect(button_cancel, "clicked", G_CALLBACK(on_cancel_button_clicked), window);
    gtk_grid_attach(GTK_GRID(grid), button_save, 0, 5, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_cancel, 1, 5, 1, 1);

    gtk_widget_show_all(window);
    }
}

void create_settings_menu(GtkWidget *menubar) {

    GtkWidget *settings_menu = gtk_menu_new();
    GtkWidget *settings_item = gtk_menu_item_new_with_label("settings");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(settings_item), settings_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), settings_item);


    // Add "Settings" to the "settings" menu
    GtkWidget *settings_sub = gtk_menu_item_new_with_label("Settings");
    gtk_menu_shell_append(GTK_MENU_SHELL(settings_menu), settings_sub);
    g_signal_connect(settings_sub, "activate", G_CALLBACK(open_settings_window), NULL);



}

#endif