find_package(Qt5 REQUIRED COMPONENTS Core Gui)

set(HEADER_FILES
    TodoConfig.h
)

set(SOURCE_FILES
    TodoConfig.cpp
)

add_library(config ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(config Qt5::Core Qt5::Gui)