
if(GLFW_INCLUDE_DIRS)
  set(GLFW_FIND_QUITELY TRUE)
endif()

find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h)
find_library(GLFW_LIBRARY NAMES glfw)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIBRARY GLFW_INCLUDE_DIR)

if(GLFW_FOUND)
  set(GLFW_LIBRARIES ${GLFW_LIBRARY})
  set(GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
endif()

mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY)
mark_as_advanced(GLFW_INCLUDE_DIRS GLFW_LIBRARIES)