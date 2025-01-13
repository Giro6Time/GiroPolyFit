add_library(model STATIC point_set_io.h point_set.h)
target_include_directories(model PUBLIC ./)
target_link_libraries(model basic math)
