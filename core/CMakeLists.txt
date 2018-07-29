find_package(Qt5 REQUIRED COMPONENTS Core Gui)

set(core_files
        ItemDetail.cpp
        ItemDetail.h
        ItemTag.cpp
        ItemTag.h
        SqlErrorException.cpp
        SqlErrorException.h
        ItemDetailAndTag.cpp
        ItemDetailAndTag.h
        ItemGroup.cpp
        ItemGroup.h
        ItemGroupRelation.cpp
        ItemGroupRelation.h
        ItemAndGroupWrapper.cpp
        ItemAndGroupWrapper.h
        ItemGroupOverview.cpp
        ItemGroupOverview.h
        ItemDetailDao.cpp ItemDetailDao.h ItemGroupDao.cpp ItemGroupDao.h)

add_library(core ${core_files})
target_link_libraries(core Qt5::Core Qt5::Gui)
