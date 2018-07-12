# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set (widgets_files
	itemdetailwidget.cpp
	itemlistwidget.cpp
	todolistwidget.cpp
	tagwidget.cpp
	logwidget.cpp
    logger.cpp
	itemaddtagwidget.cpp
	taglabelwidget.cpp
	flowlayout.cpp
		ItemListItemDelegate.cpp
	FetchConfigFilePathWidget.cpp
	TagMode/TagModeWidget.cpp
	TagMode/TagModeListItemDelegate.cpp
	CalendarMode/CalendarModeWidget.cpp
	CalendarMode/CalendarCellWidget.cpp
	CalendarMode/CalendarTimeLineWidget.cpp
		CustomListView.cpp CustomListView.h)
add_library(widgets ${widgets_files})
target_link_libraries(widgets Qt5::Widgets Qt5::Core Qt5::Gui core utils data)
