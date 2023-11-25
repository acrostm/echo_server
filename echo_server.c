/* This is the echo server, it takes the string from the client socket, return a capitalized string */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define TRUE 1
#define FALSE 0

#define SERVER_PORT 8000

#define BUFFER_SIZE 256

int main(int argc, char** argv) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0); // using TCP protocol

    // reset the struct label
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET; // select protocol: IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listen all incoming ip address
    server_addr.sin_port = htons(SERVER_PORT);

    bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(sock, BUFFER_SIZE);

    printf("Waiting for client connection...\n");

    while(TRUE) {
        struct sockaddr_in client;
        int client_sock;
        socklen_t client_addr_len;
        char client_ip[BUFFER_SIZE];
        char buffer[BUFFER_SIZE];
        int numRead;
        int numWritten;

        client_addr_len = sizeof(client);
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);

        printf("Client ip: %s\t port: %d\n", inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)), ntohs(client.sin_port));

        // read data from client
        numRead = read(client_sock,  buffer, sizeof(buffer) - 1); // sizeof buffer is 256, leave one place for '\0'
        buffer[numRead] = '\0';

        printf("Receive[%d]: %s\n", numRead, buffer);

        for(int i = 0; i < numRead; i++) {
            buffer[i] = toupper(buffer[i]);
        }

        numWritten = write(client_sock, buffer, numRead);

        printf("Write finished![%d]\n", numWritten);

        close(client_sock);
    }  

    return 0;
}
