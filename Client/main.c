// Created by shani on 08/06/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include <gst/gst.h>
#include <glib.h>

#define MAX_CHARACTERS_IN_STRING 256

GstElement *pipeline, *alsasrc , *queue, *audioresample, *audioconvert, *mulawenc ,*rtppcmupay ,*udpsink;
//gst-launch-1.0 alsasrc device="hw:1,0" ! queue ! audioresample ! audioconvert !  mulawenc ! rtppcmupay ! udpsink host=132.66.199.244 port=5600 sync=false &
//GstElement *pipeline, *udpsrc , *queue ,*rtppcmudepay ,*mulawdec,*audioconvert,*autoaudiosink;
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


    //for broadcasting the mic
    alsasrc   = gst_element_factory_make ("alsasrc","alsasrc");
    queue  = gst_element_factory_make ("queue","queue");
    audioresample = gst_element_factory_make ("audioresample","audioresample");
    audioconvert = gst_element_factory_make ("audioconvert","audioconvert");
    mulawenc   = gst_element_factory_make ("mulawenc","mulawenc");
    rtppcmupay   = gst_element_factory_make ("rtppcmupay","rtppcmupay");
    udpsink   = gst_element_factory_make ("udpsink","udpsink");

    if (!pipeline || !alsasrc || !queue || !audioresample || !audioconvert || !mulawenc || !rtppcmupay || !udpsink) {
        g_printerr ("One element could not be created. Exiting.\n");
        return;
    }

    g_object_set(G_OBJECT(alsasrc), "device", "hw:1,0", NULL);
    g_object_set(G_OBJECT(udpsink), "host", "132.66.199.244", NULL);
    g_object_set(G_OBJECT(udpsink), "sync", "false", NULL);
    g_object_set(G_OBJECT(udpsink), "port", 5600, NULL);


    //we add a message handler
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    bus_watch_id = gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    gst_bin_add_many (GST_BIN (pipeline), alsasrc,queue,audioresample,audioconvert,mulawenc,rtppcmupay,udpsink, NULL);
    gst_element_link_many (alsasrc,queue,audioresample,audioconvert,mulawenc,rtppcmupay,udpsink, NULL);

/*    //for receiving the mic
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
  */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    /* Iterate */
    g_print ("Running...\n");
    g_main_loop_run (loop);

    /* Out of the main loop, clean up nicely */
    g_print ("Returned, stopping playback\n");
    gst_element_set_state (pipeline, GST_STATE_NULL);

    g_print ("Deleting pipeline\n");
    gst_object_unref (GST_OBJECT (pipeline));
    g_source_remove (bus_watch_id);
    g_main_loop_unref (loop);

}

void stream_audio_to_server() {
    play = 1;
    create_loop();
}

void stop_stream_audio_to_server(){
    if (!play) return;
    g_main_loop_quit (loop);
}

const int device_1 = 06;
const int device_2 = 13;
const int device_3 = 19;
const int device_4 = 26;
const int connect_ind = 21;

void init_gpio(){
//    wiringPiSetup();  //Intalized wiringPi's simlified number system
    wiringPiSetupGpio();
    pinMode (device_1, OUTPUT);
    pinMode (device_2, OUTPUT);
    pinMode (device_3, OUTPUT);
    pinMode (device_4, OUTPUT);
    pinMode (connect_ind, OUTPUT);
}

int main(int argc, char** argv)
{
    gst_init(&argc,&argv);

    struct sockaddr_in client;
    int read_size;
    int num_bytes_send;
    char* client_message = (char*) malloc(MAX_CHARACTERS_IN_STRING * sizeof(char));
    if (client_message==NULL){
        printf("allocating memory failed.\n");
        exit(1);
    }
    //Create socket
    int socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    //Prepare the sockaddr_in structure
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("132.66.199.244");
    client.sin_port = htons( 5222 );

    if (connect( socket_desc, (struct sockaddr *) &client, sizeof(client)) == -1){
        perror("connection failed");
        return 1;
    };
    if ( socket_desc == 0 ) {
        perror("Error at socket()");
        return 1;
    }

    printf("connection established\n");
    stream_audio_to_server();

    init_gpio();
    printf("setting GPIOs\n");

    while( (read_size = recv(socket_desc , client_message , MAX_CHARACTERS_IN_STRING , 0)) > 0 )
    {
        char *device_num,*command;

        client_message[read_size] = '\0';
        printf("message: %s\n",client_message);

        device_num = strtok (client_message,":"); // 1,2,3,4
        command = strtok (NULL,":"); //start,stop
        if (atoi(device_num)==0){
            continue;
        }
        switch(atoi(device_num))
        {
            case 100:
                if (!strcmp(command,"start")){
                    digitalWrite(connect_ind, HIGH);
                } else if(!strcmp(command,"stop")) {
                    digitalWrite(connect_ind, LOW);
                    digitalWrite(device_1, LOW);
                    digitalWrite(device_2, LOW);
                    digitalWrite(device_3, LOW);
                    digitalWrite(device_4, LOW);
                }
                break;
            case 1:
                if (!strcmp(command,"start")){
                    digitalWrite(device_1, HIGH);
                } else if(!strcmp(command,"stop")) {
                    digitalWrite(device_1, LOW);
                }
                break;
            case 2:
                if (!strcmp(command,"start")){
                    digitalWrite(device_2, HIGH);
                } else if(!strcmp(command,"stop")) {
                    digitalWrite(device_2, LOW);
                }
                break;
            case 3:
                if (!strcmp(command,"start")){
                    digitalWrite(device_3, HIGH);
                } else if(!strcmp(command,"stop")) {
                    digitalWrite(device_3, LOW);
                }
                break;
            case 4:
                if (!strcmp(command,"start")){
                    digitalWrite(device_4, HIGH);
                } else if(!strcmp(command,"stop")) {
                    digitalWrite(device_4, LOW);
                }
                break;
            default:
                break;
        }
        //clear the message buffer
        memset(client_message, 0, 2000);
    }

    if (read_size<=0)
    {
        printf("session terminated\n");
        digitalWrite(connect_ind, LOW);
        digitalWrite(device_1, LOW);
        digitalWrite(device_2, LOW);
        digitalWrite(device_3, LOW);
        digitalWrite(device_4, LOW);
        printf("all gpio's set to zero\n");
        stop_stream_audio_to_server();
    }

    printf("Closing session\n");
}



