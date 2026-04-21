cmake_minimum_required(VERSION 4.2)
project(Devoir2_StructureDeDonnees_UQAC)

set(CMAKE_CXX_STANDARD 20)

#file(COPY data/ DESTINATION ${CMAKE_BINARY_DIR}/data)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data SYMBOLIC)

add_executable(ex1
        src/Ex1/main_ex1.cpp
        src/SDA/Kd-Tree/Kd-Tree.cpp
)
target_include_directories(ex1 PRIVATE src/SDA/Kd-Tree)
target_compile_options(ex1 PRIVATE -Wall -Wextra)

add_executable(ex2
        src/Ex2/main_ex2.cpp
        src/SDA/Hypergraphe/Hypergraphe.cpp
        src/SDA/Hypergraphe/Hypergraphe.h
)
target_include_directories(ex2 PRIVATE src)
target_compile_options(ex2 PRIVATE -Wall -Wextra)

add_executable(ex3
        src/Ex3/main_ex3.cpp
        src/SDA/Labyrinthe/labyrinth.cpp
)
target_include_directories(ex3 PRIVATE src/SDA/Labyrinthe)
target_compile_options(ex3 PRIVATE -Wall -Wextra)

add_executable(ex4
        src/Ex4/main_ex4.cpp
        src/SDA/CompressionImages/image.cpp
)
target_include_directories(ex4 PRIVATE src/SDA/CompressionImages)
target_include_directories(ex4 PRIVATE src/SDA/Labyrinthe) # use of class matrix from ex3
target_compile_options(ex4 PRIVATE -Wno-missing-field-initializers) # mainly ignore all the stb errors
target_compile_options(ex4 PRIVATE -Wall -Wextra)
