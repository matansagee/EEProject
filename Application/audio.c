#include <gst/gst.h>
#include <glib.h>
#include "audio.h"
#include<stdio.h>


GstElement *pipeline, *source, *sink;
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

    /* Initialisation */

    loop = g_main_loop_new (NULL, FALSE);



    /* Create gstreamer elements */
    pipeline = gst_pipeline_new ("audio-player");
    source   = gst_element_factory_make ("alsasrc",       "alsa-source");
    sink     = gst_element_factory_make ("autoaudiosink", "audio-output");

    if (!pipeline || !source  || !sink) {
        g_printerr ("One element could not be created. Exiting.\n");
        return;
    }

    g_object_set (G_OBJECT(source),"device","hw:1,0",NULL);

/* we add a message handler */
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    gst_bin_add_many (GST_BIN (pipeline),
                      source,  sink, NULL);

    gst_element_link (source, sink);
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
    create_loop();
}

void stop_audio_from_mic(){
    g_main_loop_quit (loop);
}

void print_status_of_all(){}
//{
//    auto it  = gst_bin_iterate_elements(GST_BIN(_pipeline.raw()));
//    GValue value = G_VALUE_INIT;
//    for(GstIteratorResult r = gst_iterator_next(it, &value); r != GST_ITERATOR_DONE; r = gst_iterator_next(it, &value))
//    {
//        if ( r == GST_ITERATOR_OK )
//        {
//            GstElement *e = static_cast<GstElement*>(g_value_peek_pointer(&value));
//            GstState  current, pending;
//            auto ret = gst_element_get_state(e, &current, &pending, 100000);
//            g_print("%s(%s), status = %s, pending = %s\n", G_VALUE_TYPE_NAME(&value), gst_element_get_name(e), gst_element_state_get_name(current), gst_element_state_get_name(pending));
//        }
//    }
//}
