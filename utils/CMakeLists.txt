find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(utils_files
    ItemTagUtils.cpp
    styleutils.cpp
    itemdetailutils.cpp
)

add_library(utils ${utils_files})
target_link_libraries(utils core Qt5::Core Qt5::Widgets)