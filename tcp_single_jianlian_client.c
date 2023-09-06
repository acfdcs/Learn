/* File Name: client.c */
 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <sys/resource.h>
 
#define MAXLINE 4096
int main(int argc, char** argv)
{
    int sockfd[1000], n,rec_len;
    char    recvline[4096], sendline[4096];
    char    buf[MAXLINE];
    struct sockaddr_in    servaddr;
    struct rusage r_usage;

    if( argc != 2){
    printf("usage: ./client <ipaddress>\n");
    exit(0);
    }

      // 获取程序的资源使用情况----------------------------------------------
    FILE* fp = fopen("/proc/meminfo", "r");
    char line1[128];int start;
    while (fgets(line1, 128, fp) != NULL)
    {
        if (strncmp(line1, "MemFree:", 8) == 0)
        {
            //printf("当前进程占用内存大小为：%d KB\n", atoi(line1 + 8));
            start=atoi(line1 + 8);
            break;
        }
    }
    //fclose(fp);   

    // 循环建立 1000 条 TCP 连接
    for (int i = 0; i < 1000; i++) {
        // 创建套接字
        if ((sockfd[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("socket error");
            exit(1);
        }

        // 设置服务器地址结构体
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(8000);
        if( inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0){
        printf("inet_pton error for %s\n",argv[1]);
        exit(0);
        }

        // 连接服务器
        if (connect(sockfd[i], (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
            perror("connect error");
            exit(1);
        }
    }
 
    sleep(1);
      // 获取程序情况----------------------------------------------
    FILE* fp1 = fopen("/proc/meminfo", "r");
    char line2[128];int end,cha;
    while (fgets(line2, 128, fp1) != NULL)
    {
        if (strncmp(line2, "MemFree:", 8) == 0)
        {
            //printf("当前进程占用内存大小为：%d KB\n", atoi(line2 + 8));
            end=atoi(line2 + 8);
            cha=start-end;
            printf("当前TCP建立连接成功\n");
            printf("当前TCP建立连接占用内存大小为：%d KB\n", cha/1000);

            break;
        }
    }
    fclose(fp1);
    close(sockfd);
    exit(0);
}
