enable_testing()

add_executable(test_utils
        TestItemGroupUtils.cpp)

add_test(test_utils
        test_utils)

target_link_libraries(test_utils gtest core utils)