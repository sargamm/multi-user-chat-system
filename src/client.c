#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <errno.h>

void * network( void * SocketID){
    char message[100];
    int t;
    int Socket=*((int*) SocketID);
    //printf("> ");
    while (1)
    {
        fgets(message,100,stdin);
        if(send(Socket,message,strlen(message),0)==-1){
            perror("send");
            exit(1);
        }
        //printf("> ");
    }
}
int main(){
    char message[100]="";
    int t;
    int Socket=socket(AF_UNIX,SOCK_STREAM,0);
    struct sockaddr_un remote;
    if(Socket==-1){
        perror("socket");
        exit(1);
    }
    remote.sun_family=AF_UNIX;
    strcpy(remote.sun_path,"socketfile");
    int length=strlen(remote.sun_path)+sizeof(remote.sun_family);
    if(connect(Socket,(struct sockaddr *)&remote,length)==-1)
    {
        perror("connect");
        exit(1);
    }
    printf("connected\n");
    pthread_t thread;
    pthread_create(&thread,NULL,network,(void *) &Socket); 
    while((t=recv(Socket,message,100,0))>0){
            message[t]='\0';
            printf("%s\n",message);
        }
         perror("recv");
         printf("Server closed connection\n");
        exit(1);
        
}
