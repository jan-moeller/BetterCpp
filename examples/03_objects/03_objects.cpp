#include<betterc++/betterc++.hpp>

def getYounger(const person1, const person2) {
    if (person1["age"] < person2["age"])
        return person1;
    return person2;
}

def printHobby(const person) {
    println(person["name"] + " (" + person["age"] + ") has hobby: " + person["hobby"]);
}

def start() {
    var luke = new({ "name", "luke" }, { "age", 20 });
    var bob = new({ "name", "bob" }, { "age", 21 });

    bob["hobby"] = "spreading misery";

    printHobby(getYounger(luke, bob));
    println(luke);

    println(bob);
    bob -= "hobby";
    println(bob);

    return 0;
}

EOP