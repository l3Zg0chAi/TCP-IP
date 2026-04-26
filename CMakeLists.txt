cmake_minimum_required(VERSION 3.10)

project(tcpclient VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)
set(SOURCES src/main.cpp)

add_executable(tcpclient ${SOURCES} -pthread)