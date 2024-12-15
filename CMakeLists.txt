cmake_minimum_required(VERSION 3.29)
project(Caso_Integrador_Tema_4)

set(CMAKE_CXX_STANDARD 14)

# Define el ejecutable y los archivos fuente
add_executable(Caso_Final_Integrador_Tema_4
        main.cpp
        variant/Variant.cpp
        variant/Variant.h
        tests/test.cpp
        tests/test.h
        variant/Variant.cpp
)

# Incluir json11 como dependencia
include(FetchContent)
FetchContent_Declare(
        json11
        GIT_REPOSITORY https://github.com/dropbox/json11.git
        GIT_TAG master
)
FetchContent_MakeAvailable(json11)

# Vincula la biblioteca json11 al proyecto
target_link_libraries(Caso_Final_Integrador_Tema_4 json11)
