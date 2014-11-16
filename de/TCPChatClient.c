#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"

#define RCVBUFSIZE 100   /* Size of receive buffer */

int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    char        echoBuffer[RCVBUFSIZE]; /* Buffer for received string */
    int         bytesRcvd;              /* Bytes read in single recv() */
    int         i;                      /* counter for data-arguments */

    parse_args (argc, argv);

    sock = CreateTCPClientSocket (argv_ip, argv_port);
    
    do 
        {
            printf("Text to send: ");
            gets(echoBuffer);
            
            delaying ();
            
            if (send (sock, echoBuffer, strlen(echoBuffer), 0) != strlen(echoBuffer))
            {
                DieWithError ("send() failed");
            }

            // receive next string
            bytesRcvd = recv (sock, echoBuffer, RCVBUFSIZE-1, 0);
            if (bytesRcvd < 0)
            {
                DieWithError ("recv() failed");
            }
            info_d ("recv", bytesRcvd);
            printf("Recieved: ");
            for (i = 0; i < bytesRcvd; i++) {
                printf("%c", echoBuffer[i]);
            }
            printf("\n");
            if (strcmp(echoBuffer, "quit") == 0) {
                close(sock);
                info ("close & exit");
                exit(0);
            }
        } while (bytesRcvd > 0);

    delaying ();

    close (sock);
    info ("close & exit");
    exit (0);
}
