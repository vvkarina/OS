set(CMAKE_C_STANDARD 11)

add_executable(main main.cpp include/struct_server.h include/server.h)
add_executable(node55 src/node55.cpp include/struct_server.h)


add_library(server src/server.cpp include/server.h)

target_include_directories(main PRIVATE include)
target_include_directories(node55 PRIVATE include)
target_include_directories(server PRIVATE include)


#target_link_libraries(server zmq )
target_link_libraries(main zmq server)
target_link_libraries(node55 zmq server)