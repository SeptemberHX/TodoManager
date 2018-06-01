find_package(Qt5 REQUIRED COMPONENTS Core Gui)

set(SOURCES
    AbstractFilter.cpp
)

set(HEADERS
    AbstractFilter.h
        DateFilter.cpp DateFilter.h)

add_library(filters ${SOURCES} ${HEADERS})

target_link_libraries(filters core Qt5::Core Qt5::Gui)