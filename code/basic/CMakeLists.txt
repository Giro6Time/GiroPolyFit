add_library(basic STATIC
    common.h
    color.h
    basic_types.cpp
    utils.cpp
    record_id.h
    raw_attribute_store.h
    rat.h
    attribute.h
    attribute_store.cpp
    attribute_life_cycle.cpp
    attribute_manager.cpp
    attribute_adapter.cpp
)
target_include_directories(basic PUBLIC ./)
target_link_libraries(basic math debugger model)
