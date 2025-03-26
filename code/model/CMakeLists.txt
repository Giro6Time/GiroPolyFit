
add_library(model STATIC
    point_set_io.cpp
    point_set.cpp
    point_set_serializer.cpp
    vertex_group.h
    map_cell.cpp
    map.cpp
    map_builder.cpp
    map_serializer.cpp
    map_io.cpp

)
target_include_directories(model PUBLIC ./)
target_link_libraries(model basic math debugger)
find_package(PCL CONFIG REQUIRED)
target_link_libraries(model ${PCL_LIBRARIES})
