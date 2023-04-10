#include <stdio.h>
#include <unistd.h>

int main() {
    // Perform CPU-intensive computation
    unsigned long long int result = 0;
    for (unsigned long long int i = 0; i < 1000000000; i++) {
        result += i;
    }

    // Print the result
    printf("This is program 2 with PID %d\n", getpid());

    return 0;
}
