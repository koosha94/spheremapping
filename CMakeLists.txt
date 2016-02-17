cmake_minimum_required(VERSION 2.8.1) 
project(NGRenderer)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
link_directories(${GLFW_LIBRARY_DIRS})
find_package(OpenCL REQUIRED)
message("OpenCL was found : ${OpenCL_FOUND}")
find_package(OpenGL REQUIRED)
message("OpenGL was found : ${OPENGL_FOUND}")
find_package(GLEW REQUIRED)
message("GLEW was found : ${GLEW_FOUND}")
find_package(GLM REQUIRED)
message("GLM was found : ${GLM_FOUND}")


set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
FIND_PACKAGE( Boost 1.60 COMPONENTS program_options filesystem  REQUIRED )
find_package(BoostCompute REQUIRED)
configure_file(lib/util/config.h.in lib/util/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
file( GLOB UTIL_LIB_SOURCES lib/util/*.cpp lib/util/*.c)
file( GLOB UTIL_LIB_HEADERS lib/util/*.h lib/util/*.hpp )
#file( GLOB RENDERER_LIB_SOURCES lib/Renderer/*.cpp )
#file( GLOB RENDERER_LIB_HEADERS lib/Renderer/*.h )

add_executable(NGRenderer main.cpp)

#add_library( Renderer  ${RENDERER_LIB_SOURCES} ${RENDERER_LIB_HEADERS} )
add_library( util ${UTIL_LIB_SOURCES} ${UTIL_LIB_HEADERS} )
#target_link_libraries(NGRenderer util Renderer)
target_link_libraries(NGRenderer util)

macro( config_project PROJNAME LIBNAME )
  include_directories( ${${LIBNAME}_INCLUDE_DIR} )
  target_link_libraries( ${PROJNAME} ${${LIBNAME}_LIBRARY} )
endmacro()
message(“${OpenCL_LIBRARY}”)
config_project(NGRenderer OpenCL)
config_project(NGRenderer OPENGL)
config_project(NGRenderer GLEW)
config_project(NGRenderer GLM)
config_project(NGRenderer Boost)
include_directories(${BoostCompute_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(NGRenderer ${GLFW_LIBRARIES})


message(“${GLFW_INCLUDE_DIRS}”)
target_link_libraries( NGRenderer ${Boost_LIBRARIES} )
#add_custom_command(TARGET NGRenderer PRE_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/resources/
#        ${PROJECT_BINARY_DIR}/resources)
#message(“${PROJECT_BINARY_DIR}”)
install(TARGETS NGRenderer RUNTIME DESTINATION bin)
