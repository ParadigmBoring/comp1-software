cmake_minimum_required(VERSION 3.15)
project(FlightComputerTest)

set(CMAKE_CXX_STANDARD 17)

include_directories(../vendors/asio-1.18.0/include)
include_directories(../vendors/EasyLogger)
include_directories(../vendors/proto/include)
include_directories(../include)
include_directories(../src)

if (WIN32)
    # Get Protobuf Library
    find_package(Protobuf CONFIG REQUIRED)
else ()
    set(Protobuf_INCLUDE_DIR /usr/local/include)
    set(Protobuf_LIBRARIES /usr/local/lib/libprotobuf.a)
endif (WIN32)

file(GLOB_RECURSE PROTO_SOURCES ../src/*.pb.cc)
file(GLOB_RECURSE HEADERS ../include/*.h ../include/*.hpp ../include/*.pb.h)

set(SOURCES
    ../vendors/EasyLogger/easylogging++.cc
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(FlightComputerTest ${SOURCES} ${PROTO_SOURCES} ${HEADERS} FlightComputerTest.cpp catch.hpp)

target_compile_definitions(FlightComputerTest PUBLIC _WIN32_WINNT=0x0601)
target_compile_definitions(FlightComputerTest PUBLIC ELPP_WINSOCK2)

target_include_directories(FlightComputerTest PRIVATE ${CMAKE_BINARY_DIR})

if (WIN32)
    target_link_libraries(FlightComputerTest protobuf::libprotobuf)
else ()
    target_link_libraries(FlightComputerTest ${Protobuf_LIBRARIES} Threads::Threads)
endif (WIN32)
