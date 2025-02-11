add_library(model STATIC point_set_io.cpp point_set.cpp point_set_serializer.cpp)
target_include_directories(model PUBLIC ./)
target_link_libraries(model basic math debugger)
