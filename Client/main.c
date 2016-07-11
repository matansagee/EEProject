//
// Created by shani on 08/06/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <glib.h>
#include <gio/gio.h>
#include <arpa/inet.h>

#define MAX_CHARACTERS_IN_STRING 256


int main(int argc, char** argv)
{
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
    puts("Socket created");

    //Prepare the sockaddr_in structure
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("132.66.199.244");
    client.sin_port = htons( 5222 );

    connect( socket_desc, (struct sockaddr *) &client, sizeof(client));
    if ( socket_desc == 0 ) {
        perror("Error at socket()");
        return 1;
    }

    //send connect signal
    num_bytes_send = write(socket_desc , "signal" , strlen("signal"));
    if (num_bytes_send==-1){
        printf("send failed.\n");
    }

    while( (read_size = recv(socket_desc , client_message , MAX_CHARACTERS_IN_STRING , 0)) > 0 )
    {
        //end of string marker
        client_message[read_size] = '\0';
        char *command,*arg;
        *command = strtok (client_message,":");
        *arg = strtok (NULL,":");

        

        //clear the message buffer
        memset(client_message, 0, 2000);
    }
}

