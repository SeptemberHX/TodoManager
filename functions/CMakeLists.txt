find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

set(SOURCES
        TaskOnTimeNotifier.cpp
        TaskArchivingTimeRecorder.cpp
        AppSystemTrayIcon.cpp
)

set(HEADERS
        TaskOnTimeNotifier.h
        TaskArchivingTimeRecorder.h
        AppSystemTrayIcon.h)

set(CMAKE_AUTOMOC ON)

add_library(functions ${SOURCES} ${HEADERS})

target_link_libraries(functions core filters sorters dao Qt5::Core Qt5::Gui Qt5::Widgets data)