cmake_minimum_required(VERSION 2.8.9)

include(cmake/add_files.cmake)
include(cmake/create_source_groups.cmake)

# set Standerd build type to Release
set(CMAKE_BUILD_TYPE_INIT "Release")


# Variables --------------------------------------------------------------------

set(PROJECT_NAME NeuralNetwork)
set(APP_PROJECT_NAME "${PROJECT_NAME}App")
set(LIB_PROJECT_NAME "${PROJECT_NAME}Lib")
set(TEST_PROJECT_NAME "${PROJECT_NAME}Test")

LINK_DIRECTORIES($ENV{VLD_DIR}/lib/Win32) # this adds the library path for VLD, must be done before the project target is created


# Project ----------------------------------------------------------------------

project(${PROJECT_NAME})

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)


# Lib --------------------------------------------------------------------------

FIND_PACKAGE(OpenCL REQUIRED)

add_subdirectory(src/lib)

add_library(${LIB_PROJECT_NAME} ${LIB_FILES})

create_source_groups(${LIB_FILES})

TARGET_LINK_LIBRARIES(${LIB_PROJECT_NAME} ${OPENCL_LIBRARY})

set_property(
	TARGET ${LIB_PROJECT_NAME}
	PROPERTY INCLUDE_DIRECTORIES
		"${CMAKE_SOURCE_DIR}/src/lib"
)

target_include_directories(${LIB_PROJECT_NAME} SYSTEM
	PUBLIC ${OPENCL_INCLUDE_DIR}
)


# App --------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/app)

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

TARGET_LINK_LIBRARIES(${APP_PROJECT_NAME} ${LIB_PROJECT_NAME})

set_property(
	TARGET ${APP_PROJECT_NAME}
	PROPERTY INCLUDE_DIRECTORIES
		"${CMAKE_SOURCE_DIR}/src/app"
		"${CMAKE_SOURCE_DIR}/src/lib"
)


# Test -------------------------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/test)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test_main.cpp "")

add_subdirectory(src/test)

set(CXXTEST_INCLUDE_DIR $ENV{CXX_TEST_DIR})
set(CXXTEST_PYTHON_TESTGEN_EXECUTABLE ${CXXTEST_INCLUDE_DIR}/bin/cxxtestgen)
set(TESTGEN_FILE ${CMAKE_CURRENT_BINARY_DIR}/test_main.cpp)

find_package(PythonInterp REQUIRED)

add_executable (${TEST_PROJECT_NAME} ${TESTGEN_FILE} ${TEST_FILES})

create_source_groups(${TEST_FILES})

target_link_libraries(${TEST_PROJECT_NAME} ${LIB_PROJECT_NAME})

find_package(CxxTest)

if (CXXTEST_FOUND)
	set_property(
		TARGET ${TEST_PROJECT_NAME}
		PROPERTY INCLUDE_DIRECTORIES
			"${CMAKE_SOURCE_DIR}/src/lib"
			"${CXXTEST_INCLUDE_DIR}"
	)
endif (CXXTEST_FOUND)

add_custom_command(
	TARGET ${TEST_PROJECT_NAME}
	PRE_BUILD
	COMMAND cd ${PROJECT_SOURCE_DIR} && ${PYTHON_EXECUTABLE} ${CXXTEST_PYTHON_TESTGEN_EXECUTABLE} --runner=ParenPrinter -o ${TESTGEN_FILE} ${TEST_FILES}
	COMMENT "Generating unittest code with cxxtestgen"
)

add_custom_command(
	TARGET ${TEST_PROJECT_NAME}
	POST_BUILD
	COMMAND cd $(ProjectDir)../../bin/test/\n$(OutDir)$(TargetName)$(TargetExt)
	COMMENT "Running unittest code"
)

# Visual Leak Detector ---------------------------------------------------------

include_directories($ENV{VLD_DIR}/include)



