/* BSD Standard Socket Programming Example - UNIX */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#define SERV_TCP_PORT 59002
#define SERV_HOST_ADDR "147.87.149.40" /* RoboGuide PC Robot */
#define MAXLINE 512
using namespace std;
int written(int fd, char *ptr, int nbytes);
int readline(int fd, char *ptr, int maxlen, char *input);
void str_cli(int sockfd);
char *pname;

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in serv_addr;
    pname = argv[0];
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr.sin_port = htons(SERV_TCP_PORT);
    if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        printf("Client: Can't Open Stream Socket\n");
    }
    printf("Client: Connecting...\n");
    if(connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
        printf("Client: Can't Connect to the server\n");
    }
    else{
        printf("Client: Connected!\n");
        str_cli(sockfd);
    }
    printf("program finished, connection ended.");
    exit(0);
}
void str_cli (int sockfd)
{
    int n, i;
    char input[6][11];
    char sendline[MAXLINE], recvline[MAXLINE + 1];
    while(1)
    {
        memset (sendline, 42, 128); /*was 2 inst of 13*/
        /*
        for(i = 0; i < 128; i++){
            printf("Client: Sendline[%d] = %c \n", i, sendline[i]);
        }
        */
        if(written(sockfd, sendline, 126)!=126){
//            printf("strcli:written error on sock\n");
        }
        i = readline(sockfd, recvline, 126, input);
    }
}
int readline(int fd, char *ptr, int maxlen, char *input)
{
    int n, rc;
    char c;
    int i = 0;
//    printf("Client: readLine() started \n");
    for(n = 0; n < maxlen; n++){
        if((rc = read(fd, &c, 1)) == 1){
            printf("read %d \n", c);
            input[n][i] = c;
            printf("array %c \n", input[n][i]);
            i++;
            *ptr++ = c;
            if(c=='\n'){
                printf("Client: newline \n");
                break;
            }
            else if(rc== 0) {
//                printf("Client: else if (n == 0) \n");
                if(n== 0) {
                    return (0);
                }
                else{

                    break;
                }
            }
        }
        else{
//            printf("Client: return -1 \n");
            return (-1);
        }
    }
    *ptr = 0;
//    printf("Client: return(n) \n");
    return (n);
}
int written(int fd, char *ptr, int nbytes)
{
    int nleft, nwritten;
    nleft = nbytes;
//    printf("Client: nleft size: %d \n", nleft);
    while(nleft > 0) {
//        printf("Client: while write started\n");
        nwritten = write(fd, ptr, nleft); /*was nleft*/
//        printf("Client: nwritten size: %d \n", nwritten);
        printf("Client: write command executed!\n");
        if(nwritten <= 0) {
//            printf("Client: return nwritten\n");
            return(nwritten);
        }
//        printf("Client: nleft and ptr\n");
        nleft -= nwritten;
//        printf("Client: nleft size: %d \n", nleft);
        ptr += nwritten;
    }
//    printf("Client: return nbytes - nleft: %d \n", nbytes-nleft);
    return(nbytes - nleft);
}
