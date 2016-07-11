//
// Created by matan on 04/01/16.
//
#include <stdio.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <math.h>
#include <gst/gst.h>
#include <string.h>

#include "connection.h"
#include "audio.h"

GtkWidget *window;
GtkWidget *buttonPlay;
GtkWidget *buttonStop;
GtkWidget *buttonConnect;
GtkWidget *grid;
GtkWidget *buttonDevice1;
GtkWidget *buttonDevice2;
GtkWidget *buttonDevice3;
GtkWidget *buttonDevice4;
GtkWidget *spinButtonDevice1;
GtkWidget *spinButtonDevice2;
GtkWidget *spinButtonDevice3;
GtkWidget *spinButtonDevice4;
GtkWidget *adjustment1;
GtkWidget *adjustment2;
GtkWidget *adjustment3;
GtkWidget *adjustment4;
GtkWidget *buttonClose;
GtkWidget *clockLabel;
GtkWidget *helpLabel;

void stop_device(GtkWidget *widget,gpointer data);
void spin_button_device1(GtkWidget *widget, gpointer data);
void button_function(GtkWidget *widget,gpointer data);
gboolean reset_help_label(gpointer label);
gboolean update_clock(gpointer label);


void spin_button_device1(GtkWidget *widget, gpointer data)
{
//    int value = gtk_spin_button_get_value_as_int(widget);
//    g_print ("new value - %d \n",value);
//    time_t curtime;
//
//    time(&curtime);
//    struct tm * timeinfo = localtime (&curtime);
//
//    // compute the angles of the indicators of our clock
//    int minutes = (int) timeinfo->tm_min * M_PI / 30;
//    int hours = (int) timeinfo->tm_hour * M_PI / 6;
//    int seconds= (int) timeinfo->tm_sec * M_PI / 30;
//    printf("Current time = %s\n", ctime(&curtime));
//    printf("%d %d %d\n",seconds,minutes,hours);

}

void button_function(GtkWidget *widget,gpointer data)
{
    char* label_string = gtk_button_get_label(widget);
    if (connection_status() == 0){
        gtk_label_set_text(GTK_LABEL(helpLabel), "please connect before turning on device");
        g_timeout_add_seconds (4, reset_help_label, (gpointer) helpLabel);
        return;
    }
    if (!strcmp(label_string,"Device1")){
        sendMessage("1:start");
        int value = gtk_spin_button_get_value_as_int(spinButtonDevice1);
        if (value==0)
        {
            gtk_label_set_text(GTK_LABEL(helpLabel), "set time");
            g_timeout_add_seconds (4, reset_help_label, (gpointer) helpLabel);
        } else {
            g_print ("new value - %d \n",value);
            g_timeout_add_seconds (value*60, stop_device, (gpointer) buttonDevice1);
        }

    } else if (!strcmp(label_string,"Device2")){

    } else if (!strcmp(label_string,"Device3")) {

    } else if (!strcmp(label_string,"Device4")) {

    }
}

void stop_device(GtkWidget *widget,gpointer data)
{
    char* label_string = gtk_button_get_label(widget);

    if (strcmp(label_string,"Device1")){
        sendMessage("1:stop");
    } else if (strcmp(label_string,"Device2")){
        sendMessage("2:stop");
    } else if (strcmp(label_string,"Device3")) {
        sendMessage("3:stop");
    } else if (strcmp(label_string,"Device4")) {
        sendMessage("4:stop");
    }
}


void play_function(GtkWidget *widget,gpointer data)
{
    sendMessage("play fucntion\n");
    play_audio_from_mic();
}

void stop_function(GtkWidget *widget,gpointer data)
{
    sendMessage("stop fucntion\n");
    stop_audio_from_mic();
}

void connect_function(GtkWidget *widget,gpointer data)
{
    char* current_string = gtk_button_get_label(widget);
    connect_to_client();
    if (connection_status() == 0) {
        return;
    }

    if (!g_strcmp0(current_string,"Connect")) {
        gtk_label_set_text(GTK_LABEL(widget), "Disconnect");
    }
    else{
        gtk_label_set_text(GTK_LABEL(widget), "Connect");
    }

}

gboolean reset_help_label(gpointer label){
    gtk_label_set_text(GTK_LABEL(label),"");
    return FALSE;
}

gboolean update_clock(gpointer label)
{
    /* the GtkLabel is passed in as user data */

    time_t simple_time;
    struct tm *time_info;
    gchar *str = g_new(gchar, 25);  /* allocate 25 characters for time */

    /* get simple time and convert it into a tm structure */
    time (&simple_time);
    time_info = localtime(&simple_time);

    /* %X = Preferred time of day representation for current locale. */
    strftime(str, 25, "%X", time_info);

    /* update the label */
    gtk_label_set_text(GTK_LABEL(label), str);

    g_free(str);    /* free memory*/

    return TRUE;
}


static void
activate (GtkApplication *app,    gpointer        user_data)
{

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Smart House");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

    clockLabel = gtk_label_new (NULL);
    helpLabel = gtk_label_new ("help");
    buttonPlay = gtk_button_new_with_label ("Play");
    buttonStop = gtk_button_new_with_label ("Stop");
    buttonClose = gtk_button_new_with_label("Close");
    buttonConnect = gtk_button_new_with_label ("Connect");
    buttonDevice1 = gtk_button_new_with_label ("Device1");
    buttonDevice2 = gtk_button_new_with_label ("Device2");
    buttonDevice3 = gtk_button_new_with_label ("Device3");
    buttonDevice4 = gtk_button_new_with_label ("Device4");

    /* Create a label to be shown in the window */
//    label = gtk_label_new ("Choose a number");

    /* Create an adjustment representing an adjustable bounded value */
    adjustment1 = gtk_adjustment_new (0, 0, 100, 1, 0, 0);
    adjustment2 = gtk_adjustment_new (0, 0, 100, 1, 0, 0);
    adjustment3 = gtk_adjustment_new (0, 0, 100, 1, 0, 0);
    adjustment4 = gtk_adjustment_new (0, 0, 100, 1, 0, 0);


    /* Create a spin button that is to be as wide as possible */
    spinButtonDevice1 = gtk_spin_button_new (adjustment1, 1, 0);
    spinButtonDevice2 = gtk_spin_button_new (adjustment2, 1, 0);
    spinButtonDevice3 = gtk_spin_button_new (adjustment3, 1, 0);
    spinButtonDevice4 = gtk_spin_button_new (adjustment4, 1, 0);
    gtk_widget_set_hexpand (spinButtonDevice1, TRUE);
    gtk_widget_set_hexpand (spinButtonDevice2, TRUE);
    gtk_widget_set_hexpand (spinButtonDevice3, TRUE);
    gtk_widget_set_hexpand (spinButtonDevice4, TRUE);

    /* Connecting the "value-changed" signal for the spinbutton
     * to the appropriate callback function.
     */


    grid = gtk_grid_new();
    gtk_grid_attach (GTK_GRID (grid),buttonConnect,0,0,2,1);
    gtk_grid_attach (GTK_GRID (grid),buttonPlay,0,1,1,1);
    gtk_grid_attach_next_to(GTK_GRID (grid),buttonStop,buttonPlay,GTK_POS_RIGHT,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonDevice1,0,2,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonDevice2,0,3,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonDevice3,0,4,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonDevice4,0,5,1,1);
    gtk_grid_attach (GTK_GRID (grid), spinButtonDevice1, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), spinButtonDevice2, 1, 3, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), spinButtonDevice3, 1, 4, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), spinButtonDevice4, 1, 5, 1, 1);
    gtk_grid_attach (GTK_GRID (grid),buttonClose,0,6,2,1);
    gtk_grid_attach(GTK_GRID (grid),helpLabel,0,7,2,1);
    gtk_grid_attach(GTK_GRID (grid),clockLabel,0,8,2,1);

    g_signal_connect (buttonClose, "clicked", G_CALLBACK (stop_function), NULL);
    g_signal_connect_swapped (buttonClose, "clicked", G_CALLBACK (gtk_widget_destroy), window);

    g_signal_connect (buttonConnect, "clicked", G_CALLBACK (connect_function), NULL);
    g_signal_connect (buttonPlay, "clicked", G_CALLBACK (play_function), NULL);
    g_signal_connect (buttonStop, "clicked", G_CALLBACK (stop_function), NULL);
    g_signal_connect (spinButtonDevice1,"value-changed", G_CALLBACK(spin_button_device1),NULL);
    g_signal_connect (buttonDevice1, "clicked", G_CALLBACK (button_function), NULL);

    gtk_container_add (GTK_CONTAINER (window), grid);

    /* set timeout */
    g_timeout_add (500, update_clock, (gpointer) clockLabel);

    gtk_widget_show_all (window);
}
int
main (int    argc, char **argv)
{
    gst_init(&argc,&argv);
    GtkApplication *app;
    int status;

    app = gtk_application_new ("smart.house", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}