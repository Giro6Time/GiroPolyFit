add_library(math STATIC
    vec.h
    line.h
    math_types.cpp
    plane.h
    box.h
)
target_include_directories(math PUBLIC ./)
target_link_libraries(math basic model debugger)
