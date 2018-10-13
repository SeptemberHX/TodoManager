cmake_minimum_required(VERSION 3.10)

IF (APPLE)
    set (CMAKE_PREFIX_PATH "/usr/local/Cellar/qt/5.11.2/lib/cmake")
ENDIF()

project(TodoManager)

add_subdirectory(core)
add_subdirectory(config)
add_subdirectory(widgets)
add_subdirectory(utils)
add_subdirectory(dao)
add_subdirectory(data)
add_subdirectory(gtest)
add_subdirectory(functions)

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})


set(main_files
    main.cpp
    mainwindow.cpp
    ../resources/resource.qrc
)

# Tell CMake to create the helloworld executable
add_executable(TodoManager ${main_files})

# Use the Widgets module from Qt 5.
target_link_libraries(TodoManager Qt5::Core Qt5::Gui Qt5::Widgets core widgets utils dao data config functions)
