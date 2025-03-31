// #pragma once
// #include <memory>
// #include <map>
// #include "point_set.h"
// #include "plane.h"
// #include "basic_types.h"
// #include "simple_attribute.h"
// class PointSet;
// class MapGenerator
// {
// public:
//     typedef Math::vec3 vec3;
//     typedef Math::vec2 vec2;
//     MapGenerator(std::shared_ptr<PointSet> pset);
//     virtual ~MapGenerator();
//     void refine_planes();
//     std::shared_ptr<Map> MapGenerator::generate();

//     struct Intersection
//     {
//         enum Type
//         {
//             EXISTING_VERTEX,
//             NEW_VERTEX
//         };

//         Intersection(Type t) : type(t), {}
//         Type type;

//         // for EXISTING_VERTEX
//         std::shared_ptr<MapTypes::Vertex> vtx;

//         // for NEW_VERTEX
//         std::shared_ptr<MapTypes::Halfedge> edge;
//         vec3 pos;
//     };

// private:
//     void merge(std::shared_ptr<VertexGroup> g1, std::shared_ptr<VertexGroup> g2);
//     void collect_valid_planes();
//     std::shared_ptr<Map> MapGenerator::construct_bbox_mesh();
//     std::shared_ptr<Map> MapGenerator::compute_proxy_mesh(std::shared_ptr<Map> bbox_mesh);
//     void triplet_intersection();
//     void pairwise_cut(std::shared_ptr<Map> mesh);
//     std::set<std::shared_ptr<Plane>> collect_cutting_planes(std::shared_ptr<MapTypes::Facet> face, std::shared_ptr<Map> mesh);
//     bool do_intersect(std::shared_ptr<MapTypes::Facet> f, std::shared_ptr<Plane> plane)

//         std::shared_ptr<PointSet> pset_;
//     std::vector<std::shared_ptr<Plane>> supporting_planes_;
//     std::vector<std::shared_ptr<VertexGroup>> plane_segments_;
//     std::map<std::shared_ptr<VertexGroup>, std::shared_ptr<Plane>> vertex_group_plane_;

//     std::shared_ptr<AttributeHandle<std::shared_ptr<VertexGroup>>> facet_attrib_supporting_vertex_group_;
//     std::shared_ptr<AttributeHandle<std::shared_ptr<Plane>>> facet_attrib_supporting_plane_;
//     std::shared_ptr<AttributeHandle<std::set<std::shared_ptr<Plane>>>> edge_source_planes_;
//     std::shared_ptr<AttributeHandle<std::set<std::shared_ptr<Plane>>>> vertex_source_planes_;

//     bool MapGenerator::intersection_plane_triplet(
//         const std::shared_ptr<Plane> plane1,
//         const std::shared_ptr<Plane> plane2,
//         const std::shared_ptr<Plane> plane3,
//         vec3 &p);
//     std::map<std::shared_ptr<Plane>,
//              std::map<std::shared_ptr<Plane>,
//                       std::map<std::shared_ptr<Plane>, std::shared_ptr<vec3>>>>
//         triplet_intersection_;
//     std::vector<std::shared_ptr<vec3>> intersecting_points_;
// };