// Created by shani on 08/06/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include <glib.h>
#include <gtk/gtk.h>


#define MAX_CHARACTERS_IN_STRING 256
int socket_desc;
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
gboolean check_network_status(gpointer null){
    int number_of_bytes_returned = write(socket_desc , "status" , strlen("status"));
    if (number_of_bytes_returned <=0){
        printf("Closing session\n");
    	close(socket_desc);
        exit(1);
    }
    return TRUE;
}
int main(int argc, char** argv)
{
    struct sockaddr_in client;
    int read_size;	
    int num_bytes_send;
	printf("hello");
    gtk_main ();
    char* client_message = (char*) malloc(MAX_CHARACTERS_IN_STRING * sizeof(char));
   if (client_message==NULL){
        printf("allocating memory failed.\n");
        exit(1);
    }
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }

    //Prepare the sockaddr_in structure
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = inet_addr("10.0.0.1");
    client.sin_port = htons( 5222 );
    printf("trying to connect\n");
    if (connect( socket_desc, (struct sockaddr *) &client, sizeof(client)) == -1){
        perror("connection failed");
        return 1;
    };
    if ( socket_desc == 0 ) {
        perror("Error at socket()");
        return 1;
    }

    printf("connection established\n");

    init_gpio();
    printf("setting GPIOs\n");
    g_timeout_add (1000, check_network_status,NULL);
    	
    while((read_size = recv(socket_desc , client_message , MAX_CHARACTERS_IN_STRING , 0)) > 0 )
    {
        char *device_num,*command;

        client_message[read_size] = '\0';
        printf("message: %s\n",client_message);

        device_num = strtok (client_message,":"); // 100,1,2,3,4,0(net status)
        command = strtok (NULL,":"); //start,stop
  
	if (command == NULL){
            continue;
        }
        switch(atoi(device_num))
        {
		case 0:
		if (atoi(command) == 1){
			printf("application disconnected\n");
			digitalWrite(connect_ind, LOW);
                    	digitalWrite(device_1, LOW);
                    	digitalWrite(device_2, LOW);
                    	digitalWrite(device_3, LOW);
                    	digitalWrite(device_4, LOW);
		} else {
		    printf("number of active connection in the server %d\n",atoi(command));
		} break;
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

    
        printf("session terminated\n");
        digitalWrite(connect_ind, LOW);
        digitalWrite(device_1, LOW);
        digitalWrite(device_2, LOW);
        digitalWrite(device_3, LOW);
        digitalWrite(device_4, LOW);
        printf("all gpio's set to zero\n");
    

    printf("Closing session\n");
    close(socket_desc);
}



