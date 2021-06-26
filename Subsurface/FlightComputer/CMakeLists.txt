cmake_minimum_required(VERSION 3.13)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    # Get Protobuf Library
    find_package(Protobuf CONFIG REQUIRED)
else ()
    set(Protobuf_INCLUDE_DIR /usr/local/include)
    set(Protobuf_LIBRARIES /usr/local/lib/libprotobuf.a)
endif (WIN32)

include_directories(${Protobuf_INCLUDE_DIR})

include_directories(vendors/asio-1.18.0/include)
include_directories(vendors/EasyLogger)
include_directories(include)
include_directories(src)

set(HEADERS
    include/FlightComputer.h
    include/Message.h
    include/NetClient.h
    include/Paradigm.pb.h
)

set(SOURCES
    src/Paradigm.pb.cc
    src/FlightComputer.cpp
    vendors/EasyLogger/easylogging++.cc
)

find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
    add_executable(FlightComputer ${SOURCES} ${HEADERS})
    target_compile_definitions(FlightComputer PUBLIC _WIN32_WINNT=0x0601)
    target_compile_definitions(FlightComputer PUBLIC ELPP_WINSOCK2)
else ()
    add_executable(FlightComputer ${SOURCES})
endif (WIN32)

target_include_directories(FlightComputer PRIVATE ${CMAKE_BINARY_DIR})

if (WIN32)
    target_link_libraries(FlightComputer protobuf::libprotobuf)
else ()
    target_link_Libraries(FlightComputer ${Protobuf_LIBRARIES} Threads::Threads)
endif (WIN32)
