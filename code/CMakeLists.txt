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


if(Qt5Core_FOUND AND Qt5Gui_FOUND AND Qt5OpenGL_FOUND AND Qt5Widgets_FOUND)
    set(QT5_FOUND TRUE)
endif()


if(NOT QT5_FOUND)
    message(WARNING "The demo program '${PROJECT_NAME}' requires Qt but Qt was not found. "
        "You can set 'Qt5_DIR' to the directory containing 'Qt5Config.cmake' or 'qt5-config.cmake'. "
        "Optionally, you can set the Qt5 root directory 'QT5_ROOT_PATH' to the directory "
        "containing the 'bin' folder.")

    return()
endif()


add_executable(GiroPolyFit main.cpp)
# # 自动复制必要的 DLL 文件
# if(WIN32)
#     message("${CMAKE_BUILD_TYPE}")
#     if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#         set(QT_DLLS
#             Qt5Cored.dll
#             Qt5Guid.dll
#             Qt5Widgetsd.dll
#             Qt5OpenGLd.dll
#         )
#         set(QT_DLLS_PATH "${Qt5_DIR}/../../../debug/bin")
#     else()
#         set(QT_DLLS
#             Qt5Core.dll
#             Qt5Gui.dll
#             Qt5Widgets.dll
#             Qt5OpenGL.dll
#         )
#         set(QT_DLLS_PATH "${Qt5_DIR}/../../../bin")

#     endif()

#     foreach(QT_DLL ${QT_DLLS})
#         add_custom_command(TARGET GiroPolyFit POST_BUILD
#             COMMAND ${CMAKE_COMMAND} -E copy_if_different
#             "${QT_DLLS_PATH}/${QT_DLL}"
#             $<TARGET_FILE_DIR:GiroPolyFit>)
#     endforeach()

# endif()
add_subdirectory(debugger)
add_subdirectory(math)
add_subdirectory(basic)
add_subdirectory(model)
add_subdirectory(ui)
add_subdirectory(scene)

target_link_libraries(GiroPolyFit debugger model math basic ui scene)

