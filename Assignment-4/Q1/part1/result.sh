#! /bin/bash

echo 1024 > /sys/fs/cgroup/cpu/question1/cpu.shares
echo "period = 1000000"
echo 1000000 > /sys/fs/cgroup/cpu/question1/cpu.cfs_period_us

g++ cpu_intensive.cc -o cpu_intensive.o -lpthread

for i in {10..2}
do
    val=$(($i*100000))
    echo "Running quota = $val"
    echo $val > /sys/fs/cgroup/cpu/question1/cpu.cfs_quota_us
    ./cpu_intensive.o
    cat /sys/fs/cgroup/cpu/question1/cpu.stat
done