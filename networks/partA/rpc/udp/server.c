#include "headers.h"
int main()
{

    int shrey_sock1, shrey_sock2;
    struct sockaddr_in server_addr1, client_addr1, server_addr2, client_addr2;
    char bufferA[1024];
    char bufferB[1024];
    socklen_t addr_size1, addr_size2;
    int A_input = 0;
    int B_input = 0;
    int score_A = 0;
    int score_B = 0;
    int n;

    shrey_sock1 = socket(AF_INET, SOCK_DGRAM, 0);
    if (shrey_sock1 < 0)
    {
        fprintf(stderr, "[-]Socket creation error: %s\n", strerror(errno));
        exit(1);
    }
    shrey_sock2 = socket(AF_INET, SOCK_DGRAM, 0);
    if (shrey_sock2 < 0)
    {
        fprintf(stderr, "[-]Socket creation error: %s\n", strerror(errno));
        exit(1);
    }
    printf("[+]UDP sockets created\n");
    memset(&server_addr1, '\0', sizeof(server_addr1));
    server_addr1.sin_family = AF_INET;
    server_addr1.sin_port = htons(port_A);
    server_addr1.sin_addr.s_addr = inet_addr(ip_address);
    memset(&server_addr2, '\0', sizeof(server_addr1));
    server_addr2.sin_family = AF_INET;
    server_addr2.sin_port = htons(port_B);
    server_addr2.sin_addr.s_addr = inet_addr(ip_address);

    n = bind(shrey_sock1, (struct sockaddr *)&server_addr1, sizeof(server_addr1));
    if (n < 0)
    {
        fprintf(stderr, "[-]Bind time error: %s\n", strerror(errno));
        exit(1);
    }
    n = bind(shrey_sock2, (struct sockaddr *)&server_addr2, sizeof(server_addr2));
    if (n < 0)
    {
        fprintf(stderr, "[-]Bind time error: %s\n", strerror(errno));
        exit(1);
    }
    printf("[+]Binded to the port numbers %d and %d\n", port_A, port_B);
    bzero(bufferA, 1024);
    bzero(bufferB, 1024);
    addr_size1 = sizeof(client_addr1);
    addr_size2 = sizeof(client_addr2);
    while (1)
    {
        bzero(bufferA, 1024);
        bzero(bufferB, 1024);
        if (recvfrom(shrey_sock1, bufferA, 1024, 0, (struct sockaddr *)&client_addr1, &addr_size1) < 0)
        {
            fprintf(stderr, "[-]Error while receiving data\n");
            exit(1);
        }
        if (strcmp(bufferA, "0") == 0)
            A_input = 0;
        else if (strcmp(bufferA, "1") == 0)
            A_input = 1;
        else if (strcmp(bufferA, "2") == 0)
            A_input = 2;
        if (recvfrom(shrey_sock2, bufferB, 1024, 0, (struct sockaddr *)&client_addr2, &addr_size2) < 0)
        {
            fprintf(stderr, "[-]Error while receiving data\n");
            exit(1);
        }
        if (strcmp(bufferB, "0") == 0)
            B_input = 0;
        else if (strcmp(bufferB, "1") == 0)
            B_input = 1;
        else if (strcmp(bufferB, "2") == 0)
            B_input = 2;
        // printf("[+]Data received from client: %s\n", buffer);

        bzero(bufferA, 1024);
        bzero(bufferB, 1024);
        if (A_input == 0)
            strcpy(bufferB, "Your opponent chose rock, ");
        else if (A_input == 1)
            strcpy(bufferB, "Your opponent chose paper, ");
        else if (A_input == 2)
            strcpy(bufferB, "Your opponent chose scissors, ");
        else
        {
            strcpy(bufferB, "Oops! some error occured");
        }
        if (B_input == 0)
            strcpy(bufferA, "Your opponent chose rock, ");
        else if (B_input == 1)
            strcpy(bufferA, "Your opponent chose paper, ");
        else if (B_input == 2)
            strcpy(bufferA, "Your opponent chose scissors, ");
        else
        {
            strcpy(bufferA, "Oops! some error occured");
        }

        if ((B_input == 0 && A_input == 1) || (B_input == 1 && A_input == 2) || (B_input == 2 && A_input == 0))
        {
            strcat(bufferA, "You won!!");
            strcat(bufferB, "You lost!!");
            score_A++;
        }
        else if ((A_input == 0 && B_input == 1) || (A_input == 1 && B_input == 2) || (A_input == 2 && B_input == 0))
        {
            strcat(bufferA, "You lost!!");
            strcat(bufferB, "You won!!");
            score_B++;
        }
        else if (A_input == B_input)
        {
            strcat(bufferA, "It's a draw!!");
            strcat(bufferB, "It's a draw!!");
        }

        if (sendto(shrey_sock1, bufferA, 1024, 0, (struct sockaddr *)&client_addr1, sizeof(client_addr1)) < 0)
        {
            fprintf(stderr, "[-]Error while sending data\n");
            exit(1);
        }
        if (sendto(shrey_sock2, bufferB, 1024, 0, (struct sockaddr *)&client_addr2, sizeof(client_addr2)) < 0)
        {
            fprintf(stderr, "[-]Error while sending data\n");
            exit(1);
        }

        bzero(bufferA, 1024);
        bzero(bufferB, 1024);
        if (recvfrom(shrey_sock1, bufferA, 1024, 0, (struct sockaddr *)&client_addr1, &addr_size1) < 0)
        {
            fprintf(stderr, "[-]Error while receiving data\n");
            exit(1);
        }
        if (recvfrom(shrey_sock2, bufferB, 1024, 0, (struct sockaddr *)&client_addr2, &addr_size2) < 0)
        {
            fprintf(stderr, "[-]Error while receiving data\n");
            exit(1);
        }

        if (strcmp(bufferA, "1") == 0 || strcmp(bufferB, "1") == 0)
        {
            bzero(bufferA, 1024);
            bzero(bufferB, 1024);
            sprintf(bufferA, "No rematch because both players didn't agree to rematch. You won %d matches", score_A);
            sprintf(bufferB, "No rematch because both players didn't agree to rematch. You won %d matches", score_B);
            if (sendto(shrey_sock1, bufferA, 1024, 0, (struct sockaddr *)&client_addr1, sizeof(client_addr1)) < 0)
            {
                fprintf(stderr, "[-]Error while sending data\n");
                exit(1);
            }
            if (sendto(shrey_sock2, bufferB, 1024, 0, (struct sockaddr *)&client_addr2, sizeof(client_addr2)) < 0)
            {
                fprintf(stderr, "[-]Error while sending data\n");
                exit(1);
            }
            break;
        }
        else
        {
            bzero(bufferA, 1024);
            bzero(bufferB, 1024);
            strcpy(bufferA, "Rematch accepted!");
            strcpy(bufferB, "Rematch accepter!");
            if (sendto(shrey_sock1, bufferA, 1024, 0, (struct sockaddr *)&client_addr1, sizeof(client_addr1)) < 0)
            {
                fprintf(stderr, "[-]Error while sending data\n");
                exit(1);
            }
            if (sendto(shrey_sock2, bufferB, 1024, 0, (struct sockaddr *)&client_addr2, sizeof(client_addr2)) < 0)
            {
                fprintf(stderr, "[-]Error while sending data\n");
                exit(1);
            }
        }
    }
    
    return 0;
}