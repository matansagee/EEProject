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
int to_counter = 0;

/**
 * connect to client function
 * open socket listener to inaddrany and port 5001
 * @return 1 - success
 * @return 0 - failed
 */

void init_socket() {
    //Prepare the sockaddr_in structure
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("132.66.199.244");
    client.sin_port = htons(5222);
}

int connect_to_client() {
    struct timeval tv;
    //Create socket
    printf("starting connect process\n");
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    int read_size;
    char *in_message = (char *) malloc(MAX_CHARACTERS_IN_STRING * sizeof(char *));

    if (in_message == NULL) {
        printf("error allocating memory\n");
        exit(1);
    }

    if (connect(socket_desc, (struct sockaddr *) &client, sizeof(client)) == -1) {
        perror("connection failed");
        return 0;
    };
    if (socket_desc == 0) {
        perror("socket failed");
        return 0;
    }

    tv.tv_sec = 5;  /* 30 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors

    if (setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(struct timeval)) == 0) {
        printf("Timeout on socket-%d created\n", socket_desc);
    } else {
        perror("Timeout on socket- failed");
        return 1;
    }

    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    int number_of_bytes_returned = write(socket_desc, "app", strlen("app"));

    if (number_of_bytes_returned <= 0) {
        disconnect();
        return 0;
    }

    read_size = recv(socket_desc, in_message, MAX_CHARACTERS_IN_STRING, 0);
    in_message[read_size] = '\0';

    if (read_size <= 0) {
        printf("connection failed\n");
        close(socket_desc);
        connected = 0;
        return connected;
    }
    printf("%s\n", in_message);
    if (strcmp(in_message, "connected")) {
        printf("app is not connected to server");
        connected = 0;
        return connected;
    }

    number_of_bytes_returned = write(socket_desc, "status", strlen("status"));
    if (number_of_bytes_returned <= 0) {
        disconnect();
        return 0;
    }

    read_size = recv(socket_desc, in_message, MAX_CHARACTERS_IN_STRING, 0);
    in_message[read_size] = '\0';

    if (read_size <= 0) {
        printf("connection failed\n");
        close(socket_desc);
        connected = 0;
    }
    printf("incoming message: %s\n", in_message);

    if (!strcmp(in_message, "0:2")) {
        printf("connection established to client through server\n");
        connected = 1;
    } else {
        printf("connection not established to client through server\n");
        close(socket_desc);
        connected = 0;
    }

    return connected;
}

/**
 * return 1 if succeeded
 * 0 if failed
 */
int sendMessage(char *message) {

    if (!connected) return 0;
    int number_of_bytes_returned = write(socket_desc, message, strlen(message));
    if (number_of_bytes_returned <= 0) {
        disconnect();
        return 0;
    }
    return (number_of_bytes_returned == strlen(message));
}

char *recvMessage() {
    if (!connected) return NULL;

    int read_size;
    char *in_message = (char *) malloc(MAX_CHARACTERS_IN_STRING * sizeof(char *));

    if (in_message == NULL) {
        printf("error allocating memory\n");
        exit(1);
    }

    read_size = recv(socket_desc, in_message, MAX_CHARACTERS_IN_STRING, 0);

    if (read_size == 0) {
        //The return value will be 0 when the peer has performed an orderly shutdown.
        printf("peer performed an orderly shutdown\n");
        disconnect();
        return NULL;

    } else if (read_size == -1) {
        printf("connection failed\n");
        disconnect();
        return NULL;
    }
    in_message[read_size] = '\0';

    return in_message;
}


void disconnect() {
    if (!connected) return;
    printf("closing socket\n");
    to_counter = 0;
    connected = 0;
    close(socket_desc);
}

int connection_status() {
    if(!connected) return connected;

    printf("checking connection status - to_counter:%d\n",to_counter);
    if (to_counter == MAX_NUMBER_OF_TO_ALLOWED) {
        disconnect();
        return connected;
    }

    if (to_counter == 0){
        //in the first time that connection_status call, dont send message to server.
        to_counter++;
        return connected;
    }

    if (sendMessage("status") == 0) {
        return connected;
    };

    char *in_message = recvMessage();
    if (in_message == NULL) {
        to_counter++;
    } else {
        printf("incoming message from server: %s\n", in_message);
        if (!strcmp(in_message, "0:1")) {
            printf("client is not connected\n");
            disconnect();
        } else if (!strcmp(in_message, "0:2")) {
            printf("client is connected\n");
        }
        to_counter = 0;
    }

    return connected;
}