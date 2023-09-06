#!/bin/bash

# 获取输入参数

port="$2"
size="$4"
count="$6"
ip="$7"

echo "TCP建链时间与TCP报文一致性测试"
./socket_test -p "$port" -s "$size" -c "$count" "$ip" | tee tcp_connect_coherence.txt
echo "TCP建链时间与TCP报文一致性测试" >> tcp_connect_coherence.txt

# example
# ./tcp_client_coherence.sh -p 8888 -s 16384 -c 10 192.168.213.135


