cmake_minimum_required(VERSION 3.20)

project(OS_LABS C CXX)
set(CMAKE_C_STANDARD 11)

enable_testing()

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_subdirectory(lab2)
add_subdirectory(tests)