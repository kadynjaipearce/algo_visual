cmake_minimum_required(VERSION 4.1)
project(algo_visual)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG master
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG master
)
FetchContent_MakeAvailable(imgui)

add_executable(algo_visual main.cpp
        src/arraysort.h
        src/arraysort.cpp)

target_sources(${PROJECT_NAME} PRIVATE
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

find_package(OpenGL REQUIRED)

if(APPLE)
    find_library(CORE_VIDEO CoreVideo REQUIRED)
    find_library(IO_KIT IOKit REQUIRED)
    find_library(COCOA Cocoa REQUIRED)
    find_library(OPENGL_LIB OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_LIB} ${COCOA} ${IO_KIT} ${CORE_VIDEO})
else()
    target_link_libraries(${PROJECT_NAME} glfw opengl32)
endif()