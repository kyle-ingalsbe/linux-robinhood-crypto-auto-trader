#ifndef CR_PWD_H_INCLUDED
#define CR_PWD_H_INCLUDED

//#include <gtk/gtk.h>
#include <gtk-3.0/gtk/gtk.h>
#include <string>
#include "cr_vars.h"
#include "cr_core_gui.h"

using namespace std;


// global string to store the password. - you are not meant to store this in a file. it is meant to be deleted every time the program (re)starts


bool password_set(GtkWidget *widget, gpointer entries) {
    cprint("koko");
    GtkWidget **entry_widgets = (GtkWidget **)entries;

    cprint("koko");
    const char *password = gtk_entry_get_text(GTK_ENTRY(entry_widgets[0]));
    cprint("koko");
    const char *confirm_password = gtk_entry_get_text(GTK_ENTRY(entry_widgets[1]));
cprint("koko");
    if (g_strcmp0(password, confirm_password) == 0) {
        hidden_password = new std::string(password);

        cout << hidden_password << endl;
        /*GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(widget)),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Password successfully set!"
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_widget_destroy(gtk_widget_get_toplevel(widget));
        return 1;
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(widget)),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Passwords do not match. Please try again."
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return 0;
        */
    }
}



bool check_hidden_password(GtkWidget *widget, gpointer entry) {
    const char *entered_password = gtk_entry_get_text(GTK_ENTRY(entry));
    const char *pwd = hidden_password->c_str();
    if (g_strcmp0(pwd, entered_password) == 0) {
        return 1;
    } else {
        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(gtk_widget_get_toplevel(widget)),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Password is incorrect."
        );
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return 0;
    }
    return 0;
}



bool ask_for_password(GtkWidget *widget, gpointer user_data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Enter Password",
        GTK_WINDOW(user_data),
        static_cast<GtkDialogFlags>(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        (const char *)"Check",
        GTK_RESPONSE_OK,
        (const char *)"Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter password");

    gtk_container_add(GTK_CONTAINER(content_area), entry);
    gtk_widget_show_all(dialog);

    // we want to get the response of whether the password worked or not.
    // 0 = bad password
    // 1 = good password.
    int resp=0;

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        resp = check_hidden_password(widget, entry);
    }

    gtk_widget_destroy(dialog);
    return resp;
}





void create_password_asker(GtkWidget *widget, gpointer entries)
{
    //gtk_init(&argc, &argv);
/*
    // Password setup dialog
    GtkWidget *dialog = gtk_dialog_new_with_buttons(
        "Set Password",
        NULL,
        static_cast<GtkDialogFlags>(GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT),
        "OK",
        GTK_RESPONSE_OK,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        NULL
    );//*/


    GtkWidget* dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Set Password");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 300, 160);



    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
   // GtkWidget *grid = gtk_grid_new();


    GtkWidget *password_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *password_confirm_row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *label_password = gtk_label_new("Enter new password:");
    GtkWidget *label_confirm = gtk_label_new("Confirm password:");
    GtkWidget *entry_password = gtk_entry_new();
    GtkWidget *entry_confirm = gtk_entry_new();
    GtkWidget *label_filler = gtk_label_new(" ");

    gtk_box_pack_start(GTK_BOX(password_row), label_password, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(password_row), entry_password, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(password_confirm_row), label_confirm, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(password_confirm_row), entry_confirm, TRUE, TRUE, 0);




     set_label_stats(label_password, 150);
     set_label_stats(label_confirm, 150);
     set_label_stats(label_filler, 300);


    GtkWidget* password_pane = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(password_pane), 15);

    gtk_box_pack_start(GTK_BOX(password_pane), password_row, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(password_pane), password_confirm_row, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(password_pane), label_filler, FALSE, FALSE, 0);

    change_color(dialog, "#FFFFFF");


    GtkWidget* ok_button_pwd = gtk_button_new_with_label("Set");
    GtkWidget* cancel_button_pwd = gtk_button_new_with_label("Cancel");

    GtkWidget* button_box_pwd = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_box_pack_start(GTK_BOX(button_box_pwd), ok_button_pwd, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box_pwd), cancel_button_pwd, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(password_pane), button_box_pwd, FALSE, FALSE, 0);


    GtkWidget *entry_widgets[] = {entry_password, entry_confirm};


    g_signal_connect(ok_button_pwd, "clicked", G_CALLBACK(password_set), entry_widgets);
    g_signal_connect(cancel_button_pwd, "clicked", G_CALLBACK(on_cancel_button_clicked), dialog);

     //set_label_stats(*entry_password, 100);
    // set_label_stats(*entry_confirm, 100);
/*
    gtk_entry_set_visibility(GTK_ENTRY(entry_password), FALSE);
    gtk_entry_set_visibility(GTK_ENTRY(entry_confirm), FALSE);

    gtk_grid_attach(GTK_GRID(grid), label_password, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_password, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_confirm, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_confirm, 1, 1, 1, 1);
*/
    gtk_container_add(GTK_CONTAINER(content_area), password_pane);

    gtk_widget_show_all(dialog);


/*    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        on_password_set(GTK_WIDGET(dialog), entry_widgets);
    } else {
        gtk_widget_destroy(dialog);
        //return 0;
    }
*/
}


void create_password_menu(GtkWidget *menubar) {

    GtkWidget *password_menu = gtk_menu_new();
    GtkWidget *password_item = gtk_menu_item_new_with_label("password");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(password_item), password_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), password_item);


    // Add "Settings" to the "password" menu
    GtkWidget *settings_item = gtk_menu_item_new_with_label("Settings");
    gtk_menu_shell_append(GTK_MENU_SHELL(password_menu), settings_item);
    g_signal_connect(settings_item, "activate", G_CALLBACK(create_password_asker), NULL);



}
#endif