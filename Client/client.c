//
// Created by matan on 09/03/16.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


void initGpio()
{

}

void safeShutdown()
{

}

void mainCleint(socket)
{
    char buffer[256];
    int exitMesagge = 0;
    while (!exitMesagge)
    {
        bzero(buffer, 256);
        n = read(socket, buffer, 255);
        if (n < 0)
            error("ERROR reading from socket");
        printf("%s\n", buffer);



    }
    close(socket);
}
