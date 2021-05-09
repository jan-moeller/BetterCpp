#ifndef BETTERC___BETTERC_HPP
#define BETTERC___BETTERC_HPP

#include"betterc++/detail/variable.hpp"
#include"betterc++/detail/object.hpp"

#include <type_traits>

int get_return_code(int i) {
    return i;
}

int get_return_code(variable const &v) {
    return v.get_as<long double>();
}

#define EOP                                             \
int main() {                                            \
    auto v = start();                                   \
    return get_return_code(v);                          \
}

#define var variable
#define const const variable
#define def auto
#define none variable {}
#define new(...) make_object({__VA_ARGS__})


#endif //BETTERC___BETTERC_HPP
