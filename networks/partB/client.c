#include "headers.h"
int remaining(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i] == 0)
            return i;
    }
    return -1;
}
int main()
{

    int shrey_sock;
    struct sockaddr_in addr;
    struct timeval timetowait;
    char buffer[1024];
    bzero(buffer, 1024);
    strcpy(buffer, "Client here, what's up!\n");

    socklen_t addr_size;
    int ackno, rec, snd, totalpackets, sndOrRec;
    totalpackets = 0;
    sndOrRec = 0;
    shrey_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (shrey_sock < 0)
    {
        fprintf(stderr, "[-]Socket creation error: %s\n", strerror(errno));
        exit(1);
    }
    printf("[+]UDP socket created\n");
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip_address);
    int flag = 0;
    while (1)
    {
        printf("Please enter 1 to send data or 2 to receive data or else any other number to exit:");
        if (flag == 1)
            getchar();
        flag = 1;
        scanf("%d", &sndOrRec);
        if (sndOrRec != 1 && sndOrRec != 2)
            sndOrRec = 0;
        // getchar();
        if (sendto(shrey_sock, &sndOrRec, sizeof(sndOrRec), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            fprintf(stderr, "[-]Send time error: %s\n", strerror(errno));
            exit(1);
        }
        if (sndOrRec == 0)
            break;
        else if (sndOrRec == 1)
        {
            totalpackets = strlen(buffer);
            if (sendto(shrey_sock, &totalpackets, sizeof(totalpackets), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                fprintf(stderr, "[-]Send time error: %s\n", strerror(errno));
                exit(1);
            }
            timetowait.tv_sec = 0;
            timetowait.tv_usec = 100000;
            setsockopt(shrey_sock, SOL_SOCKET, SO_RCVTIMEO, &timetowait, sizeof(timetowait));
            int attendanceack[totalpackets];
            for (int i = 0; i < totalpackets; i++)
                attendanceack[i] = 0;
            while (1)
            {
                int seq = remaining(attendanceack, totalpackets);
                if (seq == -1)
                    break;
                int sentcount = 0;
                for (int i = seq; i < totalpackets; i++)
                {
                    if (attendanceack[i] == 1)
                        continue;
                    struct data_packet packet;
                    packet.data_char = buffer[i];
                    packet.sequence_no = i;
                    if (sendto(shrey_sock, &packet, sizeof(struct data_packet), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
                    {
                        fprintf(stderr, "[-]Error while sending data\n");
                        exit(1);
                    }
                    sentcount++;
                    printf("%c:%d (sent)\n", packet.data_char, packet.sequence_no);
                }
                while (sentcount--)
                {
                    addr_size = sizeof(addr);
                    if (recvfrom(shrey_sock, &ackno, sizeof(int), 0, (struct sockaddr *)&addr, &addr_size) < 0)
                    {
                        if (errno != EWOULDBLOCK)
                        {
                            fprintf(stderr, "[-]Error while receiving data\n");
                            exit(1);
                        }
                        else
                            continue;
                    }
                    printf("Acknowledgement for sequence %d received\n", ackno);
                    attendanceack[ackno] = 1;
                }
            }
        }
        else if (sndOrRec == 2)
        {
            addr_size = sizeof(addr);
            if (recvfrom(shrey_sock, &totalpackets, sizeof(int), 0, (struct sockaddr *)&addr, &addr_size) < 0)
            {
                if (errno != EWOULDBLOCK)
                {
                    fprintf(stderr, "[-]Error while receiving data\n");
                    exit(1);
                }
                else
                    continue;
            }
            printf("Server is sending %d packets\n");
            timetowait.tv_sec = 0;
            timetowait.tv_usec = 100000;
            setsockopt(shrey_sock, SOL_SOCKET, SO_RCVTIMEO, &timetowait, sizeof(timetowait));
            struct data_packet packet_array[totalpackets];
            int attendanceRec[totalpackets];
            for (int i = 0; i < totalpackets; i++)
                attendanceRec[i] = 0;
            int totalrec = 0;
            while (totalrec < totalpackets)
            {
                addr_size = sizeof(addr);
                struct data_packet recPacket;
                if (recvfrom(shrey_sock, &recPacket, sizeof(recPacket), 0, (struct sockaddr *)&addr, &addr_size) < 0)
                {
                    if (errno != EWOULDBLOCK)
                    {
                        fprintf(stderr, "[-]Error while receiving data\n");
                        exit(1);
                    }
                    else
                        continue;
                }
                totalrec++;
                printf("%c:%d (received)\n", recPacket.data_char, recPacket.sequence_no);
                attendanceRec[recPacket.sequence_no] = 1;
                packet_array[recPacket.sequence_no].data_char = recPacket.data_char;
                packet_array[recPacket.sequence_no].sequence_no = recPacket.sequence_no;
                if (sendto(shrey_sock, &recPacket.sequence_no, sizeof(int), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0)
                {
                    fprintf(stderr, "[-]Error while sending data\n");
                    exit(1);
                }
                printf("Send the acknowledgement for packet sequence %d\n", recPacket.sequence_no);
            }
            for (int i = 0; i < totalpackets; i++)
            {
                printf("%c", packet_array[i].data_char);
            }
            printf("\n");
        }
    }
    printf("Data exchange terminated\n");

    return 0;
}