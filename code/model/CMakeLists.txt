add_library(model STATIC io.h pointset.h)
target_include_directories(model PUBLIC ./)
target_link_libraries(model basic math)
