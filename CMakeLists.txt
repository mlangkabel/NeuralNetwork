cmake_minimum_required(VERSION 2.8.9)

include(cmake/add_files.cmake)
include(cmake/create_source_groups.cmake)

# set Standerd build type to Release
set(CMAKE_BUILD_TYPE_INIT "Release")

# Variables --------------------------------------------------------------------

set(PROJECT_NAME NeuralNetwork)
set(APP_PROJECT_NAME "${PROJECT_NAME}")

LINK_DIRECTORIES($ENV{VLD_DIR}/lib/Win32) # this adds the library path for VLD, must be done before the project target is created

# Project ----------------------------------------------------------------------

project(${PROJECT_NAME})

# App --------------------------------------------------------------------------

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/)

FIND_PACKAGE(OpenCL REQUIRED)

add_subdirectory(src/app)

add_executable(${APP_PROJECT_NAME} ${APP_FILES})

create_source_groups(${APP_FILES})

# hide the console when running a release build.
set_target_properties(${APP_PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
set_target_properties(${APP_PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
set_target_properties(${APP_PROJECT_NAME} PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE")
set_target_properties(${APP_PROJECT_NAME} PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
set_target_properties(${APP_PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/ENTRY:\"mainCRTStartup\" /SUBSYSTEM:WINDOWS")
set_target_properties(${APP_PROJECT_NAME} PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")

message ("lolo")
message (${OPENCL_LIBRARY})
message ("lala")

TARGET_LINK_LIBRARIES(${APP_PROJECT_NAME} ${OPENCL_LIBRARY})

set_property(
	TARGET ${APP_PROJECT_NAME}
	PROPERTY INCLUDE_DIRECTORIES
		"${CMAKE_SOURCE_DIR}/src/app"
)

target_include_directories(${APP_PROJECT_NAME} SYSTEM
	PUBLIC ${OPENCL_INCLUDE_DIR}
)


# Visual Leak Detector ---------------------------------------------------------

include_directories($ENV{VLD_DIR}/include)



