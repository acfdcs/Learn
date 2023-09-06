/*
 * client
 * usage: socket_test <count> <size> <serverip>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>


enum {
    DEST_PORT = 7335,
    DEFSIZE = 67,
    //DEFFREQ = 1,
};

static long n_send;

void show_usage(char *prog)
{
    fprintf(stderr, "Usage: %s [-c <count>] [-p port] [-s <size>]  [-q <cycle>] server_ip \n", prog);
    exit(1);
}

int main(int argc, char **argv)
{
    char *thisarg;
    struct timeval con_start_time, con_end_time;

    long count;
    int size = DEFSIZE;
    int b = 0;
    int cycle = 0;
    unsigned int port = DEST_PORT;

    int clientSocket = -1;
	struct sockaddr_in serverAddr;
    int len = sizeof(serverAddr);
    int ret;
    char **save_argv = argv;

    argc--;
    argv++;
    while (argc >= 1 && **argv == '-') {
        thisarg = *argv;
        thisarg++;
        switch (*thisarg) {
        case 'c':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            count = atoi(*argv);
            if (count < 11)
                count = 11;
            break;
        case 'p':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            port = atoi(*argv);
            break;
        case 's':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            size = atoi(*argv);
            break;
        case 'b':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            b = atoi(*argv);
            break;
        case 'q':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            cycle = atoi(*argv);
            break;
        default:
            show_usage(save_argv[0]);
        }
        argc--;
        argv++;
    }
    if (argc < 1)
        show_usage(save_argv[0]);
    char *serverip = *argv;

//--------------------------------------------------------------------------------------------------------------------
    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址信息
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, serverip, &(serverAddr.sin_addr)) <= 0) {
        perror("Failed to set server IP");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", serverip, port);

//---------------------------------------------------------------------------------------------------------------------------
    /*
     * 填充缓存区
     */
    char *tmp_buf;
    tmp_buf = (char *)malloc(size);
    char *recv_buf;
    recv_buf= (char *)malloc(size);
    int c=0;
    int ret_sum;
    double duration_sum;

    while (1) {
         for (int i = 0; i < size- 1; i++) {
	    tmp_buf[i] = 'A' + rand() % 26; 
	   }
	   c++;
	  if (count > 0 && n_send == count - 1)
              tmp_buf[0] = 'q';
	
        // 发送数据
        ret = sendto(clientSocket, tmp_buf, size, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
        
        if (ret == -1) 
        {
            perror("Failed to send data");
            exit(EXIT_FAILURE);
        }

         // 接收服务器的回应
        ssize_t recv_size = recvfrom(clientSocket, recv_buf, ret, 0,
                                    NULL, NULL);
        if (recv_size == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        // 比较接收的数据与发送的数据是否一致
        if (strncmp(tmp_buf, recv_buf, size) == 0) {
          printf("数据一致\n");
         } else {
          printf("数据不一致\n");
        }
        
        if (ret < 0) {
            perror("send");
        }
        n_send++;
        if (count > 0 && n_send >= count)
            break;

        int i;
        if (cycle)
            // usleep(1000 * 1000 / freq);
            usleep(cycle);

    }


    close(clientSocket);
    return 0;
}
