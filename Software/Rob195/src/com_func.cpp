#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>

#include "com_func.h"
using namespace std;

float *connectReadJoint (float *current_joint_pos){
    // READ SETUP

    int sockfd;
    struct sockaddr_in serv_addr_read;
    bzero((char *) &serv_addr_read, sizeof(serv_addr_read));
    serv_addr_read.sin_family = AF_INET;
    serv_addr_read.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr_read.sin_port = htons(SERV_TCP_PORT_READ);

    // READ JOINT POS

    if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        printf("Client: Can't Open Stream Socket\n");
    }
    printf("Client: Connecting to READ server...\n");
    if(connect(sockfd,(struct sockaddr *) &serv_addr_read, sizeof(serv_addr_read))<0){
        printf("Client: Can't Connect to the READ server\n");
    }
    else{
        printf("Client: Connected!\n");
        current_joint_pos = readPos(sockfd);
        for(int k = 0; k < 6; k++){
            printf("joint %d pos: %f \n", k+1, current_joint_pos[k]);
        }
    }
    return current_joint_pos;
}

float *connectReadCartesian (float *current_cartesian_pos){
    //    auto startReadConnect = chrono::steady_clock::now();
    // READ SETUP
    int sockfd;
    struct sockaddr_in serv_addr_read;
    bzero((char *) &serv_addr_read, sizeof(serv_addr_read));
    serv_addr_read.sin_family = AF_INET;
    serv_addr_read.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr_read.sin_port = htons(SERV_TCP_PORT_READ);

    // READ JOINT POS

    if((sockfd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        printf("Client: Can't Open Stream Socket\n");
    }
    printf("Client: Connecting to READ server...\n");
    while(connect(sockfd,(struct sockaddr *) &serv_addr_read, sizeof(serv_addr_read))<0){
        printf("Client: Can't Connect to the READ server\r");
    }

    printf("Client: Connected!\n");
    current_cartesian_pos = readPos(sockfd);
    printf("X pos: %f \n", current_cartesian_pos[0]);
    printf("Y pos: %f \n", current_cartesian_pos[1]);
    printf("Z pos: %f \n", current_cartesian_pos[2]);
    printf("W pos: %f \n", current_cartesian_pos[3]);
    printf("P pos: %f \n", current_cartesian_pos[4]);
    printf("R pos: %f \n", current_cartesian_pos[5]);


    //    auto endReadConnect = chrono::steady_clock::now();
    //    cout << "Elapsed Time in milliseconds (connect read): " << chrono::duration_cast<chrono::milliseconds>(endReadConnect - startReadConnect).count() << "ms" << endl;
    return current_cartesian_pos;
}

void connectWriteJoint (float *goto_joint_pos, float *current_joint_pos){
    //WRITE SETUP
    int sockfd_write;
    struct sockaddr_in serv_addr_write;
    bzero((char *) &serv_addr_write, sizeof(serv_addr_write));
    serv_addr_write.sin_family = AF_INET;
    serv_addr_write.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr_write.sin_port = htons(SERV_TCP_PORT_WRITE);

    // WRITE JOINT POS
    if((sockfd_write = socket(AF_INET, SOCK_STREAM,0)) < 0){
        printf("Client: Can't Open Stream Socket\n");
    }
    printf("Client: Connecting to WRITE server...\n");
    if(connect(sockfd_write,(struct sockaddr *) &serv_addr_write, sizeof(serv_addr_write))<0){
        printf("Client: Can't Connect to the WRITE server\n");
    }
    else{
        for(int s = 0; s < 6; s++){
            goto_joint_pos[s] = current_joint_pos[s] + 10.0f;
            printf("future joint %d pos: %f \n", s+1, goto_joint_pos[s]);
        }
        writePos(sockfd_write, goto_joint_pos);
    }
}

void connectWriteCartesian (float *goto_cartesian_pos, float *current_cartesian_pos){
    //    auto startWriteConnect = chrono::steady_clock::now();
    //WRITE SETUP
    int sockfd_write;
    struct sockaddr_in serv_addr_write;
    bzero((char *) &serv_addr_write, sizeof(serv_addr_write));
    serv_addr_write.sin_family = AF_INET;
    serv_addr_write.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    serv_addr_write.sin_port = htons(SERV_TCP_PORT_WRITE);

    // WRITE JOINT POS
    if((sockfd_write = socket(AF_INET, SOCK_STREAM,0)) < 0){
        printf("Client: Can't Open Stream Socket\n");
    }
    printf("Client: Connecting to WRITE server...\n");
    while(connect(sockfd_write,(struct sockaddr *) &serv_addr_write, sizeof(serv_addr_write))<0){
        printf("Client: Can't Connect to the WRITE server\r");
    }
    printf("Client: Connected!\n");

    printf("future X pos: %f \n", goto_cartesian_pos[0]);
    printf("future Y pos: %f \n", goto_cartesian_pos[1]);
    printf("future Z pos: %f \n", goto_cartesian_pos[2]);
    printf("future W pos: %f \n", goto_cartesian_pos[3]);
    printf("future P pos: %f \n", goto_cartesian_pos[4]);
    printf("future R pos: %f \n", goto_cartesian_pos[5]);
    writePos(sockfd_write, goto_cartesian_pos);
    //    auto endWriteConnect = chrono::steady_clock::now();
    //    cout << "Elapsed Time in milliseconds (connect write): " << chrono::duration_cast<chrono::milliseconds>(endWriteConnect - startWriteConnect).count() << "ms" << endl;

}


float * readPos (int sockfd)
{
    //    printf("Reading current joint positions...\n");
    char input[MAXLINE+1];
    static float input_float[6];
    char recvline[MAXLINE + 1];
    for(int x=0; x < 6; x++)
    {
        //memset (input_float, 0, 6);
        memset (input, 0, MAXLINE+1);
        input_float[x] = readline(sockfd, recvline, 126, input);
    }
    return input_float;
}

void writePos (int sockfd, float *goto_pos)
{
    char sendline[MAXLINE];
    for(int y=0; y < 6; y++)
    {
        sprintf(sendline, "%f", goto_pos[y]);
        //        printf("sendline %u : %s\n", y, sendline);

        if(written(sockfd, sendline, 126)!=126){
            printf("writeJointPos:written error on sock\n");
        }
        //        sleep(1);
    }
}

float readline(int fd, char *ptr, int maxlen, char input[MAXLINE+1])
{
    int n, rc;
    char c;
    for(n = 0; n < maxlen; n++){
        if((rc = read(fd, &c, 1)) == 1){
            input[n] = c;
            *ptr++ = c;
            if(c=='\n'){
                break;
            }
            else if(rc== 0) {
                if(n== 0) {
                    return (0);
                }
                else{

                    break;
                }
            }
        }
        else{
            return (-1);
        }
    }
    float f = atof(input);
    *ptr = 0;
    return (f);
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
        //        printf("Client: write command executed!\n");
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


void gripperOn (void){
    system("lua5.3 ../Rob195/1.txt");
}

void gripperOff (void){
    system("lua5.3 ../Rob195/0.txt");
}