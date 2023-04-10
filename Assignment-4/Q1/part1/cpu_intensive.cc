#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <chrono>

int main() {
    // Number of threads to create
    int numThreads = 8;
    std::vector<std::thread> threads;

    // PID of the main thread
    std::cout << "PID: " << getpid() << std::endl;
    
    // Put this process into a separate cgroup already existing
    // cgroup name: question1
    // cgroup path: /sys/fs/cgroup/cpu/question1

    // write the PID of the main thread to the cgroup's tasks file
    std::ofstream tasksFile;
    tasksFile.open("/sys/fs/cgroup/cpu/question1/cgroup.procs");
    tasksFile << getpid();
    tasksFile.close();

    auto start = std::chrono::high_resolution_clock::now();

    // lambda function of cpuIntensiveComputation
    auto cpuIntensiveComputation = []() {
        double result = 0.0;
        for (int i = 0; i < 100000000; i++) {
            result += std::sqrt(i);
        }
        //std::cout << "Thread ID: " << std::this_thread::get_id() << " Result: " << result << std::endl;
    };

    // lambda expression to create and start the threads
    auto createAndStartThreads = [&]() {
        for (int i = 0; i < numThreads; i++) {
            threads.push_back(std::thread(cpuIntensiveComputation));
        }
    };

    // lambda expression to wait for all threads to finish
    auto waitForAllThreads = [&]() {
        for (auto& thread : threads) {
            thread.join();
        }
    };

    // call the lambda expressions
    createAndStartThreads();
    waitForAllThreads();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;

    return 0;
}
