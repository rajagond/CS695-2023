#include <iostream>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include <chrono>

int main() {

    // PID of the main thread
    std::cout << "PID: " << getpid() << std::endl;
    
    // Put this process into a separate cgroup already existing
    // cgroup name: question1
    // cgroup path: /sys/fs/cgroup/cpu/question1

    // write the PID of the main thread to the cgroup's tasks file
    std::ofstream tasksFile;
    tasksFile.open("/sys/fs/cgroup/cpu/question1p1/cgroup.procs");
    tasksFile << getpid();
    tasksFile.close();

    auto start = std::chrono::high_resolution_clock::now();

    double result = 0.0;
    for (long long int i = 0; i < 1000000000; i++) {
        result += std::sqrt(i);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    return 0;
}
