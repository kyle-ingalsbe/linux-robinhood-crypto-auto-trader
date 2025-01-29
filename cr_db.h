#ifndef CR_DB_H_INCLUDED
#define CR_DB_H_INCLUDED

#include <gtk-3.0/gtk/gtk.h>
//#include <gtk/gtk.h>
#include <mariadb/mysql.h>
#include <fstream>
#include <string>
#include <iostream>
#include <stdexcept>
#include <cstdio>
#include <cstring>
#include "cr_vars.h"

using namespace std;

const string db_settings_filename = "settings.rh";



void cprint(string str) {
    cout << str << endl;
}




// Function to save settings to a binary file
void save_settings() {
    try {
        FILE* fp = fopen("settings.rh", "wb");
        if (fp == NULL) {
            throw runtime_error("Error opening file for writing: " + string(strerror(errno)));
        }
        fwrite(&db_settings, sizeof(DatabaseSettings), 1, fp);
       // if (fwrite(&db_settings, sizeof(DatabaseSettings), 1, fp) != 1) {
           // fclose(fp);
       //     throw runtime_error("Error writing to file: Data could not be saved.");
      //  }

        fclose(fp);
    } catch (const runtime_error& e) {
        cerr << "Exception: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown error occurred while saving settings." << endl;
    }
}

// Function to load settings from a binary file
bool load_settings() {
    try {
        FILE* fp = fopen("settings.rh", "rb");
        if (fp == NULL) {
            throw runtime_error("Error opening file: " + string(strerror(errno)));
        }

        if (fread(&db_settings, sizeof(DatabaseSettings), 1, fp) != 1) {
            fclose(fp);
            throw runtime_error("Error reading file: Incomplete read or file format mismatch.");
        }

        fclose(fp);
        return true;
    } catch (const runtime_error& e) {
        cerr << "Exception: " << e.what() << endl;
        return false;
    } catch (...) {
        cerr << "Unknown error occurred while loading settings." << endl;
        return false;
    }
}

// Function to connect to the database
bool connect_to_database() {
    MYSQL* conn = mysql_init(nullptr);
    if (!conn) return false;

    if (!mysql_real_connect(conn, db_settings.dburl.c_str(), db_settings.username.c_str(), db_settings.password.c_str(), db_settings.dbname.c_str(), 0, nullptr, 0)) {
        mysql_close(conn);
        return false;
    }

    mysql_close(conn);
    return true;
}

void displayDatabaseSettings() {

    cout << "Username: " << db_settings.username << endl;
    cout << "Password: " << db_settings.password << endl;
    cout << "Database Name: " << db_settings.dbname << endl;
    cout << "Database URL: " << db_settings.dburl << endl;
}

void on_settings_button_clicked(GtkWidget* button, gpointer data) {
    DatabaseSettings settings;
    GtkWidget* dialog = gtk_dialog_new_with_buttons("Database Settings", GTK_WINDOW(data), GTK_DIALOG_MODAL, "Save", GTK_RESPONSE_OK, "Cancel", GTK_RESPONSE_CANCEL, nullptr);

    GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    GtkWidget* grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    GtkWidget* username_label = gtk_label_new("Username:");
    GtkWidget* password_label = gtk_label_new("Password:");
    GtkWidget* dbname_label = gtk_label_new("Database Name:");
    GtkWidget* dburl_label = gtk_label_new("Database URL:");

    GtkWidget* username_entry = gtk_entry_new();
    GtkWidget* password_entry = gtk_entry_new();
    GtkWidget* dbname_entry = gtk_entry_new();
    GtkWidget* dburl_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    // Set default text
    gtk_entry_set_text(GTK_ENTRY(username_entry), db_settings.username.c_str());
    gtk_entry_set_text(GTK_ENTRY(password_entry), db_settings.password.c_str());
    gtk_entry_set_text(GTK_ENTRY(dbname_entry), db_settings.dbname.c_str());
    gtk_entry_set_text(GTK_ENTRY(dburl_entry), db_settings.dburl.c_str());



    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dbname_label, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dbname_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dburl_label, 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), dburl_entry, 1, 3, 1, 1);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {

        db_settings.username = gtk_entry_get_text(GTK_ENTRY(username_entry));
        db_settings.password = gtk_entry_get_text(GTK_ENTRY(password_entry));
        db_settings.dbname = gtk_entry_get_text(GTK_ENTRY(dbname_entry));
        db_settings.dburl = gtk_entry_get_text(GTK_ENTRY(dburl_entry));

        save_settings();
    }

    gtk_widget_destroy(dialog);
}

void on_connect_button_clicked(GtkWidget* button, gpointer data) {
    //DatabaseSettings settings;


    if (connect_to_database()) {
        GtkWidget* message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "Database connection successful.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
    } else {
        GtkWidget* message = gtk_message_dialog_new(GTK_WINDOW(data), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Database connection failed.");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
    }
}

void create_database_menu(GtkWidget *menubar) {
    GtkWidget *database_menu = gtk_menu_new();
    GtkWidget *database_item = gtk_menu_item_new_with_label("Database");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(database_item), database_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), database_item);

    // Add "Settings" to the "Database" menu
    GtkWidget *settings_item = gtk_menu_item_new_with_label("Settings");
    gtk_menu_shell_append(GTK_MENU_SHELL(database_menu), settings_item);
    g_signal_connect(settings_item, "activate", G_CALLBACK(on_settings_button_clicked), NULL);

    // Add "Connect" to the "Database" menu
    GtkWidget *connect_item = gtk_menu_item_new_with_label("Connect");
    gtk_menu_shell_append(GTK_MENU_SHELL(database_menu), connect_item);
    g_signal_connect(connect_item, "activate", G_CALLBACK(on_connect_button_clicked), NULL);
}

/*
int main(int argc, char** argv) {
    gtk_init(&argc, &argv);

    if (!load_settings()) {
        cprint("Error: Failed to load settings.");
    }
    else{
        if (db_settings.username.empty() || db_settings.password.empty() || db_settings.dbname.empty() || db_settings.dburl.empty()) {
            cprint("Database settings are empty.");
        } else {
            connect_to_database();
        }
    }
    //displayDatabaseSettings();

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "MariaDB Connection Tool");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);

    GtkWidget* grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    GtkWidget* settings_button = gtk_button_new_with_label("Settings");
    GtkWidget* connect_button = gtk_button_new_with_label("Connect");

    gtk_grid_attach(GTK_GRID(grid), settings_button, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), connect_button, 1, 0, 1, 1);

    g_signal_connect(settings_button, "clicked", G_CALLBACK(on_settings_button_clicked), window);
    g_signal_connect(connect_button, "clicked", G_CALLBACK(on_connect_button_clicked), window);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
*/
#endif