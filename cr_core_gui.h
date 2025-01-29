/**
 * Core gui
 *
 * This file holds the core components and functions for the gui
 *
 */
#ifndef CR_CORE_GUI_H_INCLUDED
#define CR_CORE_GUI_H_INCLUDED

#include <gtk-3.0/gtk/gtk.h>
#include <string>
#include <vector>
#include "cr_vars.h"

using namespace std;


string get_current_date() {
    time_t t = time(nullptr);
    tm *tm_local = localtime(&t);
    char date_str[11];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm_local);
    return string(date_str);
}

void set_label_stats(GtkWidget *wid, int len=100) {
    gtk_widget_set_size_request(wid, len, -1); // Set consistent width for combo box
    gtk_widget_set_halign(wid, GTK_ALIGN_START); // Left-align label
    gtk_label_set_xalign(GTK_LABEL(wid), 0.0);

}

void on_cancel_button_clicked(GtkWidget* button, gpointer user_data) {
    gtk_widget_destroy(GTK_WIDGET(user_data));
}


void set_label_class(GtkWidget *label, const char *class_name, GtkCssProvider *provider) {
    GtkStyleContext *context = gtk_widget_get_style_context(label);
    gtk_style_context_add_class(context, class_name);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}


void change_color(GtkWidget* widget, const char* color) {
    GdkRGBA rgba_color;
    gdk_rgba_parse(&rgba_color, color);
    gtk_widget_override_background_color(widget, GTK_STATE_FLAG_NORMAL, &rgba_color);
}


/**
 *  create_label_and_textbox_pane
 *
 *  g_widget - pass in the global widget that you want to store the text boxes to.  this needs to be a vector
 */
GtkWidget* create_label_and_textbox_pane(vector<GtkWidget*> g_widget, const string w_name[], const string w_text[], int label_width = 100, const char *color = "#FFFFFF") {

    if (g_widget.empty()) {
        g_warning("Error 626! No Widget");
        return nullptr;
    }
    int i=0;

    /**
     * inner pane is put into the outer pane. the outer pane is colored so it will have a background color.
     * inner pane is transparent, but has a border. if I put the color on the inner pane, then the border will be the color of the window and the inner pane would look like crap. the outer pane exists for prettieness.
     * */
    GtkWidget  *pane_inner = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget  *pane_outer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width(GTK_CONTAINER(pane_inner), 15);

    for (const string &nme : w_name) {

        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
        GtkWidget *label = gtk_label_new(w_text[i].c_str());
        GtkWidget *textbox = gtk_entry_new();



        // Set a unique name for the widget
        //gtk_widget_set_name(g_widget, nme.c_str());

        gtk_box_pack_start(GTK_BOX(row), label, FALSE, FALSE, 0);
        gtk_box_pack_start(GTK_BOX(row), textbox, TRUE, TRUE, 0);

        set_label_stats(label, label_width);

        gtk_box_pack_start(GTK_BOX(pane_inner), row, FALSE, FALSE, 0);

        // Add widget to the global widget



        g_widget.push_back(textbox);

        //gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), coin.c_str());
        i++;
    }

    gtk_box_pack_start(GTK_BOX(pane_outer), pane_inner, FALSE, FALSE, 0);

    // this allows me to set the back color of the outer pane
    if (!color.empty()) {
        change_color(pane_outer, color);

    }

    return pane_outer;
}

#endif