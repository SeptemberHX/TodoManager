enable_testing()

add_executable(test_core
        TestCore.cpp)

add_test(test_core
        test_core)

target_link_libraries(test_core gtest core)