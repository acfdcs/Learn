#!/bin/bash

# 获取输入参数
port="$2"
size="$4"
count="$6"
ip="$8"

echo "TCP往返时延RTT测试"
./client  -p "$port" -s "$size" -c "$count" -i "$ip" | tee tcp_rtt.txt
echo "TCP往返时延RTT测试" >> tcp_rtt.txt


# example
# ./tcp_client_rtt.sh -p 1234 -s 1024 -c 20 -i 100.1.1.30

