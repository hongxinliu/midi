cmake_minimum_required(VERSION 3.0)
project(midi)

# Protobuf
if(DEFINED ENV{Protobuf_ROOT_DIR})
  find_package(Protobuf REQUIRED PATHS "$ENV{Protobuf_ROOT_DIR}/lib/cmake/protobuf" NO_DEFAULT_PATH)
  include("$ENV{Protobuf_ROOT_DIR}/lib/cmake/protobuf/protobuf-module.cmake")
else()
  message(WARNING "Protobuf_ROOT_DIR is not set, fallback to default paths")
  find_package(Protobuf REQUIRED)
endif()
include_directories(${Protobuf_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
set(PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/proto --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/proto)
add_subdirectory(proto)

# io
add_subdirectory(io)

# show
add_subdirectory(show)
