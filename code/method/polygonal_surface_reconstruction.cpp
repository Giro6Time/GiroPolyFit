#include "polygonal_surface_reconstruction.h"
#include "logger.h"
#include "map_builder.h"
#include "cgal_types.h"
#include <CGAL/Simple_cartesian.h>
#include <map>
#include "color.h"
Surface_mesh convert_to_mesh(Point_vector &points)
{
    Logger::out("Polygonal_surface_reconstruction") << "Converting to mesh" << std::endl;
    Polygonal_surface_reconstruction algo(
        points,
        Point_map(),
        Normal_map(),
        Plane_index_map());

    Logger::out("Polygonal_surface_reconstruction") << " Done." << std::endl;

    Surface_mesh model;

    Logger::out("Polygonal_surface_reconstruction") << "Reconstructing..." << std::endl;

    if (!algo.reconstruct<MIP_Solver>(model))
    {
        Logger::error("Polygonal_surface_reconstruction") << " Failed: " << algo.error_message() << std::endl;
        return model;
    }
    const std::string &output_file("example.obj");
    if (CGAL::IO::write_OBJ(output_file, model))
        Logger::out("Polygonal_surface_reconstruction") << " Done. Saved to " << output_file << std::endl;
    else
    {
        Logger::error("Polygonal_surface_reconstruction") << " Failed saving file." << std::endl;
        return model;
    }
    return model;
}

std::shared_ptr<Map> convert_to_map(Surface_mesh &model)
{
    PMP::triangulate_faces(model);
    bool has_non_triangular = false;
    for (auto face : model.faces())
    {
        if (model.degree(face) > 3)
            has_non_triangular = true;
    }

    if (has_non_triangular)
    {
        Logger::error("Convert_to_map") << "Some faces could not be triangulated." << std::endl;
        return nullptr;
    }

    std::shared_ptr<Map> map = std::make_shared<Map>();
    std::shared_ptr<MapBuilder> builder = std::make_shared<MapBuilder>(map);
    auto color = builder->get_facet_attribute_handle<Color>("color");
    std::map<CGAL::SM_Vertex_index, int> m;
    builder->begin_surface();
    int cnt = 0;
    for (auto it : model.vertices())
    {
        builder->add_vertex(to_my_point(model.point(it)));
        m[it] = cnt++;
    }
    for (auto it : model.faces())
    {
        builder->begin_facet();
        auto hf = model.halfedge(it);
        if (hf == Surface_mesh::Halfedge_index())
        {
            // 非法面，跳过
            continue;
        }
        for (auto jt : model.vertices_around_face(hf))
        {
            builder->add_vertex_to_facet(m[jt]);
        }
        builder->end_facet();
        color->set(builder->current_facet()->id, Color(0.7f, 0.7f, 0.7f, 1.0f));
    }
    return map;
}