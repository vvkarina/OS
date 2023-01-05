add_executable(buddy src/buddy_test.cpp include/buddy_allocator.h)
add_executable(block src/block_test.cpp include/block_allocator.h)

target_include_directories(buddy PRIVATE include)
target_include_directories(block PRIVATE include)
