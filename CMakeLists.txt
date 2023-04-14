cmake_minimum_required(VERSION 3.21)

project(USD_CPP_CMAKE)

set(CMAKE_CXX_STANDARD 17)

get_filename_component(ABS_SOURCE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)

add_definitions(-DASSETS_PATH="${ABS_SOURCE_PATH}/asset")
add_definitions(-DOUTPUT_PATH="${ABS_SOURCE_PATH}/output")

find_package(pxr REQUIRED)
find_package(Eigen3 REQUIRED NO_MODULE)

get_property(importTargets DIRECTORY "${CMAKE_SOURCE_DIR}" PROPERTY IMPORTED_TARGETS)
message("Imported targets: ${importTargets}")

add_executable(ex1_triangle include/pxgen.h)

target_sources(ex1_triangle
	PRIVATE
	ex1_triangle.cpp
	)

target_link_libraries(ex1_triangle
	PRIVATE
	usd
	usdGeom
	Eigen3::Eigen
	)

target_include_directories(ex1_triangle
	PRIVATE
	${PXR_INCLUDE_DIRS}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	)

target_precompile_headers(ex1_triangle
	PUBLIC
	include/pxr_common.h
	)