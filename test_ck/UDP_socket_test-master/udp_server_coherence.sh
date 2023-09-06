#!/bin/bash

# 获取输入参数
port="$1"

./socket_test_server  -p "$port" 

# example
# ./udp_server_coherence.sh 8888


