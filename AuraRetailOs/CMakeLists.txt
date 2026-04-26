cmake_minimum_required(VERSION 3.16)
project(AuraRetailOs VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt 6 (or Qt 5 if 6 is not found)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Collect all backend source files
file(GLOB_RECURSE BACKEND_SOURCES "src/*.cpp")

# Collect GUI source files
set(GUI_SOURCES
    gui/main.cpp
    gui/mainWindow.cpp
    gui/mainWindow.h
    gui/mainWindow.ui
)

# 1. Create the CLI Executable
add_executable(AuraRetailOsCli
    main.cpp
    ${BACKEND_SOURCES}
)

# 2. Create the GUI Executable
add_executable(AuraRetailOsGui
    ${GUI_SOURCES}
    ${BACKEND_SOURCES}
)
target_link_libraries(AuraRetailOsGui PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)

# Set Windows subsystem for GUI to prevent console popping up (optional but recommended)
if(WIN32)
    set_target_properties(AuraRetailOsGui PROPERTIES WIN32_EXECUTABLE ON)
endif()
set_target_properties(AuraRetailOsCli PROPERTIES
    AUTOMOC OFF
    AUTOUIC OFF
    AUTORCC OFF
)

