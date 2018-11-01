set(GLM_VERSION "0.9.7")
set(GLM_INCLUDE_DIRS "/home/shelly/local/CGRA350/demo/srctree/vendor/glm")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
