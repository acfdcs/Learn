#!/bin/bash
echo $(date) 
cpu_usage=$(top -b -n1 | grep "Cpu(s)" | awk '{print 100-$8}') 
# 输出CPU使用率
echo "CPU使用率： $cpu_usage%" 
# 每隔2秒执行一次
while true; do
  sleep 1
  # 重新获取CPU使用率并更新输出
  echo $(date)
  cpu_usage=$(top -b -n1 | grep "Cpu(s)" | awk '{print 100-$8}') 
  echo "CPU使用率： $cpu_usage%" 
done