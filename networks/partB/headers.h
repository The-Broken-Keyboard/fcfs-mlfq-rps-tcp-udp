#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PACKET_SIZE 1
#define RETRANSMISSION_TIME_GAP 100000
#define ip_address "127.0.0.1"
#define port 10048

struct data_packet{
    int sequence_no;
    char data_char;
};