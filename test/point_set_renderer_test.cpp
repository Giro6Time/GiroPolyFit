#include "gtest/gtest.h"
#include <QApplication>
#include "logger.h"
#include "point_set_io.h"
#include "point_set_renderer.h"
#include "camera.h"
#include "style.h"

/*该测试用于检验pointset是否有被正确的渲染
  Giro: 由于我还不懂怎么通过写gtest脚本来检验渲染是否正确发生，所以只能让他渲染出窗口来然后直接观察结果-，-*/
static QApplication *app;
static int argc = 1;
static char *argv[] = {"GiroPolyFit"};
class PointSetRendererTest : public ::testing::Test
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

        // 读取点集文件
        pointset = PointSetIO::read("D:/Study/Polyfit/GiroPolyFit/test/pointset.vg");

        // 初始化渲染器
        renderer = new PointSetRenderer();
        PointStyle style;
        style.size = 5;
        renderer->set_point_style(style);
    }

    void TearDown() override
    {
        delete renderer;
    }

    std::shared_ptr<PointSet> pointset;
    PointSetRenderer *renderer;
};

TEST_F(PointSetRendererTest, RenderPointSet)
{
    // 渲染点集
    Camera camera;
    int num = pointset->points().size();
    EXPECT_EQ(num, 3);

    app->exec();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}