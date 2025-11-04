cmake_minimum_required(VERSION 3.20)
project(bd_explorer)

set(CMAKE_CXX_STANDARD 20)

add_executable(bd_explorer
        main.cpp
        HashMap.cpp
        BTree.cpp
        utils.cpp
)
