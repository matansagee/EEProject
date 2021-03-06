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

gboolean stop_device(GtkWidget *widget,gpointer data);
void button_function(GtkWidget *widget,gpointer data);
gboolean reset_help_label(gpointer label);
gboolean update_clock(gpointer label);

int device1_on =0;
int device2_on =0;
int device3_on =0;
int device4_on =0;

int connect_bool = 0;

void button_function(GtkWidget *widget,gpointer data)
{
    char* label_string = gtk_button_get_label(widget);

    if (connection_status() == 0){
        gtk_label_set_text(GTK_LABEL(helpLabel), "please connect");
        g_timeout_add_seconds (4, reset_help_label, (gpointer) helpLabel);
        return;
    }

    if (!strcmp(label_string,"Device1") || !strcmp(label_string,"Device1-On")){
        int value = gtk_spin_button_get_value_as_int(spinButtonDevice1);
        if (value==0)
        {
            gtk_label_set_text(GTK_LABEL(helpLabel), "set time");
            g_timeout_add_seconds (4, reset_help_label, (gpointer) helpLabel);
        } else if (!device1_on){
            sendMessage("1:start");
            device1_on = 1;
            gtk_button_set_label(GTK_BUTTON(widget), "Device1-On");
            g_print ("turning on device1 for %d seconds \n",value);
            g_timeout_add_seconds (value, stop_device, (gpointer) buttonDevice1);
        } else if (device1_on) {
            sendMessage("1:stop");
            device1_on = 0;
            gtk_button_set_label(GTK_BUTTON(widget), "Device1");
            g_print ("turning off device1\n");
        }

    } else if (!strcmp(label_string,"Device2") || !strcmp(label_string,"Device2-On")){
        int value = gtk_spin_button_get_value_as_int(spinButtonDevice2);
        if (value==0)
        {
            gtk_label_set_text(GTK_LABEL(helpLabel), "set time");
            g_timeout_add_seconds (4, reset_help_label, (gpointer) helpLabel);
        } else if (!device2_on){
            sendMessage("2:start");
            device2_on = 1;
            g_print ("turning on device2 for %d seconds \n",value);
            gtk_button_set_label(GTK_BUTTON(widget), "Device2-On");
            g_timeout_add_seconds (value, stop_device, (gpointer) buttonDevice2);

        } else if (device2_on) {
            sendMessage("2:stop");
            device2_on = 0;
            gtk_button_set_label(GTK_BUTTON(widget), "Device2");
            g_print ("turning off device2\n");
        }
    } else if (!strcmp(label_string,"Device3")  || !strcmp(label_string,"Device3-On")) {
        int value = gtk_spin_button_get_value_as_int(spinButtonDevice3);
        if (value==0)
        {
            gtk_label_set_text(GTK_LABEL(helpLabel), "set time");
            g_timeout_add_seconds (4, reset_help_label, (gpointer) helpLabel);
        } else if (!device3_on){
            sendMessage("3:start");
            device3_on = 1;
            gtk_button_set_label(GTK_BUTTON(widget), "Device3-On");
            g_print ("turning on device3 for %d seconds \n",value);
            g_timeout_add_seconds (value, stop_device, (gpointer) buttonDevice3);
        } else if (device3_on) {
            sendMessage("3:stop");
            device3_on = 0;
            gtk_button_set_label(GTK_BUTTON(widget), "Device3");
            g_print ("turning off device3\n");
        }
    } else if (!strcmp(label_string,"Device4")  || !strcmp(label_string,"Device4-On")) {
        int value = gtk_spin_button_get_value_as_int(spinButtonDevice4);
        if (value==0)
        {
            gtk_label_set_text(GTK_LABEL(helpLabel), "set time");
            g_timeout_add_seconds (4, reset_help_label, (gpointer) helpLabel);
        } else if (!device4_on){
            sendMessage("4:start");
            device4_on = 1;
            gtk_button_set_label(GTK_BUTTON(widget), "Device4-On");
            g_print ("turning on device4 for %d seconds \n",value);
            g_timeout_add_seconds (value, stop_device, (gpointer) buttonDevice4);
        } else if (device4_on) {
            sendMessage("4:stop");
            device4_on = 0;
            gtk_button_set_label(GTK_BUTTON(widget), "Device4");
            g_print ("turning off device4\n");
        }
    }
}

gboolean stop_device(GtkWidget *widget,gpointer data)
{
    if (connection_status()==0) {
        return FALSE;
    }
    char* label_string = gtk_button_get_label(widget);

    if (!strcmp(label_string,"Device1-On") && device1_on){
        device1_on = 0;
        sendMessage("1:stop");
        g_print ("turning off device1\n");
        gtk_button_set_label(GTK_BUTTON(widget), "Device1");
    } else if (!strcmp(label_string,"Device2-On") && device2_on){
        device2_on = 0;
        sendMessage("2:stop");
        g_print ("turning off device2\n");
        gtk_button_set_label(GTK_BUTTON(widget), "Device2");
    } else if (!strcmp(label_string,"Device3-On") && device3_on) {
        device3_on = 0;
        sendMessage("3:stop");
        g_print ("turning off device3\n");
        gtk_button_set_label(GTK_BUTTON(widget), "Device3");
    } else if (!strcmp(label_string,"Device4-On")  && device4_on) {
        device4_on = 0;
        sendMessage("4:stop");
        g_print ("turning off device4\n");
        gtk_button_set_label(GTK_BUTTON(widget), "Device4");
    }
    return FALSE;
}


void play_function(GtkWidget *widget,gpointer data)
{
    if (connection_status() == 0){
        gtk_label_set_text(GTK_LABEL(helpLabel), "please connect");
        g_timeout_add_seconds (4, reset_help_label, (gpointer) helpLabel);
    } else {
        play_audio_from_mic();
    }
}

void stop_function(GtkWidget *widget,gpointer data)
{
    stop_audio_from_mic();
}

void connect_function(GtkWidget *widget,gpointer data)
{
    if(!connect_bool && connection_status() == 0) {
        if (connect_to_client() == 0) {
            gtk_button_set_label(GTK_BUTTON(widget), "Connect");
            gtk_label_set_text(GTK_LABEL(helpLabel), "client offline");
            g_timeout_add_seconds(4, reset_help_label, (gpointer) helpLabel);
            return;
        } else {
            sendMessage("100:start");
            gtk_button_set_label(GTK_BUTTON(widget), "Disconnect");
            connect_bool = 1;
        }
    } else {
        connect_bool = 0;
        sendMessage("100:stop");
        gtk_button_set_label(GTK_BUTTON(buttonDevice1), "Device1");
        gtk_button_set_label(GTK_BUTTON(buttonDevice2), "Device2");
        gtk_button_set_label(GTK_BUTTON(buttonDevice3), "Device3");
        gtk_button_set_label(GTK_BUTTON(buttonDevice4), "Device4");
        stop_audio_from_mic();
        disconnect();
        gtk_button_set_label(GTK_BUTTON(widget), "Connect");
    }
}




gboolean reset_help_label(gpointer label){
    gtk_label_set_text(GTK_LABEL(label),"");
    return FALSE;
}

gboolean check_network_connectivity(gpointer connectButton){
    if (connection_status()){
        gtk_button_set_label(GTK_BUTTON(connectButton), "Disconnect");
    } else {
        gtk_button_set_label(GTK_BUTTON(connectButton), "Connect");
    }
    return TRUE;
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
    gtk_window_set_default_size (GTK_WINDOW (window), -1, -1);

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
    gtk_grid_attach (GTK_GRID (grid),buttonConnect,0,0,1,1);
    gtk_grid_attach (GTK_GRID (grid),clockLabel,1,0,2,1);
//    gtk_grid_attach_next_to(GTK_GRID (grid),clockLabel,buttonConnect,GTK_POS_RIGHT,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonPlay,2,1,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonStop,2,2,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonClose,2,3,1,1);
//    gtk_grid_attach_next_to(GTK_GRID (grid),buttonStop,buttonPlay,GTK_POS_RIGHT,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonDevice1,0,1,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonDevice2,0,2,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonDevice3,0,3,1,1);
    gtk_grid_attach (GTK_GRID (grid),buttonDevice4,0,4,1,1);
    gtk_grid_attach (GTK_GRID (grid), spinButtonDevice1, 1, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), spinButtonDevice2, 1, 2, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), spinButtonDevice3, 1, 3, 1, 1);
    gtk_grid_attach (GTK_GRID (grid), spinButtonDevice4, 1, 4, 1, 1);
//    gtk_grid_attach (GTK_GRID (grid),buttonClose,0,6,1,1);
//    gtk_grid_attach_next_to(GTK_GRID (grid),helpLabel,buttonClose,GTK_POS_RIGHT,1,1);
    gtk_grid_attach(GTK_GRID (grid),helpLabel,0,5,3,1);
//    gtk_grid_attach(GTK_GRID (grid),clockLabel,0,8,2,1);
    gtk_widget_set_size_request (buttonDevice1,100,10);
    gtk_widget_set_size_request (buttonDevice2,100,10);
    gtk_widget_set_size_request (buttonDevice3,100,10);
    gtk_widget_set_size_request (buttonDevice4,100,10);
    g_signal_connect (buttonClose, "clicked", G_CALLBACK (connect_function), NULL);
    g_signal_connect_swapped (buttonClose, "clicked", G_CALLBACK (gtk_widget_destroy), window);

    g_signal_connect (buttonConnect, "clicked", G_CALLBACK (connect_function), NULL);
    g_signal_connect (buttonPlay, "clicked", G_CALLBACK (play_function), NULL);
    g_signal_connect (buttonStop, "clicked", G_CALLBACK (stop_function), NULL);
    g_signal_connect (buttonDevice1, "clicked", G_CALLBACK (button_function), NULL);
    g_signal_connect (buttonDevice2, "clicked", G_CALLBACK (button_function), NULL);
    g_signal_connect (buttonDevice3, "clicked", G_CALLBACK (button_function), NULL);
    g_signal_connect (buttonDevice4, "clicked", G_CALLBACK (button_function), NULL);

    gtk_container_add (GTK_CONTAINER (window), grid);

    /* set timeout */
    g_timeout_add (500, update_clock, (gpointer) clockLabel);
    g_timeout_add (5000, check_network_connectivity, (gpointer) buttonConnect);

    gtk_widget_show_all (window);
}
int
main (int    argc, char **argv)
{
    gst_init(&argc,&argv);
    GtkApplication *app;
    int status;
    init_socket();

    app = gtk_application_new ("smart.house", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}