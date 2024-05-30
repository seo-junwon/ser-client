#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define max_client 100
#define buffer_size 1024

int accept_sockets[max_client]={0};
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void message_send(char *message, int sender_sock){
    pthread_mutex_lock(&clients_mutex);

    for (int i=0;i<max_client;i++){
        if((accept_sockets[i]!=0) && (accept_sockets[i]!=sender_sock)){
            send(accept_sockets[i],message,strlen(message),0);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void *server_work(void *arg){
    int accept_sock = *((int*)arg);
    char buffer[buffer_size*2];
    memset(buffer, 0, buffer_size);
    int receive;
   
    while ((receive=recv(accept_sock,buffer,sizeof(buffer)-1,0))>0){
        buffer[receive]='\0';
        printf("%s",buffer);
        message_send(buffer,accept_sock);
    }


    close(accept_sock);

    pthread_mutex_lock(&clients_mutex);

    for(int i=0;i<max_client;i++){
        if(accept_sockets[i]==accept_sock){
            accept_sockets[i]=0;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
    return NULL;
}



int main(void)
{
    int server_sock;
    int accept_sock;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len= sizeof(client_addr);
    pthread_t tid;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock==-1){
        perror("socket creation fail");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(50000);
    
    
    int binding = bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr) );
    
    if (binding<0){
        perror("bind fail");
        close(server_sock);
        exit(1);
    }

    int listening=listen(server_sock,100);
    
    if (listening<0){
        perror("listen fail");
        close(server_sock);
        exit(1);
    }

    while(1){
        accept_sock=accept(server_sock,(struct sockaddr*)&client_addr,&client_addr_len);
        if (accept_sock<0){
            perror("accept fail");
            continue;
        }

        pthread_mutex_lock(&clients_mutex);

        for(int i=0;i<max_client;i++){
            if(accept_sockets[i]==0){
                accept_sockets[i]=accept_sock;
                int *p=&accept_sockets[i];
                pthread_create(&tid,NULL,server_work,(void *)p);
                break;
            }
        }

        pthread_mutex_unlock(&clients_mutex);

    }

    close(server_sock);
    return 0;
}