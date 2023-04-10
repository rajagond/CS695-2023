# CS 695 - Assignment #4 - just group it

All the code, data, automation and graph script and question wise README are in respective subdirectory.

Problem statement can be found [here](https://www.cse.iitb.ac.in/~puru/courses/spring22/assignments.html)

- Q1: seeing cgrougs - [README](Q1/README.md)
- Q2: namespaces or spaces with names? - [README](Q2/README.md)
- Q3: 1+2 = 3 - [README](Q3/README.md)

## Some Terminology

- **Execution Time** - Time taken by a process to complete its execution
- **nr_periods** - number of period intervals (as specified in cpu.cfs_period_us) that have elapsed.
- **nr_throttled** - number of times tasks in a cgroup have been throttled (that is, not allowed to run because they have exhausted all of the available time as specified by their quota).
- **throttled_time** - the total time duration (in nanoseconds) for which tasks in a cgroup have been throttled.
- **cpu.shares**- contains an integer value that specifies a relative share of CPU time available to the tasks in a cgroup.
- **cpu.cfs_period_us** - specifies a period of time in microseconds (µs, represented here as "*`us`*") for how regularly a cgroup's access to CPU resources should be reallocated.
- **cpu.cfs_quota_us** - specifies the total amount of time in microseconds (µs, represented here as "*`us`*") for which all tasks in a cgroup can run during one period (as defined by `cpu.cfs_period_us`).

## Steps to Create a cgroup in Ubuntu 20.04 (i.e cgroup v1)

Note that cgroup v2 has different method

```shell
sudo su # root user
mkdir -p /sys/fs/cgroup/cpu/<cgroup_name>

# unshare and nsenter are command line tools to create namespace and join a namespace respectively. (question 2)
# chroot is used to change the root directory and it will limit our view to new root and its subdirectory
```

