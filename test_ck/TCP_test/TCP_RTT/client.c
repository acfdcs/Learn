

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
// #include "get_clock.h"

//extern double get_cpu_mhz(int);

//参数定义
#define PORT 8888
#define BUFFER_SIZE 1024
#define DATA_SIZE 1024
#define HOST_IP "192.168.161.130"
#define INTER 1000


int getMax(int arr[], int size) {
    int max = arr[0]; // 假设第一个元素为最大值

    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}


int main(int argc,char **argv) {
    int iter=INTER;
    int client_fd;
    struct sockaddr_in server_addr;
    struct timeval start_time, end_time;
    long long start_usec, end_usec, elapsed_usec;
    long long  lat[iter],latency[iter],n=0,latt[iter];
    //int lat[inter],latency[inter];
    int s_count=0,r_count=0;
    int Data_size=DATA_SIZE;
    int buffer_size=BUFFER_SIZE;
    int port=PORT;
    char *host_ip=HOST_IP;
    
    
    int opt;
    while ((opt = getopt(argc, argv, "p:s:l:c:i:")) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                fprintf(stderr,"port: %d \n", port);
                break;
            case 's':
                buffer_size = atoi(optarg);
                fprintf(stderr,"BUFFER_SIZE: %d \n", buffer_size);
                break;
            case 'l' :
                Data_size = atoi(optarg);
                fprintf(stderr,"DATA_SIZE: %d \n", Data_size);
                break;
            case 'c':
                 iter = atoi(optarg);
                 fprintf(stderr,"iteration: %d \n",iter);
                 break;
            case 'i' :
                 host_ip=optarg;
                 fprintf(stderr,"IP: %s \n", host_ip);
                 break;
            default:
                fprintf(stderr, "Usage: %s [-p port] [-s Buffer_size] [-l Data_size] [-t iteration] [-i host_ip] \n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    // 创建套接字
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // Port number
    if (inet_pton(AF_INET, host_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // 建立连接
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // 创建随机数
    //int data_size =  DATA_SIZE;
    //char send_data[data_size];
    char *send_data = (char *)malloc(Data_size);
    for (int i = 0; i < Data_size- 1; i++) {
        send_data[i] = 'A' + rand() % 26; // Generating random alphabets
    }
    send_data[Data_size - 1] = '\0';   
    float sum_data=0;
    float sum_time=0;

    
    for (int j = 0;j < iter;j++){  
        //记录开始时间
        gettimeofday(&start_time, NULL);
        start_usec = start_time.tv_sec * 1000000LL + start_time.tv_usec;
        lat[j]=start_usec;
        // cycles_to_units = get_cpu_mhz(0);
	    // test_sample_time = get_cycles();
        //lat[j]=test_sample_time;
        //printf("lat[%d]: %.lld microseconds\n",j,lat[j]);
        //发送数据
        int k = send(client_fd, send_data, strlen(send_data), 0);
        if (k == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }
        else{
            s_count++;
        }
        // //记录结束时间
        // gettimeofday(&end_time, NULL);
        // end_usec = end_time.tv_sec * 1000000LL + end_time.tv_usec;
        

        //计算经过的时间
        //elapsed_usec = end_usec - start_usec;
        //printf("发送所使用的时间: %lld microseconds\n", elapsed_usec);
        //总时间
        //sum_time=sum_time+elapsed_usec;
        //总数据量
        // sum_data=sum_data+k;
        // //计算带宽,并将单位换算为MB、s
        // float bw=(k/(1024.0*1024.0))/(elapsed_usec*(powf(10,-6)));
        // printf("带宽：%.10f MB/s\n",bw);
        
        //printf("发送数据: %s\n", send_data);
        // 从服务端接受数据
        char *back_data = (char *)malloc(buffer_size);
        //char back_data[BUFFER_SIZE];
        int recv_size = recv(client_fd, back_data, buffer_size, 0);
        if (recv_size == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }
        else{
            r_count++;

        }
        // //记录结束时间
        // gettimeofday(&end_time, NULL);
        // end_usec = end_time.tv_sec * 1000000LL + end_time.tv_usec;
        // elapsed_usec = end_usec - start_usec;
        // printf("发送所使用的时间: %lld microseconds\n", elapsed_usec);
        // lat[j]=elapsed_usec;

        // n += lat[j];
        // printf("lat[ %d]: %.lld microseconds\n",j,lat[j]);

        
        //计算经过的时间
        //elapsed_usec = end_usec - start_usec;
        //printf("发送所使用的时间: %lld microseconds\n", elapsed_usec);
        //printf("接受数据：%s\n",back_data);
        // 将接收到的数据与原始数据进行比较
    //     if (strncmp(send_data, back_data, recv_size) == 0) {
    //   ;
    //     }
    free(back_data);
    } 
    free(send_data);

    memset(&latency,0,sizeof(latency));
    for(int i=0;i<(s_count-4);i++) {
        
        latency[i]=(lat[i+3]-lat[i+2])/2;

        //printf("latency[%d]: %.lld microseconds\n",i,latency[i]);
        n += latency[i];

        // int min = findMin(latency,0,90);
        // printf("fMin lat为:%d\n", min);
         
    }
    int min=latency[0];
    for(int j=0; j<s_count-4;j++)
    {
        if(min>latency[j]){
          min=latency[j];
        }
    }
    // printf("fMin lat为:%d\n", getMin(latency,2));
    // int min = getMin(latency,2);
    printf("Min lat为:%d usec\n", min);
    //printf("n:%d\n",n);
    int size = sizeof(latency) / sizeof(latency[0]);
    //printf("%d\n",size);

    int max = getMax(latency, size);
    printf("Max lat为:%d usec\n", max);
    int avg = n/(iter-4);
    printf("Avg lat为:%d usec\n", avg);
    
    close(client_fd);
    return 0;
}
