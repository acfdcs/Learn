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

#ifndef MAX_RECV_BUF
#define MAX_RECV_BUF    2621440
#endif

enum {
    DEST_PORT = 8888,
};

static long count;

void show_usage(char *prog)
{
    fprintf(stderr, "Usage: %s [-p port]\n", prog);
    exit(1);
}

int main(int argc, char **argv)
{
    uint32_t port = DEST_PORT;
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
        default:
            show_usage(save_argv[0]);
        }
        argc--;
        argv++;
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int server_socket = -1, client_socket=-1;
    //int client_len;
    socklen_t client_len = sizeof(client_addr);
    int ret;
    int i;
    char recv_buf[MAX_RECV_BUF];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("socket");
        exit(errno);
    }

    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    ret = bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret == -1) {
        perror("bind");
        exit(errno);
    }

    // 开始监听连接
    if (listen(server_socket, 2) < 0) {
        perror("Error in listening");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d\n", port);

    //接受客户端连接
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Error in accepting client connection");
        exit(EXIT_FAILURE);
    }

    printf("Accepted new client connection\n");

    while (1) {   
        //接受 数据
        ret= recv(client_socket, recv_buf,MAX_RECV_BUF, 0);
        if (ret == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
           
        // 将接收到的数据返回给客户端

        if (send(client_socket, recv_buf, strlen(recv_buf), 0) == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
         printf("将接收到的数据返回给客户端\n");
         if (recv_buf[0] == 'q') {
         	fprintf(stderr, "quit\n");
         	break;
         }
        
    }
    close(client_socket);
    close(server_socket);
    return 0;
}
