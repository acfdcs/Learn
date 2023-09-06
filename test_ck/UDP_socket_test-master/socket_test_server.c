/*
 * server 
 * usage: 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#define MCAST_PORT 8888
#define MCAST_ADDR "224.0.0.100"     /*一个局部连接多播地址，路由器不进行转发*/
#define LOCAL_ADDR "192.168.161.130"     /*本机网卡地址*/
#define MCAST_INTERVAL 5             /*发送间隔时间*/
#define BUFF_SIZE 256                /*接收缓冲区大小*/

#ifndef MAX_RECV_BUF
#define MAX_RECV_BUF  2621440
#endif

enum {
    DEST_PORT = 7335,
};

static long count;

void show_usage(char *prog)
{
    fprintf(stderr, "Usage: %s [-p port] [-b <boardcast>]\n", prog);
    exit(1);
}

int main(int argc, char **argv)
{
    uint32_t port = DEST_PORT;
    int b = 0;
    char *thisarg;
    char **save_argv = argv;

    argc--;
    argv++;
    while (argc >= 1 && **argv == '-') {
        thisarg = *argv;
        thisarg++;
        switch (*thisarg) {
        case 'p':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            port = atoi(*argv);
            break;
        case 'b':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            b = atoi(*argv);
            break;
        default:
            show_usage(save_argv[0]);
        }
        argc--;
        argv++;
    }

    if (b==1)
    {
        struct sockaddr_in local_addr;              /*本地地址*/
   
        int fd = socket(AF_INET, SOCK_DGRAM, 0);     /*建立套接字*/
        if (fd == -1)
        {
            perror("socket()");
            exit(1);
        }  
        
        int yes = 1;
        if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0)     
        {    
            perror("Reusing ADDR failed");    
            exit(1);    
        }
    
        /*初始化本地地址*/
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_family = AF_INET;
        local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        local_addr.sin_port = htons(MCAST_PORT);
    
        /*绑定socket*/
        int err = bind(fd,(struct sockaddr*)&local_addr, sizeof(local_addr)) ;
        if(err < 0)
        {
            perror("bind()");
            exit(1);
        }
    
        /*设置回环许可*/
        int loop = 1;
        err = setsockopt(fd,IPPROTO_IP, IP_MULTICAST_LOOP,&loop, sizeof(loop));
        if(err < 0)
        {
            perror("setsockopt():IP_MULTICAST_LOOP");
            exit(1);
        }
    
        /*加入多播组*/
        struct ip_mreq mreq;                                    
        mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR); /*多播地址*/
        mreq.imr_interface.s_addr = htonl(INADDR_ANY); /*本地网络接口为默认*/
        
        /*将本机加入多播组*/
        err = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq, sizeof(mreq));
        if (err < 0)
        {
            perror("setsockopt():IP_ADD_MEMBERSHIP");
            exit(1);
        }
    
        int times = 0;
        int addr_len = sizeof(local_addr);
        char buff[BUFF_SIZE];
        int n = 0;
        
        /*循环接收多播组的消息，5次后退出*/
        while(1)
        {
            memset(buff, 0, BUFF_SIZE);                 /*清空接收缓冲区*/
            
            /*接收数据*/
            n = recvfrom(fd, buff, BUFF_SIZE, 0,(struct sockaddr*)&local_addr,&addr_len);
            if( n== -1)
            {
                perror("recvfrom()");
            }
                                                        /*打印信息*/
            printf("Recv %dst message from server:%s\n", ++times, buff);
        }
    
        /*退出多播组*/
        err = setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));
        
        close(fd);
        return 0;
    }

    struct sockaddr_in serverAddr, clientAddr;
    struct sockaddr_in si_from;
    int serverSocket = -1;
    int clientAddrLen;
    int ret;
    int i;
    char recv_buf[MAX_RECV_BUF];
//-------------------------------------------------------------------------------------
    // 创建服务端套接字
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址信息
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    fprintf(stderr, "port is %d\n", port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 绑定套接字到指定地址和端口
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);
   while (1) {
    // 接收数据
    clientAddrLen = sizeof(clientAddr);
    memset(recv_buf, 0, sizeof(recv_buf));
    ssize_t recv_size=recvfrom(serverSocket, recv_buf, MAX_RECV_BUF, 0, 
                                  (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (recv_size == -1) {
        perror("Send failed");
        exit(EXIT_FAILURE);
     }

   // printf("Client message: %s\n", recv_buf);
    // 发送回接收到的数据
    //ssize_t send_size = sendto(serverSocket, recv_buf, recv_size, 0,
      //                           (struct sockaddr *)&clientAddr, &clientAddrLen);
    //if (send_size == -1) {
    //    perror("Send failed");
    //    exit(EXIT_FAILURE);
    //}
    //printf("数据已返还客户端");
    // // 发送响应
    // // strcpy(recv_buf, "Hello, Client!");
      //if (recv_buf[0] == 'q') {
        // fprintf(stderr, "quit\n");
       //  }
 
     if (sendto(serverSocket, recv_buf, strlen(recv_buf), 0, (struct sockaddr *)&clientAddr, clientAddrLen) == -1) {
        perror("Failed to send data");
        exit(EXIT_FAILURE);
      }
      if (recv_buf[0] == 'q') {
        fprintf(stderr, "quit\n");
        break;
       }
     }
    // 关闭套接字
    close(serverSocket);

    return 0;    
//--------------------------------------------------------------------------------
}
