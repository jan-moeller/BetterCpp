# BetterC++

BetterC++ is a framework that brings together the goodness of JavaScript
(doesn't complain about obvious bugs) with all the best of C++ (can put
braces on the next line)! You'll never be forced to think about boring
things like types, safety or semantics anymore. And all of this at the
negligible cost of a tiny bit of undefined behavior!

 * No more silly type checking!
 * Numbers and strings have value semantics, while everything else has
   reference semantics!
 * Arbitrary design decisions like:
    * The plus-operator:
        * on numbers: calculate the sum!
        * on strings: concatenate them!
        * on arrays: insert into the array!
        * on objects: merge two objects!
    * The minus-operator:
        * on numbers: subtract them!
        * on strings: fail miserably!
        * on arrays: remove the nth item. Or the item equals to rhs!
        * on objects: erase the nth item, even though you don't know which
                      one that is! Or the item where key equals rhs!
          
In short: BetterC++ does its best to shift nasty compile-time errors to
run-time where they should be! Simply better™!

## Instructions

Just link against the header-only library, then use the new and better™
syntax provided by BetterC++.

## Syntax

### Variables

Variables are introduced with either the `const` or `var` keyword.

    const i = 3;
    var str = "foobar";

#### Objects

Objects have to be created using the `new` function.

    var obj = new({"member1", 3}, {"member2", "str"});
    println(obj["member1"]); // prints "3"

#### Arrays

Arrays are introduced with braces.

    var arr = {"first", 2, new()};
    println(arr[0]); // prints "first"

### Functions

Functions are introduced with the keyword `def`. Why `def` and not
`function`, you ask? Well, having to type 8 letters on every function
definition has always been JavaScripts greatest weakness, so we've 
fixed it!

    def fibonacci(const num) {
        if (num == 0) return 0;
        if (num == 1) return 1;
        return fibonacci(num - 1) + fibonacci(num - 2);
    }

The main entry point is called "start" and needs to return a number.

    def start() {
        return 0;
    } EOP

Note: the `EOP` stands for "End Of Program" and needs to be added because
C++ is stupid!

## Examples

For more examples, check the examples/ subdirectory.
