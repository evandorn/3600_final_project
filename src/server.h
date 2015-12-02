//
//  server.h
//  3600-Final-Progject
//
//  Created by Evan Dorn on 12/2/15.
//  Copyright © 2015 evandorn. All rights reserved.
//

#ifndef server_h
#define server_h

#define BUFFER 100
#define MAX_CLIENTS 256

// Structs
struct data_packet {
    int client_socket;
    char ip_address[BUFFER];
};

typedef struct data_packet packet;

// function prototypes
void *client_handler(void *arg);
void send_message(char *message, int len);
void error_handler(char *message);

#endif /* server_h */
