cmake_minimum_required(VERSION 3.9)
project(MPI_StateLib)

find_package(MPI REQUIRED)
set(CMAKE_CXX_STANDARD 20)

add_library(MPI_StateLib MPI_State.h)
target_link_libraries(MPI_StateLib PRIVATE MPI::MPI_C)
set_target_properties(MPI_StateLib PROPERTIES LINKER_LANGUAGE CXX)