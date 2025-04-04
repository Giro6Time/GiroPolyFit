cmake_minimum_required(VERSION 3.10)
project(GiroPolyfit)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(GTEST OFF)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL") # 对应 /MDd
endif()

enable_testing()

add_subdirectory(code)
if(GTEST)
    add_subdirectory(test)
    add_subdirectory(third_party/googletest)
endif()
set(gtest_force_shared_crt ON CACHE BOOL "")

