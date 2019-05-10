cmake_minimum_required(VERSION 3.10)
project(simplified_qoppa)
#set(CMAKE_CXX_FLAGS_DEBUG "-O2")
#set(CMAKE_CXX_FLAGS_RELEASE "-O2")
include_directories(dependencies/graphnestingc)
add_subdirectory(dependencies/graphnestingc)
include_directories(dependencies/CRoaring)
set(CMAKE_CXX_STANDARD 11)

add_executable       (loading  main.cpp)
target_link_libraries(loading graphserializers)

add_executable       (running nesting.cpp)
target_link_libraries(running roaring graphserializers)

add_executable(printing printing.cpp)
target_link_libraries(printing graphserializers)

add_executable(indexing indexing.cpp)