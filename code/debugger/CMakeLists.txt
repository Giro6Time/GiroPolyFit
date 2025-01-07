add_library(debugger STATIC logger.cpp basic.cpp)
target_include_directories(debugger PUBLIC ./)
target_link_libraries(debugger basic)