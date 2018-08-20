find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(SOURCES

)

set(HEADERS
        TaskOnTimeNotifier.cpp TaskOnTimeNotifier.h TaskArchivingTimeRecorder.cpp TaskArchivingTimeRecorder.h)

set(CMAKE_AUTOMOC ON)

add_library(functions ${SOURCES} ${HEADERS})

target_link_libraries(functions core filters sorters dao Qt5::Core data)