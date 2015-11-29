//
//  client.c
//  cpsc_3600_final_project
//
//  Created by Evan Dorn on 10/31/15.
//  Copyright (c) 2015 Evan Dorn. All rights reserved.
//
// ===============Server Connection Info=====================
//    HOST: 6000
//    PORT: 127.0.0.1 (local host)
// ==========================================================
//
//


// TODOS - Set up basic client-server connection for message passing

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "client.h"

char username[BUFFER] = "[DEFAULT-USER]";
char message[BUFFER];


/***********************************************
 *            Helper Functions                 *
 ***********************************************/

// erase trailing newline
void clear_newline(char *str) {
    char *ptr = strchr(str, '\n');
    if(ptr) {
        *ptr = '\0';
    }
}

// return messages from stderr and exit the program
void error_handler(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

/***********************************************
 *            Core Functions                   *
 ***********************************************/

void *send_message(void *arg) {
    // stubbed

    return NULL;
}


void *recieve_message(void *arg) {
    // stubbed
    
    return NULL;
}


/***********************************************
 *                  Main                       *
 ***********************************************/
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
