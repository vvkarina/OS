add_executable(lab6 main.cpp include/struct_server.h include/server.h)
add_executable(node src/node.cpp include/struct_server.h)
add_library(server src/server.cpp include/server.h)
target_include_directories(lab6 PRIVATE include)
target_include_directories(node PRIVATE include)
target_include_directories(server PRIVATE include)


target_link_libraries(server zmq )
target_link_libraries(lab6 zmq server)
target_link_libraries(node zmq server)