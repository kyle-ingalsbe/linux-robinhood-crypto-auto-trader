#ifndef CR_BTNS_H_INCLUDED
#define CR_BTNS_H_INCLUDED
/*
void start_button_clicked(GtkWidget *widget, gpointer data);
void stop_button_clicked(GtkWidget *widget, gpointer data);
void update_button_clicked(GtkWidget *widget, gpointer data);

#include <gtk-3.0/gtk/gtk.h>
#include <iostream>
#include <string>
#include <ctime>
#include "cr_vars.h"
#include "cr_btns.h"
*/

#include <gtk-3.0/gtk/gtk.h>
#include <iostream>

using namespace std;

void start_button_clicked(GtkWidget *widget, gpointer data) {
    // start the main loop
    cout << "start clicked" << endl;
}
void stop_button_clicked(GtkWidget *widget, gpointer data) {
    // stop the main loop
}

void update_button_clicked(GtkWidget *widget, gpointer data) {
    // update the vars
}


void on_database_configure_clicked(GtkWidget *widget, gpointer data) {
    cout << "Database Configure clicked" << endl;
}


void on_database_test_clicked(GtkWidget *widget, gpointer data) {
    cout << "Database Test clicked" << endl;
}

void on_about_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
                                               static_cast<GtkDialogFlags>(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "RobinHood Crypto Trader\nVersion 1.0");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_connect_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Login",
                                                    NULL,
                                                    static_cast<GtkDialogFlags>(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
                                                    "_OK", GTK_RESPONSE_OK,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *username_label = gtk_label_new("Username:");
    GtkWidget *username_entry = gtk_entry_new();
    GtkWidget *password_label = gtk_label_new("Password:");
    GtkWidget *password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    gtk_box_pack_start(GTK_BOX(content_area), username_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), username_entry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), password_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), password_entry, FALSE, FALSE, 5);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *username = gtk_entry_get_text(GTK_ENTRY(username_entry));
        const char *password = gtk_entry_get_text(GTK_ENTRY(password_entry));
        cout << "Login Attempted. Username: " << username << ", Password: " << password << endl;
    }
    gtk_widget_destroy(dialog);
}
#endif