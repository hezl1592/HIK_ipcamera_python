cmake_minimum_required(VERSION 2.8)
project(libHKCamera_v1)
message("CMAKE_CURRENT_SOURCE_DIR:"${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "-fPIC")   #c++
# set(CMAKE_C_FLAGS "-fPIC")   #c

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

link_directories(/usr/lib)
# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib/HCNetSDKCom)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

add_library(${PROJECT_NAME} SHARED ../hkso.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
target_link_libraries(${PROJECT_NAME} ${LIBRARIES} ${OpenCV_LIBS} libhcnetsdk.so libHCCore.so libhpr.so libPlayCtrl.so libSuperRender.so libAudioRender.so libssl.so)