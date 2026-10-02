cmake_minimum_required(VERSION 3.16)
project(flight_control_sim CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sim
    src/app/main.cpp
)

target_include_directories(sim PRIVATE src)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.18.0
)
# Windows/MSVC: makes gtest use the same runtime library as your code
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    tests/test_pid.cpp
    src/domain/PIDController.cpp
)
target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)