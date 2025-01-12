add_library(math STATIC vec.h)
target_include_directories(math PUBLIC ./)
target_link_libraries(math basic)