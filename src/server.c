//
//  server.c
//  cpsc_3600_final_project
//
//  Created by Evan Dorn on 10/31/15.
//  Copyright (c) 2015 Evan Dorn. All rights reserved.
//
// ===============Server Address Info========================
//    HOST:
//    PORT:
// ==========================================================


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


/***********************************************
 *                  Main                       *
 ***********************************************/
int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
