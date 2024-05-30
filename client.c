#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define buffer_size 1024
#define name_size 100

void *receive_message(void*arg){
    int client_sock = *((int*)arg);
    char buffer[buffer_size*2];
    int receive_byte;

    while((receive_byte=recv(client_sock,buffer,sizeof(buffer)-1,0))>0){
        buffer[receive_byte]='\0';
        printf("%s",buffer);
    }

    return NULL;
}

int main(void){
    int client_sock;
    struct sockaddr_in server_addr;
    pthread_t tid;
    char message[buffer_size];
    char name[name_size];
    char total_message[name_size+buffer_size+1];

    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(50000);
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    client_sock=socket(AF_INET,SOCK_STREAM,0);
    if(client_sock<0){
        perror("socket create fail");
        exit(1);
    }

    int connecting=connect(client_sock,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(connecting<0){
        perror("connect fail");
        close(client_sock);
        exit(1);
    }

    pthread_create(&tid,NULL,receive_message,(void*)&client_sock);

    printf("이름:");
    fgets(name,sizeof(name),stdin);
    name[strcspn(name, "\n")] = 0;

    while(1){
        
        fgets(message,sizeof(message),stdin);
        
        snprintf(total_message, sizeof(total_message), "%s: %s", name, message);
        send(client_sock,total_message,strlen(total_message),0);
        
        
    }

    close(client_sock);
    return 0;

}