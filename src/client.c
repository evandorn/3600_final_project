//
//  client.c
//  cpsc_3600_final_project
//
//  Created by Evan Dorn on 10/31/15.
//  Copyright (c) 2015 Evan Dorn. All rights reserved.
//
// ===============Server Connection Info=====================
//    HOST:
//    PORT:
// ==========================================================
//
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <netdb.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>

#define BUFFERSIZE 256                             // basic 256 CHAR buffer

int STOP = 0;                                      // Global stop flag


/***********************************************
 *            Helper Functions                 *
 ***********************************************/

// helper function to get input from the user
char *getCommand() {
    char *cmd = NULL;
    size_t len = 256;
    
    if (getline(&cmd, &len, stdin) > 0) {
        /* strip off the newline */
        cmd[strlen(cmd)-1] = '\0';
    }
    return cmd;
}

/* this routine should run as a pthread */
void *recv_handler(void *psock) {
    
    int my_socket = *((int *)psock);
    int retval = 0;
    fd_set sockets_ready;                   // are the sockets ready to be read?
    struct timeval timevalue;               // timer value
    
    timevalue.tv_sec = 0;                     // 0 seconds
    timevalue.tv_usec = 500;                  // 500 miliseconds = .5 seconds
    
    char message_buffer[BUFFERSIZE];        // buffer to save the message
    char bracket_buffer[BUFFERSIZE];        // buffer to hold the incomming user status and name
    char user_buffer[BUFFERSIZE];
    char message[BUFFERSIZE];
    
    do {
        /*
         * recv() messages from the socket, until the user stops.
         *
         * If you are blocked in a recv() call, you will be unable to
         * terminate its execution.  Use multiplexing techniques to
         * check for reads before receiving data.
         *
         */
        FD_ZERO(&sockets_ready);                                              // zero out file discriptors
        FD_SET(my_socket, &sockets_ready);                                    // set where to save file discriptors
        retval = select(my_socket+1, &sockets_ready, NULL, NULL, &timevalue);
        if(FD_ISSET(my_socket, &sockets_ready) && retval > 0) {               // If FD is set and there is a retval == 1
            memset(&message_buffer, 0, sizeof(message_buffer));               // clear out message buffer
            recv(my_socket, message_buffer, BUFFERSIZE-1, 0);
            sscanf(message_buffer, "%s %s", bracket_buffer, user_buffer);
            
            strcpy(message, message_buffer + strlen(bracket_buffer) + strlen(user_buffer) + 1);
            if((strcmp(bracket_buffer, "private") == 0) || (strcmp(bracket_buffer, "public") == 0)) {
                printf("[%s] [%s] %s\n", bracket_buffer, user_buffer, message);
            } else {
                printf("%s\n", message_buffer);                                   // print out the result of the recv() call
            }
        }
    } while (!STOP);
    pthread_exit(NULL);
}


/***********************************************
 *                  Main                       *
 ***********************************************/
int main(int argc, char *argv[]) {

    int socket_descriptor = -1;
    char *host;
    char *port;
    pthread_t thread;
    char buffer[BUFFERSIZE];
    char quit_command[BUFFERSIZE];

    
    // initial error checking
    if(argc != 3) {
        fprintf(stderr, "Input Error! Usage:  ./client <host> <port>\n");
        exit(1);
    }
    
    host = argv[1];
    port = argv[2];
    
    // Create the TCP client socket
    // Fill out the serv_addr struct to connect to the server
    struct addrinfo addr;                                           // address struct
    memset(&addr, 0, sizeof(addr));                                 // zero out struct
    addr.ai_family = AF_UNSPEC;                                     // Use IPv4 o IPv6 doesn't matter
    addr.ai_socktype = SOCK_STREAM;                                 // Streaming sockets
    addr.ai_protocol = IPPROTO_TCP;                                 // Use only TCP

    // Get the required addresses
    struct addrinfo *server_address;
    int retval = getaddrinfo(host, port, &addr, &server_address);
    
    // getaddrinfo should return 0
    if(retval != 0) {
        fprintf(stderr, "getaddrinfo() function call failed: %s\n", gai_strerror(retval));
    }
    
    // Taking the next address and assigning it to the first address (handles multiple address)
    for(struct addrinfo *my_addr = server_address; my_addr != NULL; my_addr = my_addr->ai_next) {
        
        // create the socket
        socket_descriptor = socket(my_addr->ai_family, my_addr->ai_socktype, my_addr->ai_protocol);
        
        if(socket_descriptor < 0) {
            continue;
        }
        
        // Connect to server
        if(connect(socket_descriptor, my_addr->ai_addr, my_addr->ai_addrlen) == 0) {
            break;
        }
        close(socket_descriptor);
    }
    freeaddrinfo(server_address);

    // Create a thread to handle the recv channel
    // Main thread handles send() calls
    if(pthread_create(&thread, NULL, recv_handler, &socket_descriptor)) {
        fprintf(stderr, "error creating thread");
        exit(-1);
    }
    
    // do-while handles READY from TCP
    // Process user commands and send them to the server
    do {
        memset(&buffer, 0, sizeof(buffer));
        fgets(buffer, BUFFERSIZE, stdin);                     // Take input from stdin
        buffer[strlen(buffer)-1] = '\0';                // cut off newline character
        sscanf(buffer, "%s", quit_command);                   // store buffer in quit_command
        
        // send command to server
        // printf(">%s<\n", my_buffer);
        send(socket_descriptor, buffer, sizeof(buffer), 0);
        if(strcmp(quit_command, "quit") == 0) {
            STOP = 1;
        }
    } while (STOP != 1);
    
    // Cleanup when finished
    STOP = 1;
    close(socket_descriptor);
    pthread_join(thread, NULL);
    
    return 0;
}
