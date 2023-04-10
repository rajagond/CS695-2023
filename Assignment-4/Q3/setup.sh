#! /bin/bash

mkdir -p root/{bin,lib,lib64}
cd root/
cp -v /bin/{ls,bash,touch,rm} ./bin
list="$(ldd /bin/bash | egrep -o '/lib.*\.[0-9]')"
for i in $list; do cp -v --parents "$i" "."; done

list="$(ldd /bin/ls | egrep -o '/lib.*\.[0-9]')"
for i in $list; do cp -v --parents "$i" "."; done

list="$(ldd /bin/touch | egrep -o '/lib.*\.[0-9]')"
for i in $list; do cp -v --parents "$i" "."; done

list="$(ldd /bin/rm | egrep -o '/lib.*\.[0-9]')"
for i in $list; do cp -v --parents "$i" "."; done

# make cgroup

mkdir -p /sys/fs/cgroup/cpu/question3