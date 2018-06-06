find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

add_subdirectory(filters)
add_subdirectory(sorters)

set(SOURCES
    DataCenter.cpp
)

set(HEADERS
    DataCenter.h
)

set(CMAKE_AUTOMOC ON)

add_library(data ${SOURCES} ${HEADERS})

target_link_libraries(data core filters sorters dao Qt5::Core)