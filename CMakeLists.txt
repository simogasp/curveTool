cmake_minimum_required(VERSION 3.8)
project(CurveTool LANGUAGES CXX)

option(BUILD_TESTS "Enable testing" ON)
option(BUILD_SHARED_LIBS "Build shared library" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(LIBRARY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(curves ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
target_include_directories(curves PUBLIC $<BUILD_INTERFACE:${LIBRARY_INCLUDE_DIR}>)
target_link_libraries(curves PUBLIC glm::glm)

set(EXECUTABLE_TARGETS "")
add_executable(mainApproximation src/mainApproximation.cpp src/Camera.cpp src/Camera.h)
target_link_libraries (mainApproximation ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLEW::glew ${OPENGL_glu_LIBRARY} curves)
list(APPEND EXECUTABLE_TARGETS "mainApproximation")

add_executable(mainInterpolation src/mainInterpolation.cpp src/Camera.cpp src/Camera.h)
target_link_libraries (mainInterpolation ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLEW::glew ${OPENGL_glu_LIBRARY} curves)
list(APPEND EXECUTABLE_TARGETS "mainInterpolation")

foreach(target ${EXECUTABLE_TARGETS})
    if(APPLE)
        target_compile_definitions(${target} PUBLIC GL_SILENCE_DEPRECATION)
    endif()
    if(MSVC)
        target_compile_options(${target} PUBLIC /W4)
    else()
        target_compile_options(${target} PUBLIC -Wall -Wextra -Wconversion -Wsign-conversion  -Wshadow -Wpedantic -Werror)
    endif()
endforeach()

if(BUILD_TESTS)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    enable_testing()
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    include(BoostTestHelper)
    add_boost_test(SOURCE src/tests/parametrization_test.cpp LINK curves PREFIX curves)
    add_boost_test(SOURCE src/tests/interpolation_test.cpp LINK curves PREFIX curves)
    add_boost_test(SOURCE src/tests/point_test.cpp LINK curves PREFIX curves)
endif()