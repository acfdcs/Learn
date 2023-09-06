#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8888
#define BUFFER_SIZE 1024
#define ITERATION 100

int main(int argc, char **argv) {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(server_addr);
    int port = PORT; // 默认端口号
    int Buffer_size = BUFFER_SIZE; // 默认缓冲区大小
    int iter=ITERATION;
  
    // 处理命令行参数
    int opt;
    while ((opt = getopt(argc, argv, "p:s:c:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                fprintf(stderr,"port: %d \n",port);
                break;
            case 's':
                Buffer_size = atoi(optarg);
                fprintf(stderr,"Buffer_size: %d \n",Buffer_size);
                break;
            case 'c':
                 iter = atoi(optarg);
                 fprintf(stderr,"iteration: %d \n",iter);
                 break;
            default:
                fprintf(stderr, "Usage: %s [-p port] [-s Buffer_size] [-c iteration] \n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    
    // 创建套接字

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, addrlen);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port); // 端口号

    // 绑定IP和端口号
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 监听
    if (listen(server_fd, 5) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("服务端启动，等待客户端连接\n");

    // Accept incoming connection
    new_socket = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
    if (new_socket == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("客户端连接成功，准备接受数据\n");
   
    // 接受数据
    int buffer_size = Buffer_size;
    char buffer[buffer_size];
    //char *recv_data = (char *)malloc(Buffer_size);
    for(int j=0;j<iter;j++)
    {
        // char *memptr;
        // int ret;
        // ret = posix_memalign (&memptr, 8, Buffer_size);
        //char *recv_data = (char *)malloc(Buffer_size);
        int recv_size = recv(new_socket, buffer, buffer_size, 0);

        if (recv_size == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        // 将接收到的数据返回给客户端
        if (send(new_socket, buffer, recv_size, 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
    }
        // memset(recv_data, 0, Buffer_size);
        //free(recv_data);

    printf("将接收到的数据返回给客户端\n");
  
    }
   //关闭连接
   //close(new_socket);
   // close(server_fd);
    // free(recv_data);
    return 0;
}
