#pragma once
#include "gtest/gtest.h"
#include <QApplication>
#include "logger.h"
#include "mesh_renderer.h"
#include "map_io.h"
#include "map_serializer.h"
#include "camera.h"
#include "style.h"
#include "canvas.h"

/* 该测试用于检验 MeshRenderer 是否正确渲染网格
   Giro: 由于无法通过脚本自动验证渲染结果，测试会显示一个窗口供人工观察 */
static QApplication *app;
static int argc = 1;
static char *argv[] = {"GiroPolyFit"};

class MeshRendererTest : public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        // 初始化 QApplication
        app = new QApplication(argc, argv);
    }

    static void TearDownTestSuite()
    {
        delete app;
    }

    void SetUp() override
    {
        // 初始化 Logger
        Logger::initialize();
    }

    std::shared_ptr<Map> map;
    std::shared_ptr<Ui::Canvas> canvas;
};

TEST_F(MeshRendererTest, RenderMesh)
{
    canvas = std::make_shared<Ui::Canvas>();
    // 加载网格文件
    std::string obj_file = "D:/Study/Polyfit/GiroPolyFit/test/cube.obj";
    map = MapIO::read(obj_file);
    ASSERT_TRUE(map) << "Failed to load OBJ file: " << obj_file;

    // 渲染网格
    canvas->add_mesh(map);
    canvas->show();

    // 运行事件循环
    app->exec();
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}