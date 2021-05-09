#include<betterc++/betterc++.hpp>

def fibonacci(const num) {
    if (num == 0) return 0;
    if (num == 1) return 1;
    return fibonacci(num - 1) + fibonacci(num - 2);
}

def start() {
    return fibonacci(6);
}

EOP