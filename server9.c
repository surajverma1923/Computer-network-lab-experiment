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
    int sd, acpt, bytes, port;
    char receiv[50];
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
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("Error in bind");
        exit(1);
    }

    if (listen(sd, 3) < 0) {
        perror("Error in listen");
        exit(1);
    }

    socklen_t addr_size = sizeof(struct sockaddr_in);

    // Accept connection
    if ((acpt = accept(sd, (struct sockaddr *)&serv, &addr_size)) < 0) {
        perror("Error in accept");
        exit(1);
    }

    while (1) {
        bytes = recv(acpt, receiv, sizeof(receiv), 0);
        if (bytes <= 0) {
            perror("Connection closed or error occurred");
            break;
        }
        receiv[bytes] = '\0'; // Null-terminate the received data

        if (strcmp(receiv, "end") == 0) {
            printf("Client disconnected.\n");
            break;
        } else {
            printf("Command received: %s\n", receiv);

            // Execute the command and capture the output
            FILE *cmd_output = popen(receiv, "r");
            if (cmd_output == NULL) {
                perror("Error executing command");
            } else {
                char output_buffer[256];
                while (fgets(output_buffer, sizeof(output_buffer), cmd_output) != NULL) {
                    printf("%s", output_buffer);
                }
                pclose(cmd_output); // Close the command output stream
            }
        }
    }

    close(acpt); // Close the accepted socket
    close(sd);   // Close the server socket

    return 0;
}
