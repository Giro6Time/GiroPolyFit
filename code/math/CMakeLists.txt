add_library(math STATIC vec.h line.h math_types.h plane.h)
target_include_directories(math PUBLIC ./)
target_link_libraries(math basic)
