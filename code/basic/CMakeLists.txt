add_library(basic STATIC common.h basic_types.cpp utils.cpp record_id.h rat.h raw_attribute_store.h)
target_include_directories(basic PUBLIC ./)
target_link_libraries(basic math)
