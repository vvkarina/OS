add_executable(lab3 main.cpp include/lab3.h src/lab3.cpp include/utils.h)

target_include_directories(lab3 PRIVATE include)

include(CMakeFindDependencyMacro)
find_dependency(Threads)
target_link_libraries(lab3 PRIVATE Threads::Threads)