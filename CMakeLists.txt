cmake_minimum_required(VERSION 3.10)
project(simplified_qoppa)


#set(CMAKE_CXX_FLAGS_DEBUG "-O2")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_STANDARD 11)

add_executable(loading main.cpp serializers/commons/Serialize.cpp)

add_executable(running    nesting.cpp serializers/utils/mmapFile.cpp hashes_paper.h macros.h dependencies/CRoaring/roaring.c)

add_executable(printing printing.cpp)

add_executable(indexing indexing.cpp)