#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netinet/ip.h>
#include <pthread.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/un.h>

int NumberOfClients=0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct Client{
    int index;
    int sockID;
    struct sockaddr_un cliAddress;
    int length;
};

struct Client Cli[1024];
pthread_t thread[100];

void *Network( void *ClientDetail){
	struct Client clientDetail = *((struct Client *) ClientDetail);
	int clientSocket = clientDetail.sockID;
    int index=clientDetail.index;

	printf("Client %d connected.\n",clientDetail.index + 1);

	while(1){
        int z;
		char data[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';
        int id=atoi(data)-1;
        read = recv(clientSocket,data,1024,0);
		data[read] = '\0';
        printf("%s\n",data);
        if(id>=0)
        send(Cli[id].sockID,data,strlen(data),0);
        else{
            for(int i=0;i<NumberOfClients;i++)
            if(i!=index)
            send(Cli[i].sockID,data,strlen(data),0);
        }
    }
}

int main(){
    char message[100]="";
    int socketDescriptor=socket(AF_UNIX,SOCK_STREAM,0);
    if(socketDescriptor==-1){
        exit(0);
    }
    struct sockaddr_un local,remote;
    local.sun_family=AF_UNIX;
    strcpy(local.sun_path,"socketfile");
    unlink(local.sun_path);
    int length=strlen(local.sun_path)+sizeof(local.sun_family);
    int x=bind(socketDescriptor,(struct sockaddr *) &local, sizeof(local));
    if(x==-1){
        exit(0);
    }
    int y=listen(socketDescriptor,5);
    length=sizeof(struct sockaddr_un);
    if(y==-1){
        return 0;
    }
    while (1){
    int socket2=accept(socketDescriptor,(struct sockaddr *)&remote,&length);
    if(socket2==-1){
        perror("accept");
        exit(1);
    }

    Cli[NumberOfClients].sockID=socket2;
    Cli[NumberOfClients].index=NumberOfClients;
    printf("connected.\n");
    pthread_create(&thread[NumberOfClients],NULL,Network,(void *) &Cli[NumberOfClients]);
    NumberOfClients++;
    }
    for(int i=0;i<NumberOfClients;i++){
        pthread_join(thread[i],NULL);
    }
}
