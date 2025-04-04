#CGAL_Qt6 is needed for the drawing.

add_library(method STATIC
    map_generator.cpp
    map_generator.h
    ransac.cpp
    ransac.h
    polygonal_surface_reconstruction.cpp
    polygonal_surface_reconstruction.h

)
find_package(Boost REQUIRED COMPONENTS multiprecision)
find_package(CGAL REQUIRED OPTIONAL_COMPONENTS Qt6)
find_package(CGAL REQUIRED)
find_package(GLPK REQUIRED)
if(GLPK_FOUND)
    add_definitions(-DCGAL_USE_GLPK)
    list(APPEND CGAL_USE_FILE_LIST ${CGAL_DIR}/CGAL_GLPK_support.cmake)
    include(${CGAL_USE_FILE_LIST})
endif()

target_include_directories(method PUBLIC ./)
target_link_libraries(method model basic math debugger CGAL::CGAL CGAL::GLPK_support)
target_link_libraries(method ${GLPK_LIBRARIES}
    Boost::multiprecision
)

target_compile_options(method
    PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
    $<$<CXX_COMPILER_ID:GNU>:-Wa,-mbig-obj>)
