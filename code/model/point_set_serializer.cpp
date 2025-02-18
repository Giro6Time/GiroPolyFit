#include "point_set_serializer.h"

/*
// file format definition 文件格式
num_points: num
x  y  z
...

num_colors: num
r g b
...

num_normals: num
nx  ny  nz

num_groups: num

group_type: type (integer: 	VG_PLANE = 0, VG_CYLINDER = 1, VG_SPHERE = 2, VG_CONE = 3, VG_TORUS = 4, VG_GENERAL = 5)
num_group_parameters: NUM_GROUP_PARAMETERS   // number of floating point values (integer)
group_parameters: float[NUM_GROUP_PARAMETERS]
group_label: label  // the first group info
group_color: color (r, g, b)
group_num_points: num
idx ...

num_children: num

group_type: type (integer: 	VG_PLANE = 0, VG_CYLINDER = 1, VG_SPHERE = 2, VG_CONE = 3, VG_TORUS = 4, VG_GENERAL = 5)
num_group_parameters: NUM_GROUP_PARAMETERS   // number of floating point values (integer)
group_parameters: float[NUM_GROUP_PARAMETERS]
group_label: label  // 0th child of group 0
group_color: color (r, g, b)
group_num_points: num
idx ...

group_type: type (integer: 	VG_PLANE = 0, VG_CYLINDER = 1, VG_SPHERE = 2, VG_CONE = 3, VG_TORUS = 4, VG_GENERAL = 5)
num_group_parameters: NUM_GROUP_PARAMETERS   // number of floating point values (integer)
group_parameters: float[NUM_GROUP_PARAMETERS]
group_label: label  // 1st child of group 0
group_color: color (r, g, b)
group_num_points: num
idx ...
*/

void PointSetSerializer::load_vg(std::unique_ptr<PointSet> point_set, const std::string &file_name)
{
    // TODO: 添加动态进度记录

    // 实现加载vg文件的功能
    std::ifstream input = std::ifstream(file_name.c_str());
    if (input.fail())
    {
        Logger::error("PointSetSerializer") << "could not open file \"" << file_name << "\"" << std::endl;
        return;
    }

    input.seekg(0, input.end);
    std::streamoff length = input.tellg();
    input.seekg(0, input.beg);
    // ProgressLogger progress(length);

    // dumy表示当前读取的数据类型（如point，color等）
    std::string dumy;
    std::size_t num;

    // 读入点坐标
    input >> dumy >> num;
    std::vector<vec3> &points = point_set->points();
    points.resize(num);

    for (int i = 0; i < num; ++i)
    {
        input >> points[i];
    }

    // 颜色
    input >> dumy >> num;
    std::vector<vec3> &colors = point_set->colors();
    colors.resize(num);
    for (int i = 0; i < num; ++i)
    {
        input >> colors[i];
    }

    // 法向量
    input >> dumy >> num;
    std::vector<vec3> &normals = point_set->normals();
    normals.resize(num);
    for (int i = 0; i < num; ++i)
    {
        input >> normals[i];
    }

    std::size_t num_groups = 0;
    input >> dumy >> num_groups;
    for (int i = 0; i < num_groups; ++i)
    {
        // VertexGroup::Ptr g = read_ascii_group(input);
        // if (!g)
        //     continue;

        // if (!g->empty())
        // {
        //     g->set_point_set(pset);
        //     pset->groups().push_back(g);
        // }

        // int num_children = 0;
        // input >> dumy >> num_children;
        // for (int j = 0; j < num_children; ++j)
        // {
        //     VertexGroup::Ptr chld = read_ascii_group(input);
        //     if (!chld->empty())
        //     {
        //         chld->set_point_set(pset);
        //         g->add_child(chld);
        //     }
        // }
    }
}

// TODO: 实现PointSetSerializer的其他读写方法
void PointSetSerializer::save_vg(const std::unique_ptr<PointSet> point_set, const std::string &file_name)
{
    // 实现保存vg文件的功能
    std ::cout << "save vg" << std::endl;
}

void PointSetSerializer::load_bvg(std::unique_ptr<PointSet> point_set, const std::string &file_name)
{
    // 实现加载bvg文件的功能
    std ::cout << "save vg" << std::endl;
}

void PointSetSerializer::save_bvg(const std::unique_ptr<PointSet> point_set, const std::string &file_name)
{
    // 实现保存bvg文件的功能
    std ::cout << "save bvg" << std::endl;
}

VertexGroup *PointSetSerializer::read_ascii_group(std::istream &input)
{
    // 实现读取ascii格式的顶点组的功能
    std ::cout << "read ascii group" << std::endl;
    return nullptr;
}

void PointSetSerializer::write_ascii_group(std::ostream &output, VertexGroup *group)
{
    // 实现写入ascii格式的顶点组的功能
    std ::cout << "write ascii group" << std::endl;
}

VertexGroup *PointSetSerializer::read_binary_group(std::istream &input)
{
    // 实现读取二进制格式的顶点组的功能
    std ::cout << "save vg" << std::endl;
    return nullptr;
}

void PointSetSerializer::write_binary_group(std::ostream &output, VertexGroup *group)
{
    // 实现写入二进制格式的顶点组的功能
    std ::cout << "save vg" << std::endl;
}