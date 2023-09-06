#!/bin/bash

# 获取输入参数
ip="$1"
time="$2"
size="$3"
mulip="$4"

echo "UDP单流带宽速率测试" 
# 测试UDP单流带宽速率
iperf -c "$ip" -i 1 -u -b 25600M -t "$time" -l "$size" | tee udp_single_stream.txt 
echo "UDP单流带宽速率测试"  >>  udp_single_stream.txt

echo "UDP四流带宽速率测试"
# 测试UDP四流带宽速率
iperf -c "$ip" -i 1 -u -b 25600M -P 4 -t "$time" -l "$size" | tee udp_four_streams.txt 
echo "UDP四流带宽速率测试"  >> udp_four_streams.txt 

echo "UDP支持单播测试"
# 测试UDP支持单播
iperf -c "$ip" -i 1 -u -t "$time" -l "$size"  | tee udp_unicast.txt
echo "UDP支持单播测试"  >>  udp_unicast.txt

echo "UDP支持组播测试"
# 测试UDP支持组播
iperf -c "$mulip" -i 1 -u -t "$time"  -l "$size" | tee udp_multicast.txt
echo "UDP支持组播测试"  >>  udp_multicast.txt

# 将结果合并到一个文件中
cat udp_single_stream.txt udp_four_streams.txt udp_unicast.txt udp_multicast.txt > udp_results.txt 

# example
# ./udp_client.sh 192.168.8.110 10 65507 224.0.0.1（除组播)
# ./udp_client.sh 224.0.0.1 10 16384 100.1.1.30(组播）


