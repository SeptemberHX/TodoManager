enable_testing()

set(
    test_files
        test_main.cpp
    TestDataCenter.cpp
    TestCore.cpp
)

add_executable(test_main
        ${test_files})

add_test(test_main
        test_main)

target_link_libraries(test_main gtest core utils data)