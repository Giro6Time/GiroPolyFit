#CGAL_Qt6 is needed for the drawing.

add_library(method STATIC
    map_generator.cpp
    map_generator.h
    ransac.cpp

)
find_package(Boost REQUIRED COMPONENTS multiprecision)
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt6)
find_package(CGAL REQUIRED)
find_package(GLPK REQUIRED)
if(GLPK_FOUND)
    add_definitions(-DCGAL_USE_GLPK)
    message("QWQWQ  ${GLPK_LIBRARIES}")
endif()

target_include_directories(method PUBLIC ./)
target_link_libraries(method model basic math debugger CGAL::CGAL)
target_link_libraries(method ${GLPK_LIBRARIES}
    Boost::multiprecision
)

target_compile_options(method
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
    $<$<CXX_COMPILER_ID:GNU>:-Wa,-mbig-obj>)
