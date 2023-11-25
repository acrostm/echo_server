/* This is the echo client */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define TRUE  1
#define FALSE 0

#define SERVER_PORT 8000
#define SERVER_IP "127.0.0.1"

#define BUFFER_SIZE 256

int main(int argc, char** argv) {
    int sockfd;
    char *message = argv[1];
    struct sockaddr_in server_addr;
    int numWritten;
    int numRead;
    char buffer[BUFFER_SIZE];

    if(argc != 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        printf("Usage: %s <message>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("<Message> %s\n", message);
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // IPV4, TCP

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    numWritten = write(sockfd, message, strlen(message));
    numRead = read(sockfd, buffer, sizeof(buffer) - 1);

    if(numRead > 0) {
        buffer[numRead] = '\0';
        printf("Receive[%d]: %s\n", numRead, buffer);
    } else {
        perror("Receive failed!\n");
    }

    printf("Finished\n");
    close(sockfd);

    exit(EXIT_SUCCESS);
}