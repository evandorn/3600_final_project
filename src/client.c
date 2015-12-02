//
//  client.c
//  3600-Final-Progject
//
//  Created by Evan Dorn on 12/2/15.
//  Copyright © 2015 evandorn. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "client.h"

char username[USER] = "[DEFAULT]";
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

/*
 *  Send a message via the main thread
 */
void *send_message(void * arg) {
    
    int my_socket =* ((int*)arg);
    char recieved_message[USER + BUFFER];
    sprintf(recieved_message, "%s has joined the chat.\n", username);
    write(my_socket, recieved_message, strlen(recieved_message)+2);
    while(1) {
        fgets(message, BUFFER, stdin);
        if(!strcmp(message, "q\n")||!strcmp(message, "Q\n")||!strcmp(message, "Quit\n")||!strcmp(message, "quit\n")) {
            sprintf(recieved_message, "%s has left the chat.\n", username);
            write(my_socket, recieved_message, strlen(recieved_message)+2);
            close(my_socket);
            exit(0);
        }
        sprintf(recieved_message, "%s %s", username, message);
        write(my_socket, recieved_message, strlen(recieved_message));
    }
    return NULL;
}

/*
 * Read the message from the main thread
 */
void *recieve_message(void *arg) {
    int my_socket =* ((int*)arg);
    char recieved_message[USER+BUFFER];
    int data_len;
    while(1) {
        data_len = read(my_socket, recieved_message, USER+BUFFER-1);
        if(data_len == -1) {
            return (void*)-1;
        }
        recieved_message[data_len] = 0;
        fputs(recieved_message, stdout);
    }
    return NULL;
}

/***********************************************
 *                  Main                       *
 ***********************************************/
int main(int argc, char *argv[]) {
    
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
    
    if(connect(my_socket, (struct sockaddr*)&server_address, sizeof(server_address))==-1) {
        error_handler("Error! - connect() failure. Program exiting.");
    }
    
    pthread_create(&sending_thread, NULL, send_message, (void*)&my_socket);
    pthread_create(&recieving_thread, NULL, recieve_message, (void*)&my_socket);
    pthread_join(sending_thread, &return_thread);
    pthread_join(recieving_thread, &return_thread);
    close(my_socket);
    return 0;
}
