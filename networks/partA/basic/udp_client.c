#include "headers.h"
int main(){

  int shrey_sock;
  struct sockaddr_in addr;
  char buffer[1024];
  socklen_t addr_size;

  shrey_sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(shrey_sock<0)
  {
    fprintf(stderr,"[-]Socket creation error: %s\n",strerror(errno));
    exit(1);
  }
  printf("[+]UDP socket created\n");
  memset(&addr, '\0', sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(ip_address);

  bzero(buffer, 1024);
  printf("Send message to server: ");
  scanf("%[^\n]",buffer);
  if(sendto(shrey_sock, buffer, 1024, 0, (struct sockaddr*)&addr, sizeof(addr))<0)
  {
    fprintf(stderr,"[-]Send time error: %s\n",strerror(errno));
    exit(1);
  }
  printf("[+]Data sent to server: %s\n", buffer);

  bzero(buffer, 1024);
  addr_size = sizeof(addr);
  if( recvfrom(shrey_sock, buffer, 1024, 0, (struct sockaddr*)&addr, &addr_size)<0)
  {
    fprintf(stderr,"[-]Error while receiving data\n");
    exit(1);
  }
  printf("[+]Data received from server: %s\n", buffer);

  return 0;
}