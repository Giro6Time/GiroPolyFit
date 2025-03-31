#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include "map_serializer.h"
#include "map.h"
#include "map_io.h"
#include "logger.h"
TEST(MapIOTest, LoadCubeObj)
{
    Logger::initialize();

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
    MapIO::save("MYOBJ.obj", map);

    // 验证加载的 Map 对象是否有效
    ASSERT_GT(map->num_vertices(), 0) << "Map should have vertices.";
    ASSERT_GT(map->num_facets(), 0) << "Map should have facets.";

    // 验证顶点和面的数量是否符合预期（根据 cube.obj 的内容调整）
    EXPECT_EQ(map->num_vertices(), 8) << "Cube should have 8 vertices.";
    EXPECT_EQ(map->num_facets(), 12) << "Cube should have 12 facets.";

    // 验证每个顶点
    for (auto vertex_it = map->vertices_begin(); vertex_it != map->vertices_end(); ++vertex_it)
    {
        auto vertex = *vertex_it;
        ASSERT_TRUE(vertex != nullptr) << "Vertex should not be null.";
        ASSERT_TRUE(vertex->halfedge() != nullptr) << "Vertex should have a valid halfedge.";
    }

    // 验证每个半边
    for (auto halfedge_it = map->halfedges_begin(); halfedge_it != map->halfedges_end(); ++halfedge_it)
    {
        auto halfedge = *halfedge_it;
        ASSERT_TRUE(halfedge != nullptr) << "Halfedge should not be null.";
        ASSERT_TRUE(halfedge->next() != nullptr) << "Halfedge should have a valid next pointer.";
        ASSERT_TRUE(halfedge->opposite() != nullptr) << "Halfedge should have a valid opposite pointer.";
        ASSERT_TRUE(halfedge->vertex() != nullptr) << "Halfedge should have a valid vertex.";
        ASSERT_TRUE(halfedge->facet() != nullptr) << "Halfedge should belong to a valid facet.";
    }

    // 验证每个面
    for (auto facet_it = map->facets_begin(); facet_it != map->facets_end(); ++facet_it)
    {
        auto facet = *facet_it;
        ASSERT_TRUE(facet != nullptr) << "Facet should not be null.";
        ASSERT_TRUE(facet->halfedge() != nullptr) << "Facet should have a valid halfedge.";

        // 验证面是否闭合
        auto start_halfedge = facet->halfedge();
        auto current_halfedge = start_halfedge;
        int edge_count = 0;
        do
        {
            ASSERT_TRUE(current_halfedge != nullptr) << "Facet should have a valid halfedge loop.";
            current_halfedge = current_halfedge->next();
            ++edge_count;
        } while (current_halfedge != start_halfedge);

        ASSERT_GT(edge_count, 2) << "Facet should have at least 3 edges.";
    }
}