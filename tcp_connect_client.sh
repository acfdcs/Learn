#!/bin/bash

gcc -o tcp_single_jianlian_client tcp_single_jianlian_client.c


ip="$1"

./tcp_single_jianlian_client "$ip" 

# example
#./tcp_single_jianlian_client 192.168.213.135
