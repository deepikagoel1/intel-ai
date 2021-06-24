cmake_minimum_required(VERSION 3.15)

project(ospTutorial LANGUAGES CXX)

foreach(p
CMP0048 # OK to clear PROJECT_VERSION on project()
CMP0054 # CMake 3.1
CMP0056 # export EXE_LINKER_FLAGS to try_run
CMP0057 # Support no if() IN_LIST operator
CMP0063 # Honor visibility properties for all targets
CMP0074 # Searches prefixes specified by the <PackageName>_ROOT
)
if(POLICY ${p})
cmake_policy(SET ${p} NEW)
endif()
endforeach()

include(ExternalProject)
include(GNUInstallDirs)
include(ProcessorCount)

set(ONEAPI_ROOT /opt/intel/inteloneapi/)
set(OSPRAY_ROOT ${ONEAPI_ROOT}/ospray/latest)
set(RKCOMMON_ROOT ${ONEAPI_ROOT}/rkcommon/latest)
# set(EMBREE_ROOT ${ONEAPI_ROOT}/embree/latest)
# set(OPENVKL_ROOT ${ONEAPI_ROOT}/openvkl/latest)
# set(OIDN_ROOT ${ONEAPI_ROOT}/oidn/latest)
set(TRAINING_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(TBBROOT ${TRAINING_ROOT}/third-party/tbb)
set(GLM_ROOT ${TRAINING_ROOT}/third-party/glm)
set(STB_ROOT ${TRAINING_ROOT}/third-party)
set(TINYOBJ_ROOT ${TRAINING_ROOT}/third-party)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS ">>>>>>>>>>[STATUS] Default CMAKE_BUILD_TYPE not set on the command-line. Defaulting to Release with Debug Info (RelWithDebInfo).")
  set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE
    STRING "Choose the type of build, options are: None, Debug, Release, RelWithDebInfo, MinSizeRel."
    FORCE)
endif()

list(APPEND CMAKE_PREFIX_PATH 
  ${RKCOMMON_ROOT}/lib/cmake/rkcommon-1.6.0
  ${OSPRAY_ROOT}/lib/cmake/ospray-2.5.0
  #${EMBREE_ROOT}
  #${OPENVKL_ROOT}/lib/cmake/openvkl-0.11.0
  #${OIDN_ROOT}/lib/cmake/OpenImageDenoise
  )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

if("${TBB_ROOT}" STREQUAL "")
  message(STATUS ">>>>>>>>>>[STATUS] No TBB_ROOT set on command line. Defaulting to TBB_ROOT found by rkcomo.")
  set(TBB_ROOT ${TBBROOT} CACHE STRING "${TBBROOT}" FORCE)
  set(TBB_INCLUDE_DIR ${TBBROOT}/include CACHE STRING "${TBBROOT}/include" FORCE)
  list(APPEND CMAKE_MODULE_PATH /opt/intel/inteloneapi/rkcommon/latest/lib/cmake/rkcommon-1.5.0)
endif()

find_package(rkcommon 1.5.0 REQUIRED)
find_package(ospray 2.4.0 REQUIRED)
find_package(TBB 2020.3 REQUIRED tbb tbbmalloc)
find_package(glm PATHS ${GLM_ROOT} REQUIRED)

add_executable(${PROJECT_NAME}
  src/ospTutorial.cpp)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${OSPRAY_ROOT}/include/ospray
    PUBLIC ${OSPRAY_ROOT}/include/ospray/ospray_testing
    PUBLIC ${OSPRAY_ROOT}/src/ospray/apps
    PUBLIC ${OSPRAY_ROOT}/src/ospray/apps/common/arcball_camera
    PUBLIC ${OSPRAY_ROOT}/include
    PUBLIC ${RKCOMMON_ROOT}/include
    PUBLIC ${RKCOMMON_ROOT}/include/rkcommon
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${GLM_ROOT}/include
    PUBLIC ${STB_ROOT}
    PUBLIC ${TINYOBJ_ROOT})

target_link_directories(${PROJECT_NAME} 
  PUBLIC ${RKCOMMON_ROOT}/lib
  PUBLIC ${OSPRAY_ROOT}/lib)

target_link_libraries(${PROJECT_NAME} PUBLIC 
  rkcommon 
  ospray 
  ospray_testing 
  TBB::tbb TBB::tbbmalloc)
