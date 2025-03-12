find_package(OpenGL REQUIRED)
add_library(ui STATIC canvas.cpp)


set(QtLibs Qt::Core Qt::Widgets Qt::Xml Qt::OpenGL)
target_include_directories(ui PUBLIC ./)

target_link_libraries(ui PUBLIC ${QtLibs} OpenGL::GL OpenGL::GLU)
target_link_libraries(ui PRIVATE scene)
