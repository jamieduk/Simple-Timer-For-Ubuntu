#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

GtkWidget *entry_hours;
GtkWidget *entry_minutes;
GtkWidget *entry_seconds;
GtkWidget *label_timer;
guint timer_id;
static int seconds_remaining=0;

gint on_timer_timeout(gpointer user_data) {  // Change return type to gint
    if (seconds_remaining > 0) {
        seconds_remaining--;
        gchar *time_text=g_strdup_printf("Time remaining: %02d:%02d:%02d",
            seconds_remaining / 3600,
            (seconds_remaining % 3600) / 60,
            seconds_remaining % 60);
        gtk_label_set_text(GTK_LABEL(label_timer), time_text);
        g_free(time_text);
        return G_SOURCE_CONTINUE;  // Continue calling this function
    } else {
        gtk_label_set_text(GTK_LABEL(label_timer), "Time's up!");
        system("mpg123 sounds/alarm.mp3 &"); // Play the alarm sound
        g_source_remove(timer_id);
        return G_SOURCE_REMOVE;  // Stop calling this function
    }
}

void start_timer(GtkWidget *widget, gpointer data) {
    const gchar *hours_text=gtk_entry_get_text(GTK_ENTRY(entry_hours));
    const gchar *minutes_text=gtk_entry_get_text(GTK_ENTRY(entry_minutes));
    const gchar *seconds_text=gtk_entry_get_text(GTK_ENTRY(entry_seconds));

    int hours=atoi(hours_text);
    int minutes=atoi(minutes_text);
    int seconds=atoi(seconds_text);
    seconds_remaining=(hours * 3600) + (minutes * 60) + seconds; // Store remaining time

    if (seconds_remaining > 0) {
        gtk_label_set_text(GTK_LABEL(label_timer), "Timer started...");
        timer_id=g_timeout_add_seconds(1, on_timer_timeout, NULL);
    } else {
        gtk_label_set_text(GTK_LABEL(label_timer), "Enter a valid time.");
    }
}

void reset_timer(GtkWidget *widget, gpointer data) {
    gtk_label_set_text(GTK_LABEL(label_timer), "Enter hours, minutes, and seconds and start the timer");
    gtk_entry_set_text(GTK_ENTRY(entry_hours), "");
    gtk_entry_set_text(GTK_ENTRY(entry_minutes), "");
    gtk_entry_set_text(GTK_ENTRY(entry_seconds), "");
    seconds_remaining=0; // Reset remaining time
    g_source_remove(timer_id);
}

void show_about_dialog(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog=gtk_message_dialog_new(GTK_WINDOW(data),
                                               GTK_DIALOG_MODAL,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_OK,
                                               "Author: Jay Mee\n@ J~Net 2024");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Timer");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid=gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry_hours=gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_hours, 0, 0, 1, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_hours), "Hours");

    entry_minutes=gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_minutes, 1, 0, 1, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_minutes), "Minutes");

    entry_seconds=gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), entry_seconds, 2, 0, 1, 1);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry_seconds), "Seconds");

    GtkWidget *start_button=gtk_button_new_with_label("Start Timer");
    g_signal_connect(start_button, "clicked", G_CALLBACK(start_timer), NULL);
    gtk_grid_attach(GTK_GRID(grid), start_button, 0, 1, 3, 1);

    GtkWidget *reset_button=gtk_button_new_with_label("Reset Timer");
    g_signal_connect(reset_button, "clicked", G_CALLBACK(reset_timer), NULL);
    gtk_grid_attach(GTK_GRID(grid), reset_button, 0, 2, 3, 1);

    GtkWidget *about_button=gtk_button_new_with_label("About");
    g_signal_connect(about_button, "clicked", G_CALLBACK(show_about_dialog), window);
    gtk_grid_attach(GTK_GRID(grid), about_button, 0, 3, 3, 1);

    label_timer=gtk_label_new("Enter hours, minutes, and seconds and start the timer");
    gtk_grid_attach(GTK_GRID(grid), label_timer, 0, 4, 3, 1);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

