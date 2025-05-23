#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>

#define MAXBUF  1024

void error_exit(char *msg)
{
 printf("error %s \n",msg);
 exit(-1);
}

int main(int argc, char *argv[])
{   int sockfd, bytes_read;
    struct sockaddr_in dest;
    char buffer[MAXBUF];

    /*---Make sure we have the right number of parameters---*/
    if ( argc != 3 )
        error_exit ("usage: http-client <IP-addr> <Port>\n");
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
        error_exit ("Socket");

    /*---Initialize server address/port struct---*/
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(argv[2])); /* HTTP Server port */
    if ( inet_addr(argv[1], &dest.sin_addr.s_addr) == 0 )
        error_exit (argv[1]);

    /*---Connect to server---*/
    if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
        error_exit ("Connect");

    sprintf(buffer, "GET index.html HTTP/1.0\n\n");
    send(sockfd, buffer, strlen(buffer), 0);

    /*---While there's data, read and print it---*/
    do
    {
        bzero(buffer, sizeof(buffer));
        bytes_read = recv(sockfd, buffer, sizeof(buffer), 0);
        if ( bytes_read > 0 )
            printf("%s", buffer);
    }
    while ( bytes_read > 0 );

    /*---Clean up---*/
    close(sockfd);
    return 0;
}
