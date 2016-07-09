/*
    C socket server example, handles multiple clients using threads
    Compile
    gcc server.c -lpthread -o server
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread

#define MAX_NUMBER_OF_ACTIVE_LINKS 2

int volatile *clients_sockets;
int volatile number_of_active_links = 0;
//TODO: fixing the issue with the thread not being dead in the end
//the thread function
void *connection_handler(void *);

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
    int i;
    clients_sockets = (int) malloc(MAX_NUMBER_OF_ACTIVE_LINKS * sizeof(int));
    if (clients_sockets==NULL)
    {
        printf("allocating memory failed.\n");
        return 1;
    }
    for (i = 0; i< MAX_NUMBER_OF_ACTIVE_LINKS;i++){
        clients_sockets[i]=0;
    }
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);


    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)))
    {

        puts("Connection accepted");

        if(number_of_active_links >= MAX_NUMBER_OF_ACTIVE_LINKS)
        {
            char* message = "number of connection are in maximum state, plest try later\n";
            write(client_sock , message , strlen(message));
            close(client_sock);
            continue;
        }

        clients_sockets[number_of_active_links++] = client_sock;
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        pthread_detach(thread_id);
        puts("Handler assigned");
    }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    close(socket_desc);
    return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;

    int read_size;
    char *message , client_message[2000];
    int id = number_of_active_links == 1 ? 0 : 1;

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock , message , strlen(message));

    message = "Now type something and i shall send what you type \n";
    write(sock , message , strlen(message));

    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
        client_message[read_size] = '\0';

        //Send the message back to client
        if (number_of_active_links != 0)
        {
            write(clients_sockets[1-id] , client_message , strlen(client_message));
        }

        //clear the message buffer
        memset(client_message, 0, 2000);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    number_of_active_links--;
    close(sock);
    return 0;
}