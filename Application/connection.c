//
// Created by matan on 16/03/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "connection.h"
#include <glib.h>
#include <gio/gio.h>

int client_socket_connected;

/**
 * connect to client function
 * open socket listener to inaddrany and port 5001
 * @return 1 - success
 * @return 0 - failed
 */
int connect_to_client() {
    int sockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    client_socket_connected = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (client_socket_connected < 0) {
        perror("ERROR on accept");
        return 0;
    } else {
        return 1;
    }
}
char* recvMessage() {
    int n;
    char buffer[MAX_CHARACTERS_IN_STRING];

    /* If connection is established then start communicating */
    bzero(buffer, MAX_CHARACTERS_IN_STRING);
    n = read(client_socket_connected, buffer, MAX_CHARACTERS_IN_STRING -1);

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Here is the message: %s\n",buffer);

    return buffer;
}

void sendMessage(char* message){
    int n;
    /* Write a response to the client */
    n = write(client_socket_connected,message,strlen(message));

    if (n < 0) {
        perror("ERROR writing to socket");
        exit (1);
    }
}

int disconnect(){
    close(client_socket_connected);
}

GSocketClient* gconnect() {
    /* initialize glib */
    g_type_init();

    GError *error = NULL;

    /* create a new connection */
    GSocketConnection *connection = NULL;
    GSocketClient *client = g_socket_client_new();

    /* connect to the host */
    connection = g_socket_client_connect_to_host(client,
                                                 (gchar *) "localhost",
                                                 1500, /* your port goes here */
                                                 NULL,
                                                 &error);

    /* don't forget to check for errors */
    if (error != NULL) {
        g_error (error->message);
    }
    else {
        g_print("Connection successful!\n");
    }
    return client;
}

//    /* use the connection */
//    GInputStream * istream = g_io_stream_get_input_stream (G_IO_STREAM (connection));
//    GOutputStream * ostream = g_io_stream_get_output_stream (G_IO_STREAM (connection));
//    g_output_stream_write  (ostream,
//                            "Hello server!", /* your message goes here */
//                            13, /* length of your message */
//                            NULL,
//                            &error);
//    /* don't forget to check for errors */
//    if (error != NULL)
//    {
//        g_error (error->message);
//    }
//    return 0;
//}