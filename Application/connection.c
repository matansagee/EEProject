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
#include <arpa/inet.h>

int socket_desc;
int connected = 0;
struct sockaddr_in client;
/**
 * connect to client function
 * open socket listener to inaddrany and port 5001
 * @return 1 - success
 * @return 0 - failed
 */

void init_socket()
{
    //Prepare the sockaddr_in structure
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("132.66.199.244");
    client.sin_port = htons( 5222);
}
int connect_to_client()
{
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    if (connect( socket_desc, (struct sockaddr *) &client, sizeof(client)) == -1){
        perror("connection failed");
        return 0;
    };
    if ( socket_desc == 0 ) {
        perror("socket failed");
        return 0;
    }

    puts("connection established");
    connected = 1;
    return 1;
}

/**
 * return 1 if succeeded
 * 0 if failed
 */
int sendMessage(char* message){
    if (!connected) return 0;
    int number_of_bytes_returned = write(socket_desc , message , strlen(message));
    return (number_of_bytes_returned == strlen(message));
}


void disconnect(){
    if(!connected) return;
    connected = 0;
    close(socket_desc);
}

int connection_status(){
    return connected;
}