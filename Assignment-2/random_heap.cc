#include <iostream>
#include <unistd.h>

int main() {
    // Allocate a large block of memory on the heap
    char *large_block = new char[1000000000];

    // Print the process ID and memory usage
    std::cout << "PID: " << getpid() << std::endl;
    std::cout << "Memory usage: " << sysconf(_SC_PAGESIZE) * sysconf(_SC_PHYS_PAGES) / (1024.0 * 1024.0 * 1024.0) << std::endl;
    std::cout << "Virtual address of allocated memory: " << static_cast<void*>(large_block) << std::endl;
    std::cout << "Press Enter to access memory" << std::endl;
    std::cin.ignore();

    // Access the memory
    large_block[0] = 'a';
    large_block[999999999] = 'b';
    large_block[500000000] = 'c';
    large_block[250000000] = 'd';
    large_block[750000000] = 'e';
    large_block[200000] = 'f';
    large_block[999999800] = 'g';
    large_block[500000200] = 'h';
    large_block[250000200] = 'i';
    large_block[750000200] = 'j';
    large_block[200200] = 'k';

    std::cout << "Press Enter to release memory" << std::endl;
    std::cin.ignore();

    // Release the memory
    delete[] large_block;
    return 0;
}
