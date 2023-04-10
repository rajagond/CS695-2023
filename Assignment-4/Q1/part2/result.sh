#! /bin/bash

mkdir -p /sys/fs/cgroup/cpu/question1p1
mkdir -p /sys/fs/cgroup/cpu/question1p2

echo 8192 > /sys/fs/cgroup/cpu/question1p1/cpu.shares
echo "cpu_intensive1 - cgroup: question1p1 share_cg1 = 8192"
echo 1000000 > /sys/fs/cgroup/cpu/question1/cpu.cfs_period_us

echo 512 > /sys/fs/cgroup/cpu/question1p2/cpu.shares
echo "cpu_intensive1 - cgroup: question1p1 share_cg1 = 512"
echo 1000000 > /sys/fs/cgroup/cpu/question1p2/cpu.cfs_period_us

g++ cpu_intensive1.cc -o cpu_intensive1.o
g++ cpu_intensive2.cc -o cpu_intensive2.o

for i in {10..2}
do
    val=$(($i*100000))
    echo "Running quota = $val" >> result1.txt
    echo "Running quota = $val" >> result2.txt
    echo $val > /sys/fs/cgroup/cpu/question1p1/cpu.cfs_quota_us
    echo $val > /sys/fs/cgroup/cpu/question1p2/cpu.cfs_quota_us
    ./cpu_intensive1.o >>result1.txt
    ./cpu_intensive2.o >>result2.txt 
    wait 
    wait
    cat /sys/fs/cgroup/cpu/question1/cpu.stat >> result1.txt
    cat /sys/fs/cgroup/cpu/question1p2/cpu.stat >> result2.txt
done