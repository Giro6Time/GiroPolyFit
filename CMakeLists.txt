cmake_minimum_required(VERSION 3.10)
project(GiroPolyfit)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# add_compile_options("/utf-8")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Cmake")


enable_testing()


add_subdirectory(code)
add_subdirectory(test)
add_subdirectory(third_party/googletest)
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()
