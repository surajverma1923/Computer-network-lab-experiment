#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "netdb.h"
#include "arpa/inet.h"

#define MAX 1000
#define BACKLOG 5 // how many pending connections queue will hold

int main() {
    char serverMessage[MAX];
    char clientMessage[MAX];
    
    // create the server socket
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9002);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // calling bind function to our specified IP and port
    bind(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    listen(socketDescriptor, BACKLOG);

    // starting the accepting
    int clientSocketDescriptor = accept(socketDescriptor, NULL, NULL);

    while (1) {
        printf("\ntext message here ... :");

        // Use fgets instead of scanf to read an entire line, including spaces
        if (fgets(serverMessage, sizeof(serverMessage), stdin) == NULL) {
            perror("fgets");
            break; // Exit the loop on input error
        }

        // Remove the newline character from the input
        serverMessage[strlen(serverMessage) - 1] = '\0';

        send(clientSocketDescriptor, serverMessage, sizeof(serverMessage), 0);

        // receive the data from the client
        recv(clientSocketDescriptor, clientMessage, sizeof(clientMessage), 0);

        // received data from the client successfully, then print the data obtained from the client
        printf("\nCLIENT: %s", clientMessage);
    }

    // close the socket
    close(socketDescriptor);
    return 0;
}
