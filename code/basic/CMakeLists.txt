add_library(basic STATIC
    common.h
    color.h
    basic_types.cpp
    utils.cpp
    simple_attribute.h
)
target_include_directories(basic PUBLIC ./)
target_link_libraries(basic math debugger model)
