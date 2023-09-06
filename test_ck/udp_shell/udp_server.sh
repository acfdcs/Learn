#!/bin/bash

# 获取输入参数
mulip="$1"

iperf -s -u -i 1 "$mulip"

# example
# ./udp_server.sh 224.0.0.1
