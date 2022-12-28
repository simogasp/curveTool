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

set(LIB_SOURCE_FILES
        src/approximation.cpp
        src/BezierCurve.cpp
        src/ControlPoints.cpp
        src/interpolation.cpp
        src/InterpolationCurve.cpp
        src/parametrization.cpp
        src/Point.cpp)

set(LIB_HEADER_FILES
        src/approximation.h
        src/BezierCurve.h
        src/ControlPoints.h
        src/Point.h
        src/parametrization.h
        src/interpolation.h
        src/InterpolationCurve.h)

set(LIBRARY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_library(curves ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})
target_include_directories(curves PUBLIC $<BUILD_INTERFACE:${LIBRARY_INCLUDE_DIR}>)
target_link_libraries(curves PUBLIC glm::glm)

add_executable(mainApproximation src/mainApproximation.cpp src/Camera.cpp src/Camera.h)
target_link_libraries (mainApproximation ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_glu_LIBRARY} curves)

add_executable(mainInterpolation src/mainInterpolation.cpp src/Camera.cpp src/Camera.h)
target_link_libraries (mainInterpolation ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_glu_LIBRARY} curves)

if(APPLE)
    target_compile_definitions(mainApproximation PUBLIC GL_SILENCE_DEPRECATION)
    target_compile_definitions(mainInterpolation PUBLIC GL_SILENCE_DEPRECATION)
endif()

if(MSVC)
    target_compile_options(mainApproximation PUBLIC /W4)
    target_compile_options(mainInterpolation PUBLIC /W4)
else()
    target_compile_options(mainApproximation PUBLIC -Wall -Wextra -Wconversion -Wsign-conversion  -Wshadow -Wpedantic -Werror)
    target_compile_options(mainInterpolation PUBLIC -Wall -Wextra -Wconversion -Wsign-conversion  -Wshadow -Wpedantic -Werror)
endif()

if(BUILD_TESTS)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    enable_testing()
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    include(BoostTestHelper)
    add_boost_test(SOURCE src/tests/parametrization_test.cpp LINK curves PREFIX curves)
    add_boost_test(SOURCE src/tests/interpolation_test.cpp LINK curves PREFIX curves)
endif()