cmake_minimum_required(VERSION 3.15)
project(qt_grpc_console)

include(common.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Append the CMAKE_PREFIX_PATH to find Qt5

find_package(Qt5 REQUIRED COMPONENTS Core)

set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
set(GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/protogenerated)
file(GLOB PROTO_FILES "${PROTO_DIR}/*.proto")

foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    set(PROTO_SRC ${GEN_DIR}/${PROTO_NAME}.pb.cc)
    set(PROTO_HDR ${GEN_DIR}/${PROTO_NAME}.pb.h)
    set(GRPC_SRC ${GEN_DIR}/${PROTO_NAME}.grpc.pb.cc)
    set(GRPC_HDR ${GEN_DIR}/${PROTO_NAME}.grpc.pb.h)

    add_custom_command(
      OUTPUT "${PROTO_SRC}" "${PROTO_HDR}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${GEN_DIR}"
        --cpp_out "${GEN_DIR}"
        -I "${PROTO_DIR}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${PROTO_FILE}"
      DEPENDS "${PROTO_FILE}")

    list(APPEND PROTO_SRCS ${PROTO_SRC} ${GRPC_SRC})
    list(APPEND PROTO_HDRS ${PROTO_HDR} ${GRPC_HDR})
endforeach()

include_directories(${GEN_DIR})

message(STATUS "Generated Protobuf files: ${PROTO_SRCS}")
message(STATUS "Generated Protobuf headers: ${PROTO_HDRS}")

add_executable(echo_client
    ${CMAKE_CURRENT_SOURCE_DIR}/client/echo_client.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/client/echo_client.h
    ${CMAKE_CURRENT_SOURCE_DIR}/client/main.cpp
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

target_link_libraries(echo_client
    Qt5::Core
    absl::flags
    absl::flags_parse
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
)

add_executable(echo_server
    ${CMAKE_CURRENT_SOURCE_DIR}/server/echo_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/server/echo_server.h
    ${CMAKE_CURRENT_SOURCE_DIR}/server/main.cpp
    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

target_link_libraries(echo_server
    Qt5::Core
    absl::flags
    absl::flags_parse
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
)
