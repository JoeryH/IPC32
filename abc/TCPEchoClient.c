#include <stdio.h>
#include <string.h>     // for strlen()
#include <stdlib.h>     // for exit()
#include <sys/socket.h> // for send() and recv()
#include <unistd.h>     // for sleep(), close()

#include "Auxiliary.h"
#include "CreateTCPClientSocket.h"

#define RCVBUFSIZE 32   /* Size of receive buffer */

int main (int argc, char *argv[])
{
    int         sock;                   /* Socket descriptor */
    char *      echoString;             /* String to send to echo server */
    char        echoBuffer[RCVBUFSIZE]; /* Buffer for received string */
    int         echoStringLen;          /* Length of string to echo */
    int         bytesRcvd;              /* Bytes read in single recv() */
    int         i;                      /* counter for data-arguments */

    parse_args (argc, argv);

    sock = CreateTCPClientSocket (argv_ip, argv_port);
        
    for (i = 0; i < argv_nrofdata; i++)
    {
        echoString = argv_data [i];
        echoStringLen = strlen (echoString);          /* Determine input length */

        delaying();
        
        
        send(sock, echoString, echoStringLen, 0);
        
        info_s("data", echoString);
        
        bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE, 0);
        
        int j;
        for (j = 0; j < bytesRcvd; j++) {
            printf("%c", echoBuffer[j]);
        }
        printf("\n");
    }

    delaying ();

    close (sock);
    info ("close & exit");
    exit (0);
}
