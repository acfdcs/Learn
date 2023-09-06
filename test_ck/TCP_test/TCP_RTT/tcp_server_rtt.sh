#!/bin/bash

# 获取输入参数
port="$2"
size="$4"
count="$6"
#ip="$8"

echo "TCP往返时延RTT测试"
./server  -p "$port" -s "$size" -c "$count"  
#echo "TCP往返时延RTT测试" >> tcp_rtt.txt


# example
# ./tcp_server_rtt.sh -p 1234 -s 1024 -c 20 

