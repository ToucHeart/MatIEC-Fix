#!/bin/bash

# 定义根目录
ROOT_DIR="PLCOPEN"

# 定义输出目录
OUTPUT_DIR="testoutput"

# 初始化计数器
success_count=0
failure_count=0

# 递归遍历所有 txt 文件并运行 iec2c 命令
for file in $(find "$ROOT_DIR" -type f -name "*.txt"); do
    ./iec2c "$file" -T "$OUTPUT_DIR";
    if [ $? -eq 0 ];then
        ((success_count++))
    else
        ((failure_count++))
    fi
done

# 等待所有子进程完成
wait

# 输出统计结果
echo "测试完成。"
echo "成功的用例数目: $success_count"
echo "失败的用例数目: $failure_count"
