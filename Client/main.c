//
// Created by shani on 08/06/16.
//
#include "client.h"
#include "connection.h"

int main()
{
    int socket;
    initGpio();
    socket = connectToServer();
    mainCleint(socket);
    safeShutdown();

    printf("Im client\n");
}

