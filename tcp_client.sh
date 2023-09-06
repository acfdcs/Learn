#!/bin/bash

# 获取输入参数
ip="$2"
time="$6"
size="$8"


echo "TCP单流带宽速率测试"  
# 测试TCP单流带宽速率
iperf -c "$ip"  -i 1 -t "$time" -l "$size" | tee tcp_single_stream.txt
echo "TCP单流带宽速率测试"  >>  tcp_single_stream.txt

echo "TCP四流带宽速率测试"
# 测试TCP四流带宽速率
iperf -c "$ip" -i 1 -t "$time" -l "$size" -P 4  | tee tcp_four_streams.txt
echo "TCP四流带宽速率测试" >> tcp_four_streams.txt

echo "TCP支持全双工通信"
iperf -c "$ip" -i 1 -t "$time" -l "$size" --full-duplex | tee tcp_Full_duplex.txt
echo "TCP支持全双工通信" >> tcp_Full_duplex.txt



# 将结果合并到一个文件中
cat tcp_single_stream.txt tcp_four_streams.txt  tcp_Full_duplex.txt  > tcp_results.txt

# example
# ./tcp_client.sh -c 100.1.1.30 -i 1 -t  10 -l 16384


