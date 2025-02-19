
#include "gtest/gtest.h"
#include "point_set.h"
#include "point_set_io.h"
#include "logger.h"
class PointSetIOTest : public ::testing::Test
{
protected:
    std::shared_ptr<PointSet> ps;
    std::string input_file = "D:/Study/Polyfit/GiroPolyFit/test/pointset.vg";
    std::string output_file = "D:/Study/Polyfit/GiroPolyFit/test/pointset_output.vg";
};

TEST_F(PointSetIOTest, ReadPointSet)
{
    ps = PointSetIO::read(input_file);
    ASSERT_NE(ps, nullptr);
    ASSERT_EQ(ps->groups().size(), 1);

    EXPECT_TRUE(PointSetIO::save(output_file, ps));
    // 比较输入和输出文件是否一致
    std::ifstream in(input_file);
    std::ifstream out(output_file);

    std::string line;
    int i = 0;
    while (getline(in, line))
    {
        i++;
        std::string out_line;
        getline(out, out_line);
        EXPECT_EQ(line, out_line) << "Files differ at line " << i;
    }
}

int main(int argc, char **argv)
{
    Logger::initialize();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}