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
#include <wiringPi>

#define MAX_CHARACTERS_IN_STRING 256

const int device_1 = 06;
const int device_2 = 13;
const int device_3 = 19;
const int device_4 = 26;
const int connect_ind = 21;

void init_gpio(){
    wiringPiSetup();  //Intalized wiringPi's simlified number system
    wiringPiSerupGpio();
    pinMode (device_1, OUTPUT);
    pinMode (device_2, OUTPUT);
    pinMode (device_3, OUTPUT);
    pinMode (device_4, OUTPUT);
    pinMode (connect_ind, OUTPUT);
}

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

    init_gpio();


    while( (read_size = recv(socket_desc , client_message , MAX_CHARACTERS_IN_STRING , 0)) > 0 )
    {
        //end of string marker
        client_message[read_size] = '\0';
        char *device_num,*command;
        device_num = strtok (client_message,":"); // 1,2,3,4
        command = strtok (NULL,":"); //start,stop
        switch(atoi(device_num))
        {
            case 0:
                if (!strcmp(command,"start")){
                    digitalWrite(connect_ind, HIGH);
                } else if(!strcmp(command,"stop")) {
                    digitalWrite(connect_ind, LOW);
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
        }

        //clear the message buffer
        memset(client_message, 0, 2000);
    }
}
