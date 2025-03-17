find_package(OpenGL REQUIRED)
add_library(scene STATIC camera.cpp openglfunc.h point_set_renderer.cpp style.h grid.cpp)


set(QtLibs Qt::Core Qt::Widgets Qt::Xml Qt::OpenGL)
target_include_directories(scene PUBLIC ./)

target_link_libraries(scene PUBLIC ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(scene PUBLIC debugger model)

