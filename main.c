#include "game.h"

int main(int argc, char* argv[]){
    gtk_init(&argc, &argv);
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);


    gtk_widget_set_size_request(win, 500, 800);
    gtk_window_set_resizable (GTK_WINDOW(win), FALSE);
    gtk_container_set_border_width(win, 5);
    gtk_window_set_title(win, "Hangman");

    gtk_widget_set_name(win, "win");

    //First Menu to the screen
    GtkLabel *label = gtk_label_new("Welcome To Hangman Game");
    gtk_widget_set_name(label, "welcome_label");
    GtkButton *click_btn = gtk_button_new_with_label("Play Now");
    GtkButton *quit_btn = gtk_button_new_with_label("Quit");
    gtk_widget_set_name(click_btn, "play_btn");
    gtk_widget_set_name(quit_btn, "quit_btn");

    //adding icon
    gtk_window_set_icon_from_file(win, "hangman_icon.png", NULL);



    //styling
    GdkDisplay *display = gdk_display_get_default();
    GdkScreen *screen = gdk_display_get_default_screen (display);
    GtkCssProvider *provider = gtk_css_provider_new ();
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    gtk_css_provider_load_from_path(provider, "style.css", NULL);

    g_signal_connect(click_btn, "clicked", G_CALLBACK(select_difficulty_menu), win);

    // quit the app
    g_signal_connect(quit_btn, "clicked", G_CALLBACK(end_program), NULL);
    g_signal_connect(win, "delete_event", G_CALLBACK(end_program), NULL);

    GtkWidget *vBox = gtk_vbox_new(TRUE, 5);

    gtk_box_pack_start(vBox, label, TRUE, TRUE, 0);
    gtk_box_pack_start(vBox, click_btn, TRUE, TRUE, 0);
    gtk_box_pack_start(vBox, quit_btn, TRUE, TRUE, 0);

    gtk_container_add(GTK_CONTAINER(win), vBox);
    gtk_widget_show_all(win);
    gtk_main();

    return 0;
}
