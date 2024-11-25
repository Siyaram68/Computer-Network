#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    int arr_size;
    printf("Enter size of the array: ");
    scanf("%d", &arr_size);
    int arr[arr_size];

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter the elements of the array:\n");
    for (int i = 0; i < arr_size; i++) {
        scanf("%d", &arr[i]);
    }

    // Send size of the array to server
    send(sock, &arr_size, sizeof(int), 0);

    // Send array to server
    send(sock, arr, arr_size * sizeof(int), 0);

    // Receive sum from server
    int sum;
    read(sock, &sum, sizeof(int));
    printf("Sum received from server: %d\n", sum);

    close(sock);
    return 0;
}
