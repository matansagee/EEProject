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

int socket_desc;

/**
 * connect to client function
 * open socket listener to inaddrany and port 5001
 * @return 1 - success
 * @return 0 - failed
 */
int connect_to_client() {

    struct sockaddr_in client;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    client.sin_port = htons( 8888 );

    connect( socket_desc, (struct sockaddr *) &client, sizeof(client));
    if ( socket_desc == 0 ) {
        perror("Error at socket()");
        return 0;
    }
    return 1;
}


/**
 * return message if o.k
 * NULL if failed
 */
char* recvMessage() {

    int read_size;
    char* client_message = (char*) malloc(MAX_CHARACTERS_IN_STRING*sizeof(char));
    if (client_message==NULL)
    {
        printf("failed allocating memmory.\n");
        exit(1);
    }
    read_size = recv(socket_desc , client_message , MAX_CHARACTERS_IN_STRING , 0);

    if(read_size == 0)
    {
        printf("server disconnected\n");
        return NULL;
    }
    else if(read_size == -1)
    {
        printf("recv failed");
        return NULL;
    }

    return client_message;
}

/**
 * return 1 if succeeded
 * 0 if failed
 */
int sendMessage(char* message){
    int number_of_bytes_returned = write(socket_desc , message , strlen(message));
    return (number_of_bytes_returned == strlen(message));
}

int disconnect(){
    close(socket_desc);
}

