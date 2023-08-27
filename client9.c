#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

int main() {
    int sd, port;
    char send1[50];
    struct sockaddr_in serv;

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error in socket");
        exit(1);
    }

    bzero(&serv, sizeof(serv));
    printf("Enter the port number: ");
    scanf("%d", &port);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change this to the server IP

    if (connect(sd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("Error in connection");
        exit(1);
    }

    while (1) {
        printf("Enter the command (or 'end' to exit): ");
        fgets(send1, sizeof(send1), stdin); // Use fgets to read input
        send1[strcspn(send1, "\n")] = '\0'; // Remove newline character

        if (strcmp(send1, "end") == 0) {
            send(sd, send1, strlen(send1), 0); // Send 'end' command
            close(sd); // Close the socket
            break;
        }

        send(sd, send1, strlen(send1), 0); // Send the command
    }

    return 0;
}
