//
//  client.h
//  3600-Final-Progject
//
//  Created by Evan Dorn on 12/2/15.
//  Copyright © 2015 evandorn. All rights reserved.
//

#ifndef client_h
#define client_h

#define BUFFER 100
#define USER 20

// function prototypes
void clear_newline(char *str);
void *recieve_message(void *arg);        // read from the main thread
void *send_message(void *arg);           // send thread main
void error_handler(char *message);       // simple error handling function

#endif /* client_h */
