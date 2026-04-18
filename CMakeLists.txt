cmake_minimum_required(VERSION 4.2)
project(Devoir2_StructureDeDonnees_UQAC)

set(CMAKE_CXX_STANDARD 20)

add_executable(ex1
        src/Ex1/main_ex1.cpp
)
target_compile_options(ex1 PRIVATE -Wall -Wextra)

add_executable(ex2
        src/Ex2/main_ex2.cpp
)
target_compile_options(ex2 PRIVATE -Wall -Wextra)

add_executable(ex3
        src/Ex3/main_ex3.cpp
)
target_compile_options(ex3 PRIVATE -Wall -Wextra)

add_executable(ex4
        src/Ex4/main_ex4.cpp
)
target_compile_options(ex4 PRIVATE -Wall -Wextra)
