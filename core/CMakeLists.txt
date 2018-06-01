find_package(Qt5 REQUIRED COMPONENTS Core Gui)

add_subdirectory(gtest)

set(core_files
        ItemDetail.cpp ItemDetail.h ItemTag.cpp ItemTag.h SqlErrorException.cpp SqlErrorException.h ItemDetailAndTag.cpp ItemDetailAndTag.h)

add_library(core ${core_files})
target_link_libraries(core Qt5::Core Qt5::Gui)
