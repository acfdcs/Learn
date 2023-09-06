#!/bin/bash

# 获取输入参数

port="$1"
size="$2"
count="$3"
ip="$4"

echo "UDP报文一致性测试"
./socket_test  -p "$port" -s "$size" -c "$count" "$ip" | tee udp_coherence.txt
echo "UDP报文一致性测试" >> udp_coherence.txt


# example
# ./udp_client_coherence.sh 8888 16384 10 192.168.213.135


