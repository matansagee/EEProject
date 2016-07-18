//
// Created by matan on 16/03/16.
//

#ifndef EEPROJECT_CONNECTION_H
#define EEPROJECT_CONNECTION_H



#define MAX_CHARACTERS_IN_STRING 256
void init_socket();
int connect_to_client();
void disconnect();
int sendMessage(char* message);
int connection_status();
char* recvMessage();

#endif //EEPROJECT_CONNECTION_H
