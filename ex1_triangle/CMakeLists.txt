add_executable(ex1_triangle)

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
	${CMAKE_SOURCE_DIR}/include
	)

