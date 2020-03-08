cmake_minimum_required(VERSION 3.0)
project(midi)

# Protobuf
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
set(PROTOBUF_LIBRARY_DIR ${PROTOBUF_INCLUDE_DIRS}/../lib)

# proto
set(PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/proto --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/proto)
add_subdirectory(proto)

# io
add_subdirectory(io)

# show
add_subdirectory(show)
