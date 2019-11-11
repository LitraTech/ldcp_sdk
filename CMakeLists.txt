cmake_minimum_required(VERSION 2.8)

project(ldcp_sdk)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR MINGW)
  add_compile_options(-std=c++11)
endif()

set(SDK_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
FILE(GLOB_RECURSE SDK_SRC "${SDK_SRC_DIR}/*.cpp")
add_library(${PROJECT_NAME} ${SDK_SRC})

target_include_directories(${PROJECT_NAME}
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/Asio/asio-1.12.1/include"
  PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/third_party/RapidJSON/rapidjson-1.1.0/include"
)

add_definitions(-DASIO_STANDALONE=1)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(BONJOUR_SDK_DIR "C:/Program Files/Bonjour SDK" CACHE STRING "")
  include_directories("${BONJOUR_SDK_DIR}/Include")
  add_definitions(-D_WIN32_WINNT=0x0501 -DNOMINMAX)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries(${PROJECT_NAME} avahi-common avahi-client pthread)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  target_link_libraries(${PROJECT_NAME} "C:/Program Files/Bonjour SDK/Lib/Win32/dnssd.lib" WS2_32.Lib)
endif()
