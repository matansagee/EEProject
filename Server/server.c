#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

#ifndef    FALSE
#define    FALSE    (0)
#endif

#ifndef    TRUE
#define    TRUE    (!FALSE)
#endif

#define MAX_CHARACTERS_IN_STRING 256
#define MAX_NUMBER_OF_ACTIVE_LINKS 2
#define MAX_NUMBER_OF_TO_ALLOWED 3

int volatile *clients_sockets;
int volatile number_of_active_links = 0;

//the thread function
void *connection_handler(void *);

int main(int argc, char *argv[]) {
    printf("hello, i'm server.\ncan i make you some Java?\n");
    int socket_desc, client_sock, c;
    struct sockaddr_in server, client;
    int i;
    clients_sockets = (int *) malloc(MAX_NUMBER_OF_ACTIVE_LINKS * sizeof(int));
    if (clients_sockets == NULL) {
        printf("allocating memory failed.\n");
        return 1;
    }
    for (i = 0; i < MAX_NUMBER_OF_ACTIVE_LINKS; i++) {
        clients_sockets[i] = 0;
    }
    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    printf("server socket created\n");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5222);

    //Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while ((client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c))) {
        if (number_of_active_links >= MAX_NUMBER_OF_ACTIVE_LINKS) {
            char *message = "number of connection are in maximum state, please try later";
            write(client_sock, message, strlen(message));
            close(client_sock);
            continue;
        }

        if (pthread_create(&thread_id, NULL, connection_handler, (void *) &client_sock) < 0) {
            perror("could not create thread");
            return 1;
        }

        pthread_detach(thread_id);
    }

    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }

    close(socket_desc);
    return 0;
}

void *connection_handler(void *socket_desc) {
    printf("New connection accepted\n");
    int sock = *(int *) socket_desc;
    int read_size;
    int to_counter = 0;
    int number_of_bytes_returned;
    char *client_message;
    int id;
    struct timeval tv;
    tv.tv_sec = 5;  /* 30 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors

    client_message = (char *) malloc(MAX_CHARACTERS_IN_STRING * sizeof(char));
    if (client_message == NULL) {
        printf("allocating memory failed.\n");
        close(sock);
        return 0;
    }

    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(struct timeval)) == 0) {
        printf("Timeout on socket-%d created\n", sock);
    } else {
        perror("Timeout on socket- failed");
        close(sock);
        return 0;
    }

    read_size = recv(sock, client_message, MAX_CHARACTERS_IN_STRING, 0);

    if (read_size > 0) {
        client_message[read_size] = '\0';
        if (!strcmp(client_message, "client")) {
            printf("client connected to server\n");
            number_of_active_links++;
            clients_sockets[0] = sock;
            id = 0;
            number_of_bytes_returned = write(clients_sockets[id], "connected", strlen("connected"));
        } else if (!strcmp(client_message, "app")) {
            printf("application connected to server\n");
            number_of_active_links++;
            clients_sockets[1] = sock;
            id = 1;
            number_of_bytes_returned = write(clients_sockets[id], "connected", strlen("connected"));
        } else {
            close(sock);
            return 0;
        }
    } else {
        close(sock);
        return 0;
    }

    if (number_of_bytes_returned <= 0) {
        printf("%d:\tSend failed due to disconnected peer\n", id);
        number_of_active_links--;
        close(sock);
        return 0;
    }

    memset(client_message, 0, MAX_CHARACTERS_IN_STRING);
    printf("Number of active connections: %d\n", number_of_active_links);

    while (TRUE) {

        while ((read_size = recv(clients_sockets[id], client_message, MAX_CHARACTERS_IN_STRING, 0)) > 0) {
            to_counter = 0;
            client_message[read_size] = '\0';
            printf("%d:\tmessage:%s\n", id, client_message);

            if (!strcmp(client_message, "status")) {
                sprintf(client_message, "0:%d", number_of_active_links);
                number_of_bytes_returned = write(clients_sockets[id], client_message, strlen(client_message));
                if (number_of_bytes_returned <= 0) {
                    printf("%d:\tSend failed due to disconnected peer\n", id);
                    break;
                };
            }
            else if (number_of_active_links == MAX_NUMBER_OF_ACTIVE_LINKS) {
                number_of_bytes_returned = write(clients_sockets[1 - id], client_message, strlen(client_message));
                if (number_of_bytes_returned <= 0) {
                    printf("%d:\tSend failed due to disconnected opposite peer\n", id);
                    break;
                }
            };
            //clear the message buffer
            memset(client_message, 0, MAX_CHARACTERS_IN_STRING);
        }

        if (read_size == 0) {
            //The return value will be 0 when the peer has performed an orderly shutdown.
            printf("%d:\tpeer performed an orderly shutdown.\n", id);
            break;
        }
        if (number_of_bytes_returned <= 0) {
            printf("%d:\tSend failed due to disconnected peer\n", id);
            break;
        }

        printf("%d:\tSocket timeout reached\n", id);
        to_counter++;

        if (to_counter == MAX_NUMBER_OF_TO_ALLOWED) {
            printf("%d:\ttimeout counter reached maximum attempt allowed, closing connection\n", id);
            break;
        }


        if (id == 0) {
            //if it's client socket - check if live.
            printf("%d:\tcheck if client is live\n",id);

            number_of_bytes_returned = write(clients_sockets[id], "status", strlen("status"));
            if (number_of_bytes_returned <= 0) {
                printf("%d:\tSend message to socket failed\n",id);
                break;
            }
        }
    }

    printf("%d:\tthread ended\n", id);
    number_of_active_links--;
    close(sock);
    return 0;
}