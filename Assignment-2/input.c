#include <stdio.h>

#define MAX 124567

int main() {
    int count = 0;
    float pi = 3.14159;
    char letter = 'A';
    char string[] = "student";

    // Loop constructs
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            count += i;
        } else {
            count -= i;
        }
    }

    // Bitwise operations
    int a = 5 & 3;
    int b = 5 | 3;
    int c = 5 ^ 3;
    int d = ~5;
    int e = 5 << 1;
    int f = 5 >> 1;

    return 0;
}