int factorial(int n) {
    if (n <= 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}

void main() {
    int num;
    int result;
    
    num = 5;
    result = factorial(num)
    
    // This is a comment
    while (num > 0) {
        num = num - 1;
    }
}