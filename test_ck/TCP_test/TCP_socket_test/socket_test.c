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
#define COUNT 10
enum {
    DEST_PORT = 7324,
    DEFSIZE = 67,
    DEFFREQ = 1,
};

static long n_send;

void show_usage(char *prog)
{
    fprintf(stderr, "Usage: %s [-c <count>] [-p port] [-s <size>] [-f <frequency>] [-q <cycle>] server_ip\n", prog);
    exit(1);
}
int getMax(int arr[], int size) {
    int max = arr[0]; // 假设第一个元素为最大值

    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}

int main(int argc, char **argv)
{
    char *thisarg;

    int count=COUNT;
    int size = DEFSIZE;
    int freq = DEFFREQ;
    int cycle = 0;
    unsigned int port = DEST_PORT;

    struct timeval con_start_time1, con_end_time1;
    struct timeval con_start_time2, con_end_time2;
     struct timeval start_time, end_time;

    long long start_usec, end_usec, elapsed_usec;
    long long  lat[count],latency[count],n=0;
    int socket_client = -1;
	struct sockaddr_in server_addr;
    int len = sizeof(server_addr);
    int ret;
    char **save_argv = argv;
    int s_count=0,r_count=0;

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
            if (count < 1)
                count = 1;
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
        case 'f':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            freq = atoi(*argv);
            break;
        case 'q':
            if (--argc <= 0)
                show_usage(save_argv[0]);
            argv++;
            cycle = atoi(*argv);
            break;;
        default:
            show_usage(save_argv[0]);
        }
        argc--;
        argv++;
    }
    if (argc < 1)
        show_usage(save_argv[0]);
    char *serverip = *argv;

    //fclose(fp);   
    memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);          
    server_addr.sin_addr.s_addr = inet_addr(serverip);// serverip
    socket_client = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_client == -1) {
        perror("socket");
        exit(-1);
    }

    gettimeofday(&con_start_time1, NULL);
    if ((connect(socket_client, (struct sockaddr *)&server_addr, len)) == -1) {
        perror("connect");
        exit(-1);
    }
    gettimeofday(&con_end_time1, NULL);

    long long start_usec1 = con_start_time1.tv_sec * 1000000 + con_start_time1.tv_usec;
    long long end_usec1 = con_end_time1.tv_sec * 1000000 + con_end_time1.tv_usec;
    int duration1 = (int)(end_usec1 - start_usec1);  // 计算传输时间（微秒）
    printf("建链时间: %d microsecond\n", duration1);
    /*
     * 填充缓存区
     */
    char *tmp_buf;
    tmp_buf = (char *)malloc(size);
    int c =0;
    int ret_sum = 0 ;
    double duration2_sum=0.0;

    while (1) {
    
         //
         for (int j = 0; j < size-1; j++) {
	    tmp_buf[j] = 'A' + rand() % 26; 
	   } 
	   c++;
	 
	 
	
	  if (count > 0 && n_send == count - 1){
	      //printf("%d\n",n_send==count-1);
               tmp_buf[0] = 'q';
               
           }
    


        gettimeofday(&con_start_time2, NULL);
        
        //发送数据
        ret = send(socket_client, tmp_buf, size, MSG_DONTWAIT);
        if (ret == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        else{
            s_count++;
           
        }
 
        //从服务端接受数据
        char *recv_data;
        recv_data = (char *)malloc(size);
        int recv_size = recv(socket_client, recv_data, size, 0);
 
        if (recv_size == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }else{
            r_count++;
            
        }
        //将接收到的数据与发送的数据比较
         if (strncmp(tmp_buf, recv_data, size) == 0) {
          printf("第[%d]数据一致\n",c-1);
         } else {
          printf("第[%d]数据不一致\n",c-1);
        }
       
        if (ret < 0) {
         perror("send");
        }
        //printf("[%d]n_send: %d\n",c-1,n_send);
        n_send++;
        if (count > 0 && n_send >= count)
            break;

        int i;
        if (cycle)
             usleep(1000 * 1000 / freq);
             usleep(cycle);
    }

    close(socket_client);
    return 0;
}
