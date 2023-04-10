#! /bin/bash

# This script compiles all the files in the current directory and program directory

# Compile all the files in the current directory
gcc -Wall -o pid_namespaces.o pid_namespaces.c
gcc -Wall -o pid_namespaces2.o pid_namespaces_2.c


# Compile all the files in the program directory
for file in ./root/program/*.c
do
    chmod 777 $file
    gcc -Wall -o ${file%.c}.o $file
    chmod 777 ${file%.c}.o
done