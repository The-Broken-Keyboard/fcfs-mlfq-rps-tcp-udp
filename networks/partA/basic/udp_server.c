#include "headers.h"
int main(){

  int shrey_sock;
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];
  socklen_t addr_size;
  int n;

  shrey_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (shrey_sock < 0){
    fprintf(stderr,"[-]Socket creation error: %s\n", strerror(errno));
    exit(1);
  }

  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip_address);

  n = bind(shrey_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0) {
    fprintf(stderr,"[-]Bind time error: %s\n",strerror(errno));
    exit(1);
  }

  bzero(buffer, 1024);
  addr_size = sizeof(client_addr);
  if(recvfrom(shrey_sock, buffer, 1024, 0, (struct sockaddr*)&client_addr, &addr_size)<0)
  {
    fprintf(stderr,"[-]Error while receiving data\n");
    exit(1);
  }
  printf("[+]Data received from client: %s\n", buffer);

  bzero(buffer, 1024);
  printf("Enter your message to client: ");
  scanf("%[^\n]",buffer);
  if(sendto(shrey_sock, buffer, 1024, 0, (struct sockaddr*)&client_addr, sizeof(client_addr))<0)
  {
    fprintf(stderr,"[-]Error while sending data\n");
    exit(1);
  }
  printf("[+]Data sent to client: %s\n", buffer);

  return 0;
}