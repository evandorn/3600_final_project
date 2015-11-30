//
//  server.c
//  cpsc_3600_final_project
//
//  Created by Evan Dorn on 10/31/15.
//  Copyright (c) 2015 Evan Dorn. All rights reserved.
//

// TODOS - Debug and got through MARK on line 85

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "server.h"

pthread_mutex_t mutex;

int client_count = 0;                                                        // Number of connected clients
int client_connections[MAX_CONNECTIONS];                                     // connected client's socket conncetions

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
 *  Send a message to *all* of the connected clients
 */
void *send_message(char *message, int message_length) {
    
    // Handle concurrency for writing to multiple clients via mutex locks
    pthread_mutex_lock(&mutex);
    for(int count = 0; count < client_count; count++) {
        write(client_connections[count], message, message_length);
    }
    pthread_mutex_lock(&mutex);
    return NULL;
}

/*
 *  Handle client actions
 */
void *client_handler(void *arg) {
    
    packet sent_packet =* ((packet *)arg);
    
    int client_socket = sent_packet.client_socket;
    int ret_val = 0;
    int count;
    char message[BUFFER];
    char ip_address[BUFFER];
    
    sprintf(ip_address, "%s", sent_packet.ip_address);
    
    while((ret_val = read(client_socket, message, sizeof(message))) != 0) {
        send_message(message, ret_val);
    }

    pthread_mutex_lock(&mutex);
    // Handle a disconnecting user
    // MARK -- Potentially buggy code. Wrote this at 3am
    for(count = 0; count < client_count; count++) {
        if(client_count == client_connections[count]) {
            while(count++ < client_count - 1) {
                client_connections[count] = client_connections[count + 1];
            }
            break;
        }
    }
    
    client_count--;
    printf("%s is now disconnected\n", ip_address);
    printf("%d users are now currently active.\n", client_count);
    pthread_mutex_unlock(&mutex);
    close(client_socket);
    
    return NULL;
}

/***********************************************
 *                  Main                       *
 ***********************************************/
int main(int argc, const char *argv[]) {

    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int client_address_size;
    pthread_t thread_id;
    packet sent_packet;
    
    if(argc != 2) {
        fprintf(stderr, "Error! Usage: %s <Port>\n", argv[0]);
        exit(1);
    }
    
    pthread_mutex_init(&mutex, NULL);
    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));
    
    if(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handler("Error: bind() failure. Program exiting.\n");
    }
    if(listen(server_socket, 5) == -1) {
        error_handler("Error: listen() failure. Program exiting.\n");
    }
    
    while(1) {
        client_address_size = sizeof(client_address);
        
    }

    
    return 0;
}
