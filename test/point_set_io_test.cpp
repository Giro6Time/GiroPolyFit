
#include "gtest/gtest.h"
#include "point_set.h"
#include "point_set_io.h"
#include "logger.h"
class PointSetIOTest : public ::testing::Test
{
protected:
    PointSet *ps;

    void SetUp() override
    {
        ps = nullptr;
    }

    void TearDown() override
    {
        if (!ps)
            delete ps; // 在每次测试后释放 ps 指向的内存
        ps = nullptr;
    }
};

TEST_F(PointSetIOTest, ReadPointSet)
{
    ps = PointSetIO::read("D:/Study/Polyfit/GiroPolyFit/test/pointset.vg");
    ASSERT_NE(ps, nullptr);
}

int main(int argc, char **argv)
{
    Logger::initialize();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}