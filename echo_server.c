/* This is the echo server, it takes the string from the client socket, return a capitalized string */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>


#define TRUE 1
#define FALSE 0

#define SERVER_PORT 8000

#define BUFFER_SIZE 256

void* handleRequest(void* arg);

int main(int argc, char** argv) {
    int sock;
    int status;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0); // using TCP protocol
    if(sock < 0)
    {
        fprintf(stderr, "Socket error, reason: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // reset the struct
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET; // select protocol: IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listen all incoming ip address
    server_addr.sin_port = htons(SERVER_PORT);

    bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(sock, 10);

    printf("Waiting for client connection...\n");

    while(TRUE) {
        struct sockaddr_in client;
        int client_sock;
        socklen_t client_addr_len;
        char client_ip[BUFFER_SIZE];
        char buffer[BUFFER_SIZE];
        int numRead;
        int numWritten;
        int err = -1;

        client_addr_len = sizeof(client);
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);
        if(client_sock < 0)
        {
            fprintf(stderr, "Accept error, reason: %s\n", strerror(errno));
            continue;
        }

        printf("Client ip: %s\t port: %d | ", inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)), ntohs(client.sin_port));

        {
			pthread_t tid;
			int *int_ptr_clientSock = NULL;
			
			int_ptr_clientSock = (int *)malloc(sizeof(int));
			*int_ptr_clientSock = client_sock;
			
			if(pthread_create(&tid, NULL, handleRequest, (void*)int_ptr_clientSock) == err)
            {
				fprintf(stderr, "Can't create thread, reason: %s\n", strerror(errno));
				if(int_ptr_clientSock) free(int_ptr_clientSock);
			}

            printf("Thread [%ld]\n", tid);
			
		}
    }
    status = close(sock);
    if(status < 0)
    {
        fprintf(stderr, "Close error, reason: %s\n", strerror(errno));
    }

    exit(EXIT_SUCCESS);
}

void* handleRequest(void* arg)
{
    int client_sock = *(int *)arg;
    int status;
    char buffer[BUFFER_SIZE];
    int numRead, numWritten;

    numRead = read(client_sock, buffer, sizeof(buffer) - 1);
    if(numRead < 0)
    {
        fprintf(stderr, "Read error, reason: %s\n", strerror(errno));
        if(arg) free(arg);
        return NULL;
    }

    buffer[numRead] = '\0';
    
    printf("Received from client[%d]: %s\n", numRead, buffer);

    for(int i = 0; i < numRead; i++)
    {
        buffer[i] = toupper(buffer[i]);
    }

    numWritten = write(client_sock, buffer, numRead);
    if(numWritten < numRead)
    {
        fprintf(stderr, "Write error, reason: %s\n", strerror(errno));
        if(arg) free(arg);
        return NULL;
    }

    printf("Sent to client[%d]: %s\n", numWritten, buffer);

    status = close(client_sock);
    if (status < 0)
    {
        fprintf(stderr, "Close error, reason: %s\n", strerror(errno));
        if(arg) free(arg);
        return NULL;
    }

    if(arg) free(arg);

    return NULL;
}
