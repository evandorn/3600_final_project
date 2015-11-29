//
//  server.h
//  cpsc_3600_final_project
//
//  Created by Evan Dorn on 11/24/15.
//  Copyright © 2015 Evan Dorn. All rights reserved.
//

#ifndef server_h
#define server_h

#define BUFFERSIZE 1024  
#define MAX_CONNECTIONS 256             // Arbitrary number of maximum client connections
#define SERVER_PORT "6000"

// Function Prototypes
void clear_newline(char *str);
void *send_message(void *arg);           // send thread main
void error_handler(char *message);       // simple error handling function
void *client_handler(void *arg);         // client handling function


#endif /* server_h */
