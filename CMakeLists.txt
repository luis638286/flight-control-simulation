cmake_minimum_required(VERSION 3.16)
project(flight_control_sim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sim
    src/app/main.cpp
)

target_include_directories(sim PRIVATE src)