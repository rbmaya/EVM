cmake_minimum_required(VERSION 3.15)
project(lab7 C)

set(CMAKE_C_STANDARD 99)

add_executable(lab7 main.c blas_invert_matrix.c)
