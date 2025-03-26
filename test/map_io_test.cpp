#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include "map_serializer.h"
#include "map.h"

TEST(MapIOTest, LoadCubeObj)
{
    // 创建 MapSerializer_obj 实例
    std::shared_ptr<MapSerializer_obj> serializer = std::make_shared<MapSerializer_obj>();

    // 创建一个空的 Map 对象
    std::shared_ptr<Map> map = std::make_shared<Map>();
    // 定义测试文件路径
    std::string obj_file = "D:/Study/Polyfit/GiroPolyFit/test/cube.obj";

    // 打开文件并检查是否存在
    std::ifstream input(obj_file);
    ASSERT_TRUE(input.is_open()) << "Failed to open test file: " << obj_file;

    // 测试 serialize_read 方法
    bool success = serializer->serialize_read(obj_file, map);
    ASSERT_TRUE(success) << "Failed to load OBJ file: " << obj_file;

    // 验证加载的 Map 对象是否有效
    ASSERT_GT(map->num_vertices(), 0) << "Map should have vertices.";
    ASSERT_GT(map->num_facets(), 0) << "Map should have facets.";

    // 验证顶点和面的数量是否符合预期（根据 cube.obj 的内容调整）
    EXPECT_EQ(map->num_vertices(), 8) << "Cube should have 8 vertices.";
    EXPECT_EQ(map->num_facets(), 6) << "Cube should have 6 facets.";
}