#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void bubbleSort(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j+1]
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    int len, n;
    len = sizeof(cliaddr); //len is value/resuslt

    // Receive size of the array from client
    n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    int size = atoi(buffer);

    // Receive array from client
    int arr[size];
    n = recvfrom(sockfd, (int *)arr, size * sizeof(int), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);

    // Perform bubble sort and send array after each iteration
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j+1]
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;

                // Send array after each iteration
                sendto(sockfd, (const int *)arr, size * sizeof(int), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
            }
        }
    }

    // Send sorted array to client
    sendto(sockfd, (const int *)arr, size * sizeof(int), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);

    printf("Sorted array sent to client.\n");

    return 0;
}
