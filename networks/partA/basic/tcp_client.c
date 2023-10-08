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
  addr.sin_port = port;
  addr.sin_addr.s_addr = inet_addr(ip_address);

  if (connect(shrey_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
  {
    fprintf(stderr, "[-]Connection time error: %s\n", strerror(errno));
    exit(1);
  }
  printf("Connected to the server.\n");

  bzero(buffer, 1024);
  printf("Your message: ");
  scanf("%[^\n]", buffer);
  printf("Sending message to server: %s\n", buffer);

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
  printf("Server's response: %s\n", buffer);

  if (close(shrey_sock) < 0)
  {
    fprintf(stderr, "[-]Error closing socket: %s\n", strerror(errno));
    exit(1);
  }
  printf("Disconnected from the server.\n");

  return 0;
}