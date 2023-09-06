#!/bin/bash

# 获取输入参数
port="$2"

./socket_test_server  -p "$port"

# example
# ./tcp_server_coherence.sh -p 8888


