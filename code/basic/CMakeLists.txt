add_library(basic STATIC common.h basic_types.cpp utils.cpp)
target_include_directories(basic PUBLIC ./)
target_link_libraries(basic math)
