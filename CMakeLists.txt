project(groboy)
cmake_minimum_required(VERSION 2.8.12)

find_package(Qt5Core)
find_package(Qt5Widgets)

file(GLOB SOURCES src/*.cpp src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} src/cmd/main.cpp)
set(CMAKE_CXX_FLAGS "-std=c++11")

if(${Qt5Core_FOUND} AND ${Qt5Widgets_FOUND})
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(GUI_PROJECT_NAME "${PROJECT_NAME}_gui")
    file(GLOB GUI_SOURCES src/gui/*.cpp src/gui/*.h)
    file(GLOB GUI_UIS src/gui/*.ui)
    qt5_wrap_ui(UIS_HEADERS ${GUI_UIS})
    include_directories(${Qt5Core_INCLUDE_DIR} ${Qt5Widgets_INCLUDE_DIR})
    add_executable(${GUI_PROJECT_NAME} ${SOURCES} ${GUI_SOURCES} ${UIS_HEADERS})
    target_link_libraries(${GUI_PROJECT_NAME} Qt5::Core Qt5::Widgets)
else()
    message(WARNING "Qt not found, groboy_gui will not be compiled")
endif()


