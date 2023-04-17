add_executable(ex2_imaging_gl)

target_sources(ex2_imaging_gl
	PRIVATE
	ex2_imaging_gl.cpp
	)

target_link_libraries(ex2_imaging_gl
	PRIVATE
	usd
	usdGeom
	usdImaging
	usdImagingGL
	Eigen3::Eigen
	glfw
	OpenGL::GL
	)

target_include_directories(ex2_imaging_gl
	PRIVATE
	${PXR_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/include
	)

