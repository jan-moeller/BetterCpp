#include<betterc++/betterc++.hpp>

def start() {
    var myThings = {"phone", 300, new({ "category", "games" }, { "name", "foobar" }), {1, 2, 3}};
    println(myThings);

    myThings += none;
    println(myThings);

    myThings -= 0;
    println(myThings);

    myThings -= none;
    println(myThings);

    return 0;
}

EOP