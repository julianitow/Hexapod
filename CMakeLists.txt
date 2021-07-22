cmake_minimum_required(VERSION 3.13)

project(WebSocketServer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core LinguistTools REQUIRED)

set(TS_FILES WebSocketServer_fr_FR.ts)

add_executable(WebSocketServer
  main.cpp
  WebSocketServer.cpp
  ${TS_FILES}
)
target_include_directories(WebSocketServer PUBLIC /home/julianitow/rpi-qt/sysroot/include)
target_link_directories(WebSocketServer PUBLIC /home/julianitow/rpi-qt/sysroot/lib)
target_link_libraries(WebSocketServer Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::WebSockets )

if(COMMAND qt_create_translation)
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()
