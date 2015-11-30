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

/*
 *  Send a message via the main thread
 */
void *send_message(void *arg) {
    
    int my_socket = *((int *)arg);
    char recieved_message[BUFFER];

    // User joining the chat
    sprintf(recieved_message, "%s has joined the chat.\n", username);
    write(my_socket, recieved_message, strlen(recieved_message) + 2);
    
    while(1) {
        fgets(message, BUFFER, stdin);
        // User quitting the chat
        // TODO - Test input for Quit
        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n") || !strcmp(message, "Quit\n") || !strcmp(message, "quit\n")) {
            sprintf(recieved_message, "%s has left the chat.\n", username);
            write(my_socket, recieved_message, strlen(recieved_message) + 2);
            close(my_socket);
            exit(0);
        }
        // User input and printing the console
        sprintf(recieved_message, "%s %s", username, message);
        write(my_socket, recieved_message, strlen(recieved_message));
    }
    return NULL;
}

/*
 * Read the message from the main thread
 */
void *recieve_message(void *arg) {
    
    int my_socket = *((int *)arg);
    char recieved_message[BUFFER];
    int str_count;
    
    while(1) {
    str_count = read(my_socket, recieved_message, BUFFER - 1);
    
    if(str_count == -1) {
        return (void *) - 1;
    }
    recieved_message[str_count] = 0;
    fputs(recieved_message, stdout);
    }
    
    return NULL;
}

/***********************************************
 *                  Main                       *
 ***********************************************/
int main(int argc, const char *argv[]) {
    
    int my_socket;
    struct sockaddr_in server_address;
    pthread_t sending_thread, recieving_thread;
    void *return_thread;
    
    if(argc != 4) {
        fprintf(stderr, "Input error! Usage: %s <IP Address> <Port> <Username>\n", argv[0]);
        exit(1);
    }
    
    sprintf(username, "[%s]", argv[3]);
    my_socket = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));
    
    // MARK - Potential warning here.
    if(connect(my_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        error_handler("Error! - connect() failure. Program exiting.");
    }
    
    // create the sending and recieving threads
    pthread_create(&sending_thread, NULL, send_message, (void *)&my_socket);
    pthread_create(&recieving_thread, NULL, recieve_message, (void *)&my_socket);
    pthread_join(sending_thread, &return_thread);
    pthread_join(recieving_thread, &return_thread);
    close(my_socket);
    
    return 0;
}
