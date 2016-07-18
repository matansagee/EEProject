// Created by shani on 08/06/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
//#include <wiringPi.h>
#include <glib.h>
#include <gtk/gtk.h>


#define MAX_CHARACTERS_IN_STRING 256
#define MAX_NUMBER_OF_TO_ALLOWED 3

int socket_desc;
const int device_1 = 06;
const int device_2 = 13;
const int device_3 = 19;
const int device_4 = 26;
const int connect_ind = 21;

void init_gpio() {
//    wiringPiSetup();  //Intalized wiringPi's simlified number system
//    wiringPiSetupGpio();
//    pinMode (device_1, OUTPUT);
//    pinMode (device_2, OUTPUT);
//    pinMode (device_3, OUTPUT);
//    pinMode (device_4, OUTPUT);
//    pinMode (connect_ind, OUTPUT);
}

int main(int argc, char **argv) {
    printf("setting GPIOs\n");
    init_gpio();

    struct sockaddr_in client;
    int read_size;
    int number_of_bytes_returned;
    int num_bytes_send;
    int to_counter = 0;
    struct timeval tv;

    tv.tv_sec = 5;  /* 30 Secs Timeout */
    tv.tv_usec = 0;  // Not init'ing this can cause strange errors

    char *client_message = (char *) malloc(MAX_CHARACTERS_IN_STRING * sizeof(char));
    if (client_message == NULL) {
        printf("allocating memory failed.\n");
        return 1;
    }

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }

    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("10.0.0.1");//("127.0.0.1");//("132.66.199.244");
    client.sin_port = htons(5222);

    printf("trying to connect\n");
    if (connect(socket_desc, (struct sockaddr *) &client, sizeof(client)) == -1) {
        perror("connection failed");
        return 1;
    };

    if (setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv, sizeof(struct timeval)) == 0) {
        printf("Timeout on socket created\n");
    } else {
        perror("Timeout on socket failed");
        return 1;
    }

    printf("connection established\n");
    number_of_bytes_returned = write(socket_desc, "client", strlen("client"));
    if (number_of_bytes_returned <= 0) {
        printf("Send message to socket failed\n");
        return 1;
    }

    while (TRUE) {
        while ((read_size = recv(socket_desc, client_message, MAX_CHARACTERS_IN_STRING, 0)) > 0) {
            char *command, *arg;

            client_message[read_size] = '\0';
            printf("incoming message: %s\n", client_message);

            if (!strcmp(client_message, "connected")) {
                printf("client connected to server\n");
            }
            else if (!strcmp(client_message, "status")) {
                printf("\tsend message live to server\n");
                if (write(socket_desc, "live", strlen("live")) <= 0) {
                    printf("Send message to socket failed\n");
                    break;
                }
            }
            else {
                command = strtok(client_message, ":");
                arg = strtok(NULL, ":");

                if (arg == NULL) {
                    memset(client_message, 0, MAX_CHARACTERS_IN_STRING);
                    continue;
                }
                switch (atoi(command)) {
                    case 0:
                        to_counter = 0;
                        if (atoi(arg) == 1) {
                            printf("\tapplication is not connected\n");
//                    digitalWrite(connect_ind, LOW);
//                    digitalWrite(device_1, LOW);
//                    digitalWrite(device_2, LOW);
//                    digitalWrite(device_3, LOW);
//                    digitalWrite(device_4, LOW);
                        } else {
                            printf("\tapplication is connected\n");
                        }
                        break;
                    case 100:
                        if (!strcmp(arg, "start")) {
//                    digitalWrite(connect_ind, HIGH);
                        } else if (!strcmp(arg, "stop")) {
//                    digitalWrite(connect_ind, LOW);
//                    digitalWrite(device_1, LOW);
//                    digitalWrite(device_2, LOW);
//                    digitalWrite(device_3, LOW);
//                    digitalWrite(device_4, LOW);
                        }
                        break;
                    case 1:
                        if (!strcmp(arg, "start")) {
//                    digitalWrite(device_1, HIGH);
                        } else if (!strcmp(arg, "stop")) {
//                    digitalWrite(device_1, LOW);
                        }
                        break;
                    case 2:
                        if (!strcmp(arg, "start")) {
//                    digitalWrite(device_2, HIGH);
                        } else if (!strcmp(arg, "stop")) {
//                    digitalWrite(device_2, LOW);
                        }
                        break;
                    case 3:
                        if (!strcmp(arg, "start")) {
//                    digitalWrite(device_3, HIGH);
                        } else if (!strcmp(arg, "stop")) {
//                    digitalWrite(device_3, LOW);
                        }
                        break;
                    case 4:
                        if (!strcmp(arg, "start")) {
//                    digitalWrite(device_4, HIGH);
                        } else if (!strcmp(arg, "stop")) {
//                    digitalWrite(device_4, LOW);
                        }
                        break;
                    default:
                        break;
                }
            }
            //clear the message buffer
            memset(client_message, 0, MAX_CHARACTERS_IN_STRING);

        }

        if (read_size == 0) {
            //The return value will be 0 when the peer has performed an orderly shutdown.
            printf("server performed an orderly shutdown.\n");
            break;
        }

        if (number_of_bytes_returned <= 0) {
            printf("server disconnected\n");
            break;
        }

        printf("Socket timeout reached,check if server is live\n");
        to_counter++;

        number_of_bytes_returned = write(socket_desc, "status", strlen("status"));
        if (number_of_bytes_returned <= 0) {
            printf("Send message to socket failed\n");
            break;
        }

        if (to_counter == MAX_NUMBER_OF_TO_ALLOWED) {
            printf("server is not responding in the last %d sec\n", (int) tv.tv_sec * to_counter);
            break;
        }
    }

    printf("session terminated\n");
//    digitalWrite(connect_ind, LOW);
//    digitalWrite(device_1, LOW);
//    digitalWrite(device_2, LOW);
//    digitalWrite(device_3, LOW);
//    digitalWrite(device_4, LOW);
    printf("all gpio's set to zero\n");
    printf("Closing session\n");
    close(socket_desc);
}