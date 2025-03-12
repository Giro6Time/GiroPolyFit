project(GiroPolyFitTest)
find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Core Gui OpenGL Widgets Xml REQUIRED)
# find_package(GTest CONFIG REQUIRED)

message(STATUS "Enable testing")
set(AllTest "AllTest")
set(QtLibs Qt::Core Qt::Widgets Qt::Xml Qt::OpenGL)


# 添加 gtest 的头文件路径
# 定义一个函数来添加测试
function(add_gtest test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name} PRIVATE model debugger scene ${QtLibs} OpenGL::GL OpenGL::GLU)
    target_link_libraries(${test_name} PRIVATE gtest gtest_main)
    message(STATUS "Adding test: ${test_name} from source: ${source_file}")
    add_test(NAME ${test_name} COMMAND "${test_name}.exe")
endfunction()

file(GLOB TEST_SOURCES "*.cpp")

foreach(source_file IN LISTS TEST_SOURCES)
    get_filename_component(unit_test_name ${source_file} NAME_WE)
    add_gtest(${unit_test_name} ${source_file})
endforeach()

# add_gtest(testest point_set_io_test.cpp)
