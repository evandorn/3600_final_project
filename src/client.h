//
//  client.h
//  cpsc_3600_final_project
//
//  Created by Evan Dorn on 11/24/15.
//  Copyright © 2015 Evan Dorn. All rights reserved.
//

#ifndef client_h
#define client_h

#define BUFFER 1024

#define SERVER_PORT "6000"
#define CLIENT_PORT "6001"

// function prototypes
void clear_newline(char *str);
void *recieve_message(void *arg);        // read from the main thread
void *send_message(void *arg);           // send thread main
void error_handler(char *message);       // simple error handling function

#endif /* client_h */
