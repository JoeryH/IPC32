#include <stdio.h>
#include <ctype.h>      // for isupper() etc.
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()
#include <string.h>

#include "Auxiliary.h"
#include "HandleTCPClient.h"

#define RCVBUFSIZE 100   /* Size of receive buffer */

void HandleTCPClient (int clntSocket)
{
    // 'clntSocket' is obtained from AcceptTCPConnection()

    char echoBuffer[RCVBUFSIZE];        /* Buffer for echo string */
    int  recvMsgSize;                   /* Size of received message */

    /* Receive message from client */
    recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
    if (recvMsgSize < 0)
    {
        DieWithError ("recv() failed");
    }
    info_d ("Recv", recvMsgSize);
    printf("Recieved: ");
    int i;
    for (i = 0; i < recvMsgSize; i++) {
        printf("%c", echoBuffer[i]);
    }
    printf("\n");
    
    if (strcmp(echoBuffer, "quit") == 0) {
        close(clntSocket);
        info ("close");
        return;
    }

    /* Send received string and receive again until end of transmission */
    while (recvMsgSize > 0)      /* zero indicates end of transmission */
    {
        printf("Text to send: ");
        gets(echoBuffer);
        
        delaying ();
        
        if (send (clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
        {
            DieWithError ("send() failed");
        }

        // receive next string
        recvMsgSize = recv (clntSocket, echoBuffer, RCVBUFSIZE-1, 0);
        if (recvMsgSize < 0)
        {
            DieWithError ("recv() failed");
        }
        info_d ("recv", recvMsgSize);
        printf("Recieved: ");
        for (i = 0; i < recvMsgSize; i++) {
            printf("%c", echoBuffer[i]);
        }
        printf("\n");
        if (strcmp(echoBuffer, "quit") == 0) {
            close(clntSocket);
            info ("close");
            return;
        }
    }

    close (clntSocket);    /* Close client socket */
    info ("close");
}
