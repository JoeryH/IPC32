#include <stdlib.h>     // for atoi() and exit()
#include <unistd.h>     // for fork()
#include <sys/wait.h>   // for waitpid()
#include <stdio.h>

#include "Auxiliary.h"
#include "AcceptTCPConnection.h"
#include "CreateTCPServerSocket.h"
#include "HandleTCPClient.h"

int main(int argc, char *argv[])
{
    int     servSock;                  /* Socket descriptor for server */
    int     clntSock;                  /* Socket descriptor for client */
    pid_t   processID;                 /* Process ID from fork() */
    bool    to_quit = false;

    parse_args (argc, argv);

    servSock = CreateTCPServerSocket (argv_port);

    while (to_quit == false)                /* run until someone indicates to quit... */
    {
        clntSock = AcceptTCPConnection (servSock);
        
        processID = fork();
        if (processID < 0) {
            // fatal error!
            perror("fork() failed");
            exit (1);
        } else if (processID == 0) { // child process handles client
            info("started child process");
            HandleTCPClient(clntSock);
            close(clntSock);
            info("closing child process");
            exit(0);
        } // else parent process continues

    }
    close(servSock);
    // server stops...
	return (0);
}
