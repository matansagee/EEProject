//
// Created by matan on 16/03/16.
//

#ifndef EEPROJECT_CONNECTION_H
#define EEPROJECT_CONNECTION_H



#define MAX_CHARACTERS_IN_STRING 256

int connect_to_client();
int disconnect();
void sendMessage(char* message);
char* recvMessage();
//GSocketClient* gconnect();

#endif //EEPROJECT_CONNECTION_H