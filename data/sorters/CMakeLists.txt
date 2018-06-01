find_package(Qt5 REQUIRED COMPONENTS Core Gui)

set(SOURCES
    AbstractSorter.cpp
)

set(HEADERS
    AbstractSorter.h
)

add_library(sorters ${SOURCES} ${HEADERS})

target_link_libraries(sorters core Qt5::Core Qt5::Gui)