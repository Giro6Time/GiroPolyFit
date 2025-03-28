﻿#include "point_set_serializer.h"

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

void PointSetSerializer::load_vg(std::shared_ptr<PointSet> point_set, const std::string &file_name)
{

    std::ifstream input = std::ifstream(file_name.c_str());
    if (input.fail())
    {
        Logger::error("PointSetSerializer") << "could not open file \"" << file_name << "\"" << std::endl;
        return;
    }

    input.seekg(0, input.end);
    std::streamoff length = input.tellg();
    input.seekg(0, input.beg);
    ProgressLogger progress(length);

    // dumy表示当前读取的数据类型（如point，color等）
    std::string dumy;
    std::size_t num;
    std::string dbgstr;

    // 读入点坐标
    input >> dumy >> num;
    progress.next();
    std::vector<vec3> &points = point_set->points();
    points.resize(num);

    for (int i = 0; i < num; ++i)
    {
        input >> points[i];
        progress.next();
    }

    // 颜色
    input >> dumy >> num;
    progress.next();
    std::vector<vec3> &colors = point_set->colors();
    colors.resize(num);
    for (int i = 0; i < num; ++i)
    {
        input >> colors[i];
        progress.next();
    }

    // 法向量
    input >> dumy >> num;
    progress.next();
    std::vector<vec3> &normals = point_set->normals();
    normals.resize(num);
    for (int i = 0; i < num; ++i)
    {
        input >> normals[i];
        progress.next();
    }

    std::size_t num_groups = 0;
    input >> dumy >> num_groups;
    progress.next();
    for (int i = 0; i < num_groups; ++i)
    {
        std::shared_ptr<VertexGroup> g = read_ascii_group(input);
        if (!g)
            continue;

        if (!g->empty())
        {
            g->set_point_set(point_set);
            point_set->groups().push_back(g);
        }

        int num_children = 0;
        input >> dumy >> num_children;
        for (int j = 0; j < num_children; ++j)
        {
            std::shared_ptr<VertexGroup> chld = read_ascii_group(input);
            if (!chld->empty())
            {
                chld->set_point_set(point_set);
                g->add_child(chld);
            }
        }
        std::streamoff pos = input.tellg();
        progress.notify(pos);
    }
}

void PointSetSerializer::save_vg(const std::shared_ptr<PointSet> point_set, const std::string &file_name)
{
    std::ofstream output(file_name);
    if (!output.is_open())
    {
        Logger::error("PointSetSerializer") << "could not open file \"" << file_name << "\"" << std::endl;
        return;
    }
    output.precision(16);
    const std::vector<vec3> &points = point_set->points();
    const std::vector<vec3> &colors = point_set->colors();
    const std::vector<vec3> &normals = point_set->normals();
    const std::vector<std::shared_ptr<VertexGroup>> &groups = point_set->groups();

    ProgressLogger progress(points.size() + colors.size() + normals.size() + groups.size());

    // 记录点坐标
    output << "num_points: " << points.size() << std::endl;
    for (const auto &p : points)
    {
        output << p << std::endl;
        progress.next();
    }
    output << std::endl;

    // 记录颜色到文件中
    output << "num_colors: " << colors.size() << std::endl;
    for (const auto &c : colors)
    {
        output << c << std::endl;
        progress.next();
    }
    output << std::endl;

    // 记录法向量到文件中
    output << "num_normals: " << normals.size() << std::endl;
    for (const auto &n : normals)
    {
        output << n << std::endl;
        progress.next();
    }
    output << std::endl;

    // 记录群组信息到文件中
    output << "num_groups: " << groups.size() << std::endl
           << std::endl;
    for (const auto &g : groups)
    {
        // 写入当前群组的信息
        write_ascii_group(output, g);
        output << std::endl
               << std::endl;

        // 处理子群组
        const std::vector<std::shared_ptr<VertexGroup>> children = g->children();
        output << "num_children: " << children.size() << std::endl
               << std::endl;
        for (const auto &child : children)
        {
            if (!child->empty())
            {
                write_ascii_group(output, child);
                output << std::endl;
            }
        }
        progress.next();
    }

    // 关闭输出文件
    output.close();
}

// TODO: 实现PointSetSerializer的其他读写方法

void PointSetSerializer::load_bvg(std::shared_ptr<PointSet> point_set, const std::string &file_name)
{
    // 实现加载bvg文件的功能
    std ::cout << "save vg" << std::endl;
}

void PointSetSerializer::save_bvg(const std::shared_ptr<PointSet> point_set, const std::string &file_name)
{
    // 实现保存bvg文件的功能
    std ::cout << "save bvg" << std::endl;
}

std::shared_ptr<VertexGroup> PointSetSerializer::read_ascii_group(std::istream &input)
{
    std::string dumy;
    int type;
    input >> dumy >> type;

    int num;
    input >> dumy >> num;
    if (num != 4)
        return nullptr; // bad/unknown data

    std::vector<float> para(num);
    input >> dumy;
    for (int i = 0; i < num; ++i)
        input >> para[i];

    std::string label;
    input >> dumy >> label;

    float r, g, b;
    input >> dumy >> r >> g >> b;
    Color color(r, g, b);

    int num_points;
    input >> dumy >> num_points;

    std::shared_ptr<VertexGroup> grp = std::make_shared<VertexGroup>();

    my_assert(para.size() == 4);
    grp->set_plane(Plane(para[0], para[1], para[2], para[3]));

    for (int i = 0; i < num_points; ++i)
    {
        int idx;
        input >> idx;
        grp->push_back(idx);
    }

    grp->set_label(label);
    grp->set_color(color);

    return grp;
}

void PointSetSerializer::write_ascii_group(std::ostream &output, std::shared_ptr<VertexGroup> group)
{
    // 实现写入ascii格式的顶点组的功能
    {
        // 输出后把vg都修改为plane
        int type = 0;
        output << "group_type: " << type << std::endl;

        const std::vector<float> &para = get_group_parameters(group);
        output << "num_group_parameters: " << para.size() << std::endl;
        output << "group_parameters: ";
        for (std::size_t i = 0; i < para.size() - 1; ++i)
            output << para[i] << " ";
        output << para[para.size() - 1] << std::endl;

        std::string label = group->label();
        output << "group_label: " << label << std::endl;

        Color c = group->color();
        output << "group_color: " << c.r() << " " << c.g() << " " << c.b() << std::endl;

        std::size_t num_point = group->size();
        output << "group_num_points: " << num_point << std::endl;

        for (std::size_t i = 0; i < group->size() - 1; ++i)
        {
            output << group->at(i) << " ";
        }
        output << group->at(group->size() - 1);
    }
}

std::shared_ptr<VertexGroup> PointSetSerializer::read_binary_group(std::istream &input)
{
    // 实现读取二进制格式的顶点组的功能
    std ::cout << "save vg" << std::endl;
    return nullptr;
}

void PointSetSerializer::write_binary_group(std::ostream &output, std::shared_ptr<VertexGroup> group)
{
    // 实现写入二进制格式的顶点组的功能
    std ::cout << "save vg" << std::endl;
}

std::vector<float> PointSetSerializer::get_group_parameters(std::shared_ptr<VertexGroup> g)
{
    int num = 4;
    std::vector<float> para(num);

    para[0] = static_cast<float>(g->plane().a());
    para[1] = static_cast<float>(g->plane().b());
    para[2] = static_cast<float>(g->plane().c());
    para[3] = static_cast<float>(g->plane().d());

    return para;
}

void PointSetSerializer::load_from_pcl_pointcloud(std::shared_ptr<PointSet> point_set, const pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
    for (int i = 0; i < cloud->size(); i++)
    {
            point_set->points().push_back(vec3(cloud->points[i].x, cloud->points[i].y, cloud->points[i].z));
            point_set->colors().push_back(vec3(1, 1, 1));

    }
}
