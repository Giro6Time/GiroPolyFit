#include "map_io.h"
#include "map.h"
#include "map_serializer.h"
#include "utils.h"
#include "basic_types.h"

std::shared_ptr<Map> MapIO::read(std::string &filename)
{
    auto serializer = resolve_serializer(filename);
    if (serializer)
    {
        auto mesh = std::make_shared<Map>();

        Logger::out("-") << "reading file ..." << std::endl;

        if (serializer->serialize_read(filename, mesh))
        {
            // Logger::out("-") << "done. " << w.elapsed() << " sec." << std::endl; 监听时间
            return mesh;
        }
        else
        {
            mesh.reset();
            Logger::error("-") << "reading file failed" << std::endl;
        }
    }
    return nullptr;
}

bool MapIO::save(const std::string &filename, std::shared_ptr<Map> map)
{
    if (!map)
    {
        Logger::error("MapIO") << "Map is null, cannot save to file" << std::endl;
        return false;
    }

    std::ofstream output(filename);
    if (!output.is_open())
    {
        Logger::error("MapIO") << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }

    Logger::out("MapIO") << "Saving map to OBJ file: " << filename << std::endl;

    // 写入顶点信息
    for (auto vertex_it = map->vertices_begin(); vertex_it != map->vertices_end(); ++vertex_it)
    {
        auto vertex = *vertex_it;
        const auto &point = vertex->point();
        output << "v " << point.x << " " << point.y << " " << point.z << "\n";
    }

    // 写入面信息
    for (auto facet_it = map->facets_begin(); facet_it != map->facets_end(); ++facet_it)
    {
        auto facet = *facet_it;
        auto start_halfedge = facet->halfedge();
        auto current_halfedge = start_halfedge;

        output << "f";
        do
        {
            auto vertex = current_halfedge->vertex();
            output << " " << (vertex->id + 1); // OBJ 文件中的索引从 1 开始
            current_halfedge = current_halfedge->next();
        } while (current_halfedge != start_halfedge);
        output << "\n";
    }

    // 写入半边信息（附加在文件底部）
    output << "# Halfedge information\n";
    for (auto halfedge_it = map->halfedges_begin(); halfedge_it != map->halfedges_end(); ++halfedge_it)
    {
        auto halfedge = *halfedge_it;
        auto vertex = halfedge->vertex();
        auto next = halfedge->next();
        auto opposite = halfedge->opposite();
        auto facet = halfedge->facet();

        output << "he " << halfedge->id
               << " v:" << (vertex ? vertex->id : -1)
               << " next:" << (next ? next->id : -1)
               << " opp:" << (opposite ? opposite->id : -1)
               << " f:" << (facet ? facet->id : -1)
               << "\n";
    }

    output.close();
    Logger::out("MapIO") << "Map successfully saved to: " << filename << std::endl;
    return true;
}

std::shared_ptr<MapSerializer_obj> MapIO::resolve_serializer(const std::string &file_name)
{
    std::string extension = FileUtils::extension(file_name);

    if (extension.length() == 0)
    {
        Logger::error("MapIO") << "No extension in file name" << std::endl;
        return nil;
    }

    std::shared_ptr<MapSerializer_obj> serializer;

    if (extension == "obj")
        serializer = std::make_shared<MapSerializer_obj>();
    else if (extension == "eobj")
        serializer = std::make_shared<MapSerializer_obj>(); // TODO 拓展更多可文件类型
    else
    {
        Logger::error("-") << "unknown file format" << std::endl;
        return nullptr;
    }

    return serializer;
}
