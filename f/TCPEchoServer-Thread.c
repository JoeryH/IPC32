#include <pthread.h>
#include <stdio.h>
#include <unistd.h> 
#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"

static void * myThread (void * arg);            /* thread that does the work */

int main (int argc, char *argv[])
{
    int         servSock;     /* Socket descriptor for server */
    int         clntSock;     /* Socket descriptor for client */
    pthread_t   threadID;     /* Thread ID from pthread_create() */
    bool        to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (to_quit == false)                /* run until someone indicates to quit... */
    {
        clntSock = AcceptTCPConnection (servSock);
        
        if (pthread_create (&threadID, NULL, myThread, (void *) clntSock) != 0) {
            perror ("pthread_create");
        }

    }
    close(servSock);
    
    // server stops...
    return (0);
}

static void *
myThread (void * threadArgs)
{
    HandleTCPClient((int) threadArgs);
    
    pthread_detach(pthread_self());
    
    close((int) threadArgs);
    
    return (NULL);
}
