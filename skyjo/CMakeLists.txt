cmake_minimum_required(VERSION 3.16)
project(JeuDeCartes LANGUAGES CXX)

# Configuration C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fichiers sources
set(SOURCES
    carte.cc
    jeuconsole.cc
    planche.cc
    main.cc
)

# Exécutable principal
add_executable(JeuDeCartes ${SOURCES})

# Options de compilation (ajustez selon vos besoins)
target_compile_options(JeuDeCartes PRIVATE -Wall -Wextra -pedantic)
