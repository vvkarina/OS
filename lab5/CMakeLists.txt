# Generate shared / static librarry for hello.cpp
# STATIC generates `libhello.a`
# SHARED geneartes `libhello.so`

add_library(d1_static STATIC src/first_lib.cpp include/lib.h)
add_library(d2_static STATIC src/second_lib.cpp include/lib.h)
add_library(d1_dynamic SHARED src/first_lib.cpp include/lib.h)
add_library(d2_dynamic SHARED src/second_lib.cpp include/lib.h)

target_include_directories(d1_static PRIVATE include)
target_include_directories(d2_static PRIVATE include)
target_include_directories(d1_dynamic PRIVATE include)
target_include_directories(d2_dynamic PRIVATE include)


# Genarete `main` executable from `main.cpp`
add_executable(main_static_1 main_static.cpp)
add_executable(main_static_2 main_static.cpp)
add_executable(main_dynamic main_dynamic.cpp)

target_include_directories(main_static_1 PRIVATE include)
target_include_directories(main_static_2 PRIVATE include)
target_include_directories(main_dynamic PRIVATE include)

# Link `main` program with library
target_link_libraries(main_static_1 PRIVATE d1_static)
target_link_libraries(main_static_2 PRIVATE d2_static)