find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(utils_files
    ItemTagUtils.cpp
    styleutils.cpp
    itemdetailutils.cpp
        NofityUtils.cpp NofityUtils.h StringUtils.cpp StringUtils.h DrawUtils.cpp DrawUtils.h)

set(CMAKE_AUTOMOC ON)

add_library(utils ${utils_files})
target_link_libraries(utils core Qt5::Core Qt5::Widgets dao data)