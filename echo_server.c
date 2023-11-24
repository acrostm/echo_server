#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SERVER_PORT 8000

int main(void) {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0); // using TCP protocol

    // reset the struct label
    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET; // select protocol: IPV4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // listen all incoming ip address
    server_addr.sin_port = htons(SERVER_PORT);

    bind(sock, (struct sockaddr *) &server_addr, sizeof(server_addr));

    listen(sock, 128);

    printf("Waiting for client connection...\n");

    int done = 1;
    while(done) {
        struct sockaddr_in client;
        int client_sock;
        socklen_t client_addr_len;
        char client_ip[64];

        client_addr_len = sizeof(client);
        client_sock = accept(sock, (struct sockaddr *)&client, &client_addr_len);

        printf("Client ip: %s\t port: %d\n", inet_ntop(AF_INET, &client.sin_addr.s_addr, client_ip, sizeof(client_ip)), ntohs(client.sin_port));
    }  

    return 0;
}