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
    int read_size;
    char message[MAX_CHARACTERS_IN_STRING];
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

    read_size = recv(socket_desc , message , MAX_CHARACTERS_IN_STRING , 0);
    message[read_size] = '\0';
    if (read_size<=0){
        printf("connection failed\n");
        return 0;
    }
    if (atoi(message) == 2){
        printf("connection established to client through server\n");
        connected = 1;
    } else {
        printf("connection not established to client through server\n");
        connected = 0;
    }

    return connected;
}

/**
 * return 1 if succeeded
 * 0 if failed
 */
int sendMessage(char* message){
    int read_size;
    char* in_message[MAX_CHARACTERS_IN_STRING];

    if (!connected) return 0;
    int number_of_bytes_returned = write(socket_desc , message , strlen(message));
    if (number_of_bytes_returned <=0){
        disconnect();
        return 0;
    }
    read_size = recv(socket_desc , in_message , MAX_CHARACTERS_IN_STRING , 0);
    in_message[read_size] = '\0';
    printf("number of active connection in the server %d\n",atoi(in_message));
    if (read_size<=0){
        printf("connection failed\n");
        return 0;
    }
    if (atoi(in_message) == 1) {
        disconnect();
        return 0;
    }
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