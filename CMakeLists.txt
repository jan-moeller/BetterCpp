cmake_minimum_required(VERSION 3.19)
project(bettercpp)

set(CMAKE_CXX_STANDARD 20)

add_library(bettercpp
        include/betterc++/detail/object.hpp
        include/betterc++/detail/variable.hpp
        include/betterc++/betterc++.hpp
        )
target_include_directories(bettercpp PUBLIC include)
set_target_properties(bettercpp PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(examples)