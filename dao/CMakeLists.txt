find_package(Qt5 REQUIRED COMPONENTS Core Sql)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(files
    AbstractDao.cpp
    DaoFactory.cpp
        SQLDao.cpp
)

add_library(dao ${files})
target_link_libraries(dao Qt5::Core core Qt5::Sql config)