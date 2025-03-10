cmake_minimum_required(VERSION 3.10)
project(GiroPolyfit)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# add_compile_options("/utf-8")

enable_testing()


add_subdirectory(code)
add_subdirectory(test)
add_subdirectory(third_party/googletest)
set(gtest_force_shared_crt ON)
message(${gtest_force_shared_crt})
set(-DCMAKE_TOOLCHAIN_FILE "D:/Study/vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake")
