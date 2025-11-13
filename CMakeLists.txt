cmake_minimum_required(VERSION 3.20)
project(CurveTool LANGUAGES CXX)

option(BUILD_TESTS "Enable testing" ON)
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(ENABLE_WARNING_AS_ERROR "Enable warnings as errors" OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    set(CurveTool_COMPILE_OPTIONS "-Wall;-Wextra;-pedantic;-Wno-comment;-Wshadow;-Wnon-virtual-dtor;-Wpedantic;-Wconversion;-Wmisleading-indentation;-Wsign-conversion;-Wfloat-equal")
    if(ENABLE_WARNINGS_AS_ERRORS)
        list(APPEND CurveTool_COMPILE_OPTIONS "-Werror")
    endif()
elseif(MSVC)
    set(CurveTool_COMPILE_OPTIONS "/W4;/permissive-;/Zc:__cplusplus")
    if(ENABLE_WARNINGS_AS_ERRORS)
        list(APPEND CurveTool_COMPILE_OPTIONS "/WX")
    endif()
endif()

# compile definitions for all targets depending on the compiler and the system
set(CurveTool_COMPILE_DEFINITIONS "")
if(MSVC)
    set(CurveTool_COMPILE_DEFINITIONS "-DNOMINMAX;-D_USE_MATH_DEFINES")
elseif(APPLE)
    set(CurveTool_COMPILE_DEFINITIONS "-DGL_SILENCE_DEPRECATION")
endif()


if(BUILD_SHARED_LIBS)
    if(WIN32)
        # export all symbols on a windows build of dynamic libs
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS  ON)
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CurveTool_CXX_FEATURE cxx_std_${CMAKE_CXX_STANDARD})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
message(STATUS "GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIRS}")
message(STATUS "GLEW_LIBRARIES ${GLEW_LIBRARIES}")

set(LIB_SOURCE_FILES
        src/curves/approximation.cpp
        src/curves/BezierCurve.cpp
        src/curves/ControlPoints.cpp
        src/curves/interpolation.cpp
        src/curves/InterpolationCurve.cpp
        src/curves/parametrization.cpp
        src/curves/Point.cpp)

set(LIB_HEADER_FILES
        src/curves/approximation.h
        src/curves/BezierCurve.h
        src/curves/ControlPoints.h
        src/curves/Point.h
        src/curves/parametrization.h
        src/curves/interpolation.h
        src/curves/InterpolationCurve.h)

set(CurveTool_TARGETS "")
set(LIBRARY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(curves ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
target_include_directories(curves PUBLIC $<BUILD_INTERFACE:${LIBRARY_INCLUDE_DIR}>)
target_link_libraries(curves PUBLIC glm::glm)
list(APPEND CurveTool_TARGETS curves)

add_executable(mainApproximation src/mainApproximation.cpp src/Camera.cpp src/Camera.h)
target_link_libraries (mainApproximation ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLEW::glew ${OPENGL_glu_LIBRARY} curves)
list(APPEND CurveTool_TARGETS mainApproximation)

add_executable(mainInterpolation src/mainInterpolation.cpp src/Camera.cpp src/Camera.h)
target_link_libraries (mainInterpolation ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLEW::glew ${OPENGL_glu_LIBRARY} curves)
list(APPEND CurveTool_TARGETS mainInterpolation)

foreach(target ${CurveTool_TARGETS})
    target_compile_definitions(${target} PUBLIC ${CurveTool_COMPILE_DEFINITIONS})
    target_compile_options(${target} PUBLIC ${CurveTool_COMPILE_OPTIONS})
    target_compile_features(${target} PUBLIC ${CurveTool_CXX_FEATURE})
endforeach()

if(BUILD_TESTS)

    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        message(STATUS "GTest not found, fetching GTest")
        # Set up googletest
        include(FetchContent)
        FetchContent_Declare(
            googletest
            GIT_TAG v1.17.0
            GIT_REPOSITORY https://github.com/google/googletest.git
            FIND_PACKAGE_ARGS 1.16.0 NAMES GTest COMPONENTS gmain_main
            EXCLUDE_FROM_ALL
            SYSTEM
        )

        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()
    enable_testing()
    include(cmake/GTestHelper.cmake)

     set(TESTS_SOURCES
        src/tests/parametrization_test.cpp
        src/tests/interpolation_test.cpp
        src/tests/point_test.cpp)

    foreach(source ${TESTS_SOURCES})
        add_gtest_test(SOURCE ${source}
            LINK curves
            PREFIX curves_
            COMPILE_OPTIONS ${CurveTool_COMPILE_OPTIONS}
            COMPILE_DEFINITIONS ${CurveTool_COMPILE_DEFINITIONS})
    endforeach()

endif()