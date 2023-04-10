#! /bin/bash

echo 1024 > /sys/fs/cgroup/cpu/question3/cpu.shares
echo "period = 1000000"
echo 1000000 > /sys/fs/cgroup/cpu/question3/cpu.cfs_period_us
touch result.txt
for i in {10..2}
do
    val=$(($i*100000))
    echo "Running quota = $val" >> result.txt
    echo $val > /sys/fs/cgroup/cpu/question3/cpu.cfs_quota_us
    time ./pid_namespaces2.o > output.txt
    cat /sys/fs/cgroup/cpu/question3/cpu.stat >> result.txt
done