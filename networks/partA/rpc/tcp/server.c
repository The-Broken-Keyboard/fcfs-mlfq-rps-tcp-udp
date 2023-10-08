#include "headers.h"
int main()
{

    int server_sock1, client_sock1, server_sock2, client_sock2;
    struct sockaddr_in server_addr1, client_addr1, server_addr2, client_addr2;
    socklen_t addr_size1, addr_size2;
    char bufferA[1024];
    char bufferB[1024];
    int A_input = 0;
    int B_input = 0;
    int score_A = 0;
    int score_B = 0;
    int n;

    server_sock1 = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock1 < 0)
    {
        fprintf(stderr, "[-]Socket creation error: %s\n", strerror(errno));
        exit(1);
    }
    server_sock2 = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock2 < 0)
    {
        fprintf(stderr, "[-]Socket creation error: %s\n", strerror(errno));
        exit(1);
    }
    printf("[+]TCP server sockets created.\n");

    memset(&server_addr1, '\0', sizeof(server_addr1));
    server_addr1.sin_family = AF_INET;
    server_addr1.sin_port = port_A;
    server_addr1.sin_addr.s_addr = inet_addr(ip_address);

    memset(&server_addr2, '\0', sizeof(server_addr2));
    server_addr2.sin_family = AF_INET;
    server_addr2.sin_port = port_B;
    server_addr2.sin_addr.s_addr = inet_addr(ip_address);

    n = bind(server_sock1, (struct sockaddr *)&server_addr1, sizeof(server_addr1));
    if (n < 0)
    {
        fprintf(stderr, "[-]Bind error: %s\n", strerror(errno));
        close(server_sock1);
        exit(1);
    }
    n = bind(server_sock2, (struct sockaddr *)&server_addr2, sizeof(server_addr2));
    if (n < 0)
    {
        fprintf(stderr, "[-]Bind error: %s\n", strerror(errno));
        close(server_sock2);
        exit(1);
    }
    printf("[+]Bind to the port numbers: %d & %d\n", port_A, port_B);
    if (listen(server_sock1, 5) < 0)
    {
        fprintf(stderr, "[-]Listen error: %s\n", strerror(errno));
        close(server_sock1);
        exit(1);
    }
    if (listen(server_sock2, 5) < 0)
    {
        fprintf(stderr, "[-]Listen error: %s\n", strerror(errno));
        close(server_sock2);
        exit(1);
    }
    printf("Listening...\n");
    addr_size1 = sizeof(client_addr1);
    client_sock1 = accept(server_sock1, (struct sockaddr *)&client_addr1, &addr_size1);
    if (client_sock1 < 0)
    {
        fprintf(stderr, "[-]Accept error: %s\n", strerror(errno));
        if (close(server_sock1) < 0)
            fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
        exit(1);
    }
    printf("[+]Player A connected.\n");
    addr_size2 = sizeof(client_addr2);
    client_sock2 = accept(server_sock2, (struct sockaddr *)&client_addr2, &addr_size2);
    if (client_sock2 < 0)
    {
        fprintf(stderr, "[-]Accept error: %s\n", strerror(errno));
        if (close(server_sock2) < 0)
            fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
        exit(1);
    }
    printf("[+]Player B connected.\n");

    while (1)
    {

        bzero(bufferA, 1024);
        if (recv(client_sock1, bufferA, sizeof(bufferA), 0) < 0)
        {
            fprintf(stderr, "[-]Receive error: %s\n", strerror(errno));
            if (close(client_sock1) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            if (close(server_sock1) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            exit(1);
        }
        if (strcmp(bufferA, "0") == 0)
            A_input = 0;
        else if (strcmp(bufferA, "1") == 0)
            A_input = 1;
        else if (strcmp(bufferA, "2") == 0)
            A_input = 2;
        bzero(bufferB, 1024);
        if (recv(client_sock2, bufferB, sizeof(bufferB), 0) < 0)
        {
            fprintf(stderr, "[-]Receive error: %s\n", strerror(errno));
            if (close(client_sock2) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            if (close(server_sock2) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            exit(1);
        }
        if (strcmp(bufferB, "0") == 0)
            B_input = 0;
        else if (strcmp(bufferB, "1") == 0)
            B_input = 1;
        else if (strcmp(bufferB, "2") == 0)
            B_input = 2;
        // printf("Client's message: %s\n", bufferA);
        // printf("Reply so`mething: ");

        // buffer[0]='\0';
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

        // bzero(buffer, 1024);
        // strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        // printf("Server: %s\n", buffer);
        if (send(client_sock1, bufferA, strlen(bufferA), 0) < 0)
        {
            fprintf(stderr, "[-]Sendtime error: %s\n", strerror(errno));
            if (close(client_sock1) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            if (close(server_sock1) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            exit(1);
        }
        if (send(client_sock2, bufferB, strlen(bufferB), 0) < 0)
        {
            fprintf(stderr, "[-]Sendtime error: %s\n", strerror(errno));
            if (close(client_sock2) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            if (close(server_sock2) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            exit(1);
        }
        bzero(bufferA, 1024);
        bzero(bufferB, 1024);
        if (recv(client_sock1, bufferA, sizeof(bufferA), 0) < 0)
        {
            fprintf(stderr, "[-]Receive error: %s\n", strerror(errno));
            if (close(client_sock1) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            if (close(server_sock1) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            exit(1);
        }
        if (recv(client_sock2, bufferB, sizeof(bufferB), 0) < 0)
        {
            fprintf(stderr, "[-]Receive error: %s\n", strerror(errno));
            if (close(client_sock2) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            if (close(server_sock2) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            exit(1);
        }
        if (strcmp(bufferA, "1") == 0 || strcmp(bufferB, "1") == 0)
        {
            bzero(bufferA, 1024);
            bzero(bufferB, 1024);
            sprintf(bufferA, "No rematch because both players didn't agree to rematch. You won %d matches", score_A);
            sprintf(bufferB, "No rematch because both players didn't agree to rematch. You won %d matches", score_B);
            if (send(client_sock1, bufferA, strlen(bufferA), 0) < 0)
            {
                fprintf(stderr, "[-]Sendtime error: %s\n", strerror(errno));
                if (close(client_sock1) < 0)
                    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
                if (close(server_sock1) < 0)
                    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
                exit(1);
            }
            if (send(client_sock2, bufferB, strlen(bufferB), 0) < 0)
            {
                fprintf(stderr, "[-]Sendtime error: %s\n", strerror(errno));
                if (close(client_sock2) < 0)
                    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
                if (close(server_sock2) < 0)
                    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
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
            if (send(client_sock1, bufferA, strlen(bufferA), 0) < 0)
            {
                fprintf(stderr, "[-]Sendtime error: %s\n", strerror(errno));
                if (close(client_sock1) < 0)
                    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
                if (close(server_sock1) < 0)
                    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
                exit(1);
            }
            if (send(client_sock2, bufferB, strlen(bufferB), 0) < 0)
            {
                fprintf(stderr, "[-]Sendtime error: %s\n", strerror(errno));
                if (close(client_sock2) < 0)
                    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
                if (close(server_sock2) < 0)
                    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
                exit(1);
            }
        }
    }
    int flag2=0;
    if (close(client_sock1) < 0){
        flag2=1;
        fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
    }
    if (close(server_sock1) < 0){
        flag2=1;
        fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
    }
    if (close(client_sock2) < 0){
        flag2=1;
        fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
    }
    if (close(server_sock2) < 0){
        flag2=1;
        fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
    }
    if(flag2)
    exit(1);
    return 0;
}