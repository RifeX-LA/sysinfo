cmake_minimum_required(VERSION 3.16)
project(sysinfo)

set(CMAKE_CXX_STANDARD 20)

set(BOOST_ROOT "C:/") # set boost root path here

set (Boost_NO_SYSTEM_PATHS ON)
set (Boost_USE_MULTITHREADED ON)
set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME OFF)
set (BOOST_ALL_DYN_LINK OFF)

find_package(Boost 1.81.0 REQUIRED COMPONENTS program_options) # set your boost version here
include_directories(${Boost_INCLUDE_DIR} ./)

add_executable(sysinfo main.cpp)
target_link_libraries(sysinfo Boost::program_options)