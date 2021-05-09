#include <unordered_map>
#include <string>
#include <memory>

#ifndef BETTERC___OBJECT_HPP
#define BETTERC___OBJECT_HPP

class variable;

using object = std::unordered_map<std::string, variable>;

#endif //BETTERC___OBJECT_HPP
