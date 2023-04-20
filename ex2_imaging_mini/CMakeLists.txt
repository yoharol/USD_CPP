add_executable(ex2_imaging_mini)

target_sources(ex2_imaging_mini
	PRIVATE
	ex2_imaging_mini.cpp
	)

target_link_libraries(ex2_imaging_mini
	PRIVATE
	usd
	usdGeom
	usdImaging
	usdImagingGL
	glfw
	OpenGL::GL
	)

target_include_directories(ex2_imaging_mini
	PRIVATE
	${PXR_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/include
	)
