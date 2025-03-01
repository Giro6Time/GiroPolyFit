set(PROJECT_NAME "GiroPolyFit")
project(${PROJECT_NAME})


# ------------------------------------------------------------------------------
# Qt
# ------------------------------------------------------------------------------
## we will use cmake autouic/automoc/autorcc features
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)



# find qt5 components
find_package(Qt5 COMPONENTS Core Gui OpenGL Widgets REQUIRED)
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5OpenGL)
find_package(Qt5Widgets)
find_package(Qt5Xml QUIET)


if (Qt5Core_FOUND AND Qt5Gui_FOUND AND Qt5OpenGL_FOUND AND Qt5Widgets_FOUND)
    set(QT5_FOUND TRUE)
endif ()


if (NOT QT5_FOUND)
    message(WARNING "The demo program '${PROJECT_NAME}' requires Qt but Qt was not found. "
            "You can set 'Qt5_DIR' to the directory containing 'Qt5Config.cmake' or 'qt5-config.cmake'. "
            "Optionally, you can set the Qt5 root directory 'QT5_ROOT_PATH' to the directory "
            "containing the 'bin' folder.")

    return()
endif ()


add_executable(GiroPolyFit main.cpp)
target_link_libraries( ${PROJECT_NAME} Qt5::Widgets)
target_include_directories( ${PROJECT_NAME} PRIVATE Qt5Core)

add_subdirectory(debugger)
add_subdirectory(math)
add_subdirectory(basic)
add_subdirectory(model)

target_link_libraries(GiroPolyFit debugger model math basic)

