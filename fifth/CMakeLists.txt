cmake_minimum_required(VERSION 3.9)
project(CxxMPI_University_Tasks)

find_package(MPI REQUIRED)
set(CMAKE_CXX_STANDARD 20)

add_executable(CxxMPI_University_Tasks main.cpp)
target_link_libraries(CxxMPI_University_Tasks PRIVATE MPI::MPI_C)
set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")