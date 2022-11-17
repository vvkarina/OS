include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)


set(CMAKE_CXX_STANDARD 17)

# LAB 2 tests
add_executable(test_lab2 test_lab2.cpp  ../lab2/include/errorlib.cpp ../lab2/src/parent.cpp ../lab2/include/parent.h)


target_link_libraries(test_lab2 PRIVATE GTest::GTest)
#target_link_libraries(test_lab2 errorlib)
target_include_directories(test_lab2 PRIVATE ../lab2/include)

add_test(test_lab2 test_lab2)

