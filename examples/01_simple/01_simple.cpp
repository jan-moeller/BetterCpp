#include<betterc++/betterc++.hpp>

def min(const a, const b) {
    if (a < b) return a;
    return b;
}

def start() {
    const a = 1;
    var b = 2;
    return min(a + b, 42);
}

EOP