find_package(Qt5 REQUIRED COMPONENTS Core Sql)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(files
    DaoFactory.cpp
    SQLDao.cpp
    AbstractDao.h
)

add_library(dao ${files})
target_link_libraries(dao Qt5::Core core Qt5::Sql config)