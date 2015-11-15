project(groboy)
cmake_minimum_required(VERSION 2.8.12)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

include_directories(src/gui)
file(GLOB SOURCES src/*.cpp src/*.h src/gui/*.cpp src/gui/*.h)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB GUI_UIS src/gui/*.ui)
qt5_wrap_ui(UIS_HEADERS ${GUI_UIS})

set(RESOURCE_FILE src/gui/resources/resources.qrc)

qt5_add_resources(RESOURCES ${RESOURCE_FILE})

include_directories(./gui
    ${Qt5Core_INCLUDE_DIR}
    ${Qt5Widgets_INCLUDE_DIR}
)
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${UIS_HEADERS}
    ${RESOURCES}
)
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
)




