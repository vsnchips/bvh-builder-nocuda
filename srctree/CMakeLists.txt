################################################################
#                                                              #
#                          Created by:                         #
#                         James Miller                         #
#                                                              #
#                          Edited by:                          #
#                           Dan Aston                          #
#                                                              #
################################################################

cmake_minimum_required(VERSION 3.1)

# Include Extra CMake capabilities
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

# Assignment Name
set(CGRA_PROJECT "BVH_Renderer" CACHE STRING "CGRA Project Name")

# Project
project("CGRA_PROJECT_${CGRA_PROJECT}" CXX C)

include(CGRACompiler)
include(CGRAFunctions)

#Package Finding
FIND_PACKAGE(PkgConfig REQUIRED)


# Enable IDE Project Folders
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Force Output Directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Vendored libraries and Submodules
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/submods/stringStreamConCat/")
#add_subdirectory("${PROJECT_SOURCE_DIR}/submods/stringStreamConCat/")
add_subdirectory(vendor/glew)
include_directories("${PROJECT_SOURCE_DIR}/vendor/glfw/include")
include_directories("${PROJECT_SOURCE_DIR}")
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/imgui)

# Find OpenGL
find_package(OpenGL REQUIRED)

#TODO: Only using dialogs for testing the renderer on linux builds - cross platform next iteration
if(UNIX)
#NFD
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/submods/")
LINK_DIRECTORIES(submods/nfd/)
ADD_LIBRARY(nfd_lib STATIC IMPORTED)
SET_PROPERTY(TARGET nfd_lib PROPERTY IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/nfd/libnfd.a")

#GTK3
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
message(status "GTK3_INCLUDE_DIRS = ${GTK3_INCLUDE_DIRS}")
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
message(status "GTK3_LIBRARY_DIRS = ${GTK3_LIBRARY_DIRS}")
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})
message(status "GTK3_CFLAGS_OTHER = ${GTK3_CFLAGS_OTHER}")
endif(UNIX)

# Add subdirectories
add_subdirectory(res) # Resources; for example shaders
#set_property(TARGET ${CGRA_PROJECT} PROPERTY FOLDER "CGRA")

# Source Files
#
# If you add any new files, remember to add them here
SET(sources
  #news
  #BVH.cpp
  BVH_BBox.cpp 
  BVH_Primitives.cpp 
  BVH_MeshLoad.cpp
  BVH_NodeCode.cpp
  BVH_Sender.cpp 
  submods/stringStreamConCat/vmpwStringStreamConcat.cpp
  BVHRenderer.hpp
  BVHRenderer.cpp
  bvhApp.hpp
  bvhApp.cpp
  #olds
  bvhApp_dogui.cpp
  bvhApp_inevents.cpp
  drawStyle.hpp
  main.cpp
)

add_executable(${CGRA_PROJECT} ${sources})

add_subdirectory(cgra)

target_source_group_tree(${CGRA_PROJECT})

target_link_libraries(${CGRA_PROJECT} PRIVATE ${OPENGL_LIBRARY})
target_link_libraries(${CGRA_PROJECT} PRIVATE glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glm imgui)
#target_link_libraries(${CGRA_PROJECT} PRIVATE nfd_lib)
target_link_libraries(${CGRA_PROJECT} PRIVATE nfd)
target_link_libraries(${CGRA_PROJECT} PRIVATE ${GTK3_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE stdc++fs)

# Set the source directory as a preprocessor define, used to make sure that the relative paths
# work correctly, regardless of where the project is run fron (as long as it's run on the same
# machine it was built on).
target_compile_definitions(${CGRA_PROJECT} PRIVATE "-DCGRA_SRCDIR=\"${PROJECT_SOURCE_DIR}\"")
