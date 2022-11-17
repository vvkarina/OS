set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(lab2 
		main.cpp
		src/parent.cpp)
                
add_executable(child1 src/child1.cpp)
add_executable(child2 src/child2.cpp)

add_subdirectory(include)

target_link_libraries(lab2 errorlib)
target_link_libraries(child1 errorlib)
target_link_libraries(child2 errorlib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/1.txt  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/2.txt  DESTINATION ${CMAKE_CURRENT_BINARY_DIR})