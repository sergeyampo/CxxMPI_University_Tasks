cmake_minimum_required(VERSION 3.9)
project(CxxMPI_University_Tasks_Second)

find_package(MPI REQUIRED)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

add_executable(CxxMPI_University_Tasks_Second main.cpp)

target_link_libraries(CxxMPI_University_Tasks_Second PRIVATE MPI::MPI_C MPI_StateLib)

set(CMAKE_CXX_COMPILER "/usr/bin/mpic++")