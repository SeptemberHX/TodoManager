add_subdirectory(filters)
add_subdirectory(sorters)

set(SOURCES
    DataCenter.cpp
)

set(HEADERS
    DataCenter.h
)

add_library(data ${SOURCES} ${HEADERS})

target_link_libraries(data core filters sorters dao)