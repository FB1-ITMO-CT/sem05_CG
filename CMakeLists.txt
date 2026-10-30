cmake_minimum_required(VERSION 3.16)
project(itmo-gl-hw-2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets)
    set(QT_OPENGL_TARGET Qt6::OpenGLWidgets)
else()
    find_package(Qt5 REQUIRED COMPONENTS Widgets OpenGL)
    set(QT_OPENGL_TARGET Qt5::OpenGL)
endif()

set(PROJECT_SOURCES
        main.cpp
        window.cpp
        window.h
        textures/cat.jpg
)

add_executable(itmo-gl-hw-2026
    ${PROJECT_SOURCES}
    resources.qrc
    shaders/template.fs
    shaders/template.vs
    openglwidget.h openglwidget.cpp
)

target_link_libraries(itmo-gl-hw-2026 PRIVATE Qt::Widgets ${QT_OPENGL_TARGET})
