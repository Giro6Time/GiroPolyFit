add_library(math STATIC
    vec.h
    line.h
    math_types.cpp
    plane.h
    box.h
    principal_axes.cpp
    principal_axes.h
    semi_definite_symmetric_eigen.h
)
target_include_directories(math PUBLIC ./)
target_link_libraries(math basic model debugger)
