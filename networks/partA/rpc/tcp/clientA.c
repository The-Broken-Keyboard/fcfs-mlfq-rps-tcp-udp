#include "headers.h"

int main()
{

    int shrey_sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    shrey_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (shrey_sock < 0)
    {
        fprintf(stderr, "[-]Socket creation error: %s\n", strerror(errno));

        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port_A;
    addr.sin_addr.s_addr = inet_addr(ip_address);

    if (connect(shrey_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        fprintf(stderr, "[-]Connection time error: %s\n", strerror(errno));
        exit(1);
    }
    printf("Welcome, Player A!\n");
    int flag = 0;
    while (1)
    {
        bzero(buffer, 1024);
        printf("Enter 0 for rock, 1 for paper and 2 for scissors: ");
        if (flag)
            getchar();
        flag = 1;
        scanf("%[^\n]", buffer);
        getchar();
        if (strcmp(buffer, "1") == 0 || strcmp(buffer, "0") == 0 || strcmp(buffer, "2") == 0)
        {
            if (strcmp(buffer, "0") == 0)
                printf("Your choice is rock!\n");
            else if (strcmp(buffer, "1") == 0)
                printf("Your choice is paper!\n");
            else if (strcmp(buffer, "2") == 0)
                printf("Your choice is scissors!\n");
        }
        else
        {
            printf("Invalid choice, please re-enter\n");
            continue;
        }
        if (send(shrey_sock, buffer, strlen(buffer), 0) < 0)
        {
            fprintf(stderr, "[-]Send time error: %s\n", strerror(errno));
            if (close(shrey_sock) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            exit(1);
        }

        bzero(buffer, 1024);
        if (recv(shrey_sock, buffer, sizeof(buffer), 0) < 0)
        {
            fprintf(stderr, "[-]Receive time error: %s\n", strerror(errno));
            if (close(shrey_sock) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));

            exit(1);
        }
        printf("%s\n", buffer);
        printf("Want a rematch? ( Give 0 to request rematch or else give any other input to end game) : ");
        bzero(buffer, 1024);
        scanf("%s", buffer);
        if (strcmp(buffer, "0") != 0)
        {
            bzero(buffer, 1024);
            strcpy(buffer, "1");
        }

        if (send(shrey_sock, buffer, strlen(buffer), 0) < 0)
        {
            fprintf(stderr, "[-]Send time error: %s\n", strerror(errno));
            if (close(shrey_sock) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
            exit(1);
        }
        bzero(buffer, 1024);
        if (recv(shrey_sock, buffer, sizeof(buffer), 0) < 0)
        {
            fprintf(stderr, "[-]Receive time error: %s\n", strerror(errno));
            if (close(shrey_sock) < 0)
                fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));

            exit(1);
        }
        printf("%s\n", buffer);
        if (buffer[0] == 'R')
            continue;
        break;
    }
    if (close(shrey_sock) < 0)
    {
        fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
        exit(1);
    }
    printf("Game over\n");

    return 0;
}