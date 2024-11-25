#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int size, n;
    printf("Enter size of the array: ");
    scanf("%d", &size);
    sprintf(buffer, "%d", size);
    sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    int arr[size];
    printf("Enter the elements of the array:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    // Send array to server
    sendto(sockfd, (const int *)arr, size * sizeof(int), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    // Receive and print steps of each iteration
    printf("Steps of Bubble Sort:\n");
    while (1) {
        n = recvfrom(sockfd, (int *)arr, size * sizeof(int), MSG_WAITALL, NULL, NULL);
        if (n <= 0) {
            break;
        }
        printArray(arr, size);
    }

    close(sockfd);
    return 0;
}
