//
//  server.c
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
#include <netinet/in.h>
#include <pthread.h>

#include "server.h"

int client_count = 0;
int client_sockets[MAX_CLIENTS];
int name_size = 0;

pthread_mutex_t mutex;

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
void send_message(char *message, int data_len) {
    int i;
    pthread_mutex_lock(&mutex);
    for(i=0; i < client_count; i++) {
        write(client_sockets[i], message, data_len);
    }
    pthread_mutex_unlock(&mutex);
}

/*
 *  Handle client actions
 */
void *client_handler(void *arg) {
    packet sent_packet =* ((packet*)arg);
    int client_socket=sent_packet.client_socket;
    int data_len = 0, i;
    char message[BUFFER];
    char connected_ip[BUFFER];
    
    sprintf(connected_ip, "%s", sent_packet.ip_address);
    
    while((data_len = read(client_socket, message, sizeof(message))) != 0)
        send_message(message, data_len);
    
    pthread_mutex_lock(&mutex);
    // Boot a disconnecting user
    for(i=0; i < client_count; i++) {
        if(client_socket == client_sockets[i]) {
            while(i++ < client_count-1)
                client_sockets[i]=client_sockets[i+1];
            break;
        }
    }
    
    client_count--;
    fprintf(stdout, "%s is now disconnected\n", connected_ip);
    fprintf(stdout, "%d users are now currently active.\n", client_count);
    
    pthread_mutex_unlock(&mutex);
    close(client_socket);
    return NULL;
}

int main(int argc, char *argv[]) {
    
    int server_socket, client_socket;
    struct sockaddr_in serv_adr, client_address;
    unsigned int client_address_size;
    pthread_t thread_id;
    packet sent_packet;
    
    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    pthread_mutex_init(&mutex, NULL);
    server_socket=socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));
    
    if(bind(server_socket, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1) {
        error_handler("bind() error");
    }
    
    if(listen(server_socket, 5)==-1) {
        error_handler("listen() error");
    }
    
    while(1) {
        client_address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
        
        pthread_mutex_lock(&mutex);
        client_sockets[client_count++] = client_socket;
        pthread_mutex_unlock(&mutex);
        
        sent_packet.client_socket = client_socket;
        sprintf(sent_packet.ip_address, "%s", inet_ntoa(client_address.sin_addr));
        pthread_create(&thread_id, NULL, client_handler, (void*)&sent_packet);
        pthread_detach(thread_id);
        printf("Connected client IP: %s \n", inet_ntoa(client_address.sin_addr));
        printf("Now Connecting : %d persons\n", client_count);
        
    }
    close(server_socket);
    return 0;
}
