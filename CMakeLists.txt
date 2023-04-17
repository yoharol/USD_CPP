cmake_minimum_required(VERSION 3.21)

project(USD_CPP_CMAKE)

set(CMAKE_CXX_STANDARD 17)

get_filename_component(ABS_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)

add_definitions(-DASSETS_PATH="${ABS_SOURCE_PATH}/asset")
add_definitions(-DOUTPUT_PATH="${ABS_SOURCE_PATH}/output")

if (LINUX)
	find_package(pxr REQUIRED PATHS "/usr/local/USD")
endif()
if (APPLE)
	find_package(pxr REQUIRED PATHS "/opt/local/USD")
endif()
if (WIN32)
	find_package(pxr REQUIRED PATHS "C:\\Program Files\\USD")
endif()

find_package(Eigen3 REQUIRED NO_MODULE)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED PATHS "/opt/local")

get_property(importTargets DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
message("Imported targets: ${importTargets}")

add_subdirectory(ex1_triangle)
add_subdirectory(ex2_imaging_gl)