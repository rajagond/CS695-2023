#include <iostream>
#include <unistd.h>

int main() {
    // Allocate a large block of memory on the heap
    char *large_block = new char[1000000000];

    // Print the process ID and memory usage
    std::cout << "PID: " << getpid() << std::endl;
    std::cout << "Memory usage: " << sysconf(_SC_PAGESIZE) * sysconf(_SC_PHYS_PAGES) / (1024.0 * 1024.0 * 1024.0) << std::endl;
    std::cout << "Virtual address of allocated memory: " << static_cast<void*>(large_block) << std::endl;
    std::cout << "Press Enter to release memory" << std::endl;
    std::cin.ignore();

    // Release the memory
    delete[] large_block;
    return 0;
}