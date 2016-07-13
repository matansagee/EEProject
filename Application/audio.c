#include <gst/gst.h>
#include <glib.h>
#include "audio.h"
#include<stdio.h>


//GstElement *pipeline, *alsasrc , *mulawenc ,*rtppcmupay ,*udpsink;
GstElement *pipeline, *udpsrc , *queue ,*rtppcmudepay ,*mulawdec,*audioconvert,*autoaudiosink;
int play =0;
GMainLoop *loop;

static gboolean
bus_call (GstBus     *bus,
          GstMessage *msg,
          gpointer    data)
{
    GMainLoop *loop = (GMainLoop *) data;

    switch (GST_MESSAGE_TYPE (msg)) {

        case GST_MESSAGE_EOS:
            g_print ("End of stream\n");
            g_main_loop_quit (loop);
            break;

        case GST_MESSAGE_ERROR: {
            gchar  *debug;
            GError *error;

            gst_message_parse_error (msg, &error, &debug);
            g_free (debug);

            g_printerr ("Error: %s\n", error->message);
            g_error_free (error);

            g_main_loop_quit (loop);
            break;
        }
        default:
            break;
    }

    return TRUE;
}


void create_loop()
{

    GstBus *bus;
    guint bus_watch_id;
    GstCaps *rx_caps;

    /* Initialisation */
    loop = g_main_loop_new (NULL, FALSE);

    /* Create gstreamer elements */
    pipeline = gst_pipeline_new ("audio-player");

/*
    //for broadcasting the mic
    alsasrc   = gst_element_factory_make ("alsasrc","alsasrc");
    mulawenc   = gst_element_factory_make ("mulawenc","mulawenc");
    rtppcmupay   = gst_element_factory_make ("rtppcmupay","rtppcmupay");
    udpsink   = gst_element_factory_make ("udpsink","udpsink");

    if (!pipeline || !alsasrc || !mulawenc || !rtppcmupay || !udpsink) {
        g_printerr ("One element could not be created. Exiting.\n");
        return;
    }

    g_object_set(G_OBJECT(udpsink), "host", "127.0.0.1", NULL);
    g_object_set(G_OBJECT(udpsink), "port", 5600, NULL);


    //we add a message handler
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    gst_bin_add_many (GST_BIN (pipeline), alsasrc,mulawenc,rtppcmupay,udpsink, NULL);
    gst_element_link_many (alsasrc,mulawenc,rtppcmupay,udpsink, NULL);
*/
    //for receiving the mic
    udpsrc   = gst_element_factory_make ("udpsrc","udpsrc");
    queue   = gst_element_factory_make ("queue","queue");
    rtppcmudepay   = gst_element_factory_make ("rtppcmudepay","rtppcmudepay");
    mulawdec = gst_element_factory_make ("mulawdec","mulawdec");
    audioconvert   = gst_element_factory_make ("audioconvert","audioconvert");
    autoaudiosink   = gst_element_factory_make ("alsasink","alsasink");

    rx_caps = gst_caps_new_simple("application/x-rtp",
                                  "media", G_TYPE_STRING, "audio",
                                  "clock-rate", G_TYPE_INT, 8000,
                                  "encoding-name", G_TYPE_STRING, "PCMU",
                                  NULL);

    if (!pipeline || !udpsrc || !queue || !rtppcmudepay || !mulawdec || !audioconvert ||!autoaudiosink) {
        g_printerr ("One element could not be created. Exiting.\n");
        return;
    }

    g_object_set(G_OBJECT(udpsrc), "port", 5601, NULL);
    g_object_set(G_OBJECT(udpsrc), "caps",rx_caps, NULL);
    gst_caps_unref (rx_caps);

    //we add a message handler
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    gst_bin_add_many (GST_BIN (pipeline),udpsrc,queue,rtppcmudepay,mulawdec,audioconvert,autoaudiosink, NULL);
    gst_element_link_many (udpsrc,queue,rtppcmudepay,mulawdec,audioconvert,autoaudiosink, NULL);
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    /* Iterate */
    g_print ("Running...\n");
    g_main_loop_run (loop);

    /* Out of the main loop, clean up nicely */
    g_print ("Returned, stopping playback\n");
    printf ("Returned, stopping playback\n");
    gst_element_set_state (pipeline, GST_STATE_NULL);

    g_print ("Deleting pipeline\n");
    gst_object_unref (GST_OBJECT (pipeline));
    g_source_remove (bus_watch_id);
    g_main_loop_unref (loop);

}

void play_audio_from_mic() {
    play = 1;
    create_loop();
}

void stop_audio_from_mic(){
    if (!play) return;
    play = 0;
    g_main_loop_quit (loop);
}