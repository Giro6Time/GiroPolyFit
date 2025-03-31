// #include "map_generator.h"
// #include "vertex_group.h"
// #include "basic_types.h"
// #include "logger.h"
// #include <algorithm>
// #include <list>
// #include "color.h"
// #include "map_builder.h"
// #include "map_cell.h"
// #include "math_types.h"
// #include "line.h"
// #include <CGAL/Projection_traits_xy_3.h>
// #include <CGAL/convex_hull_2.h>
// #include "cgal_types.h"
// #include "progress.h"
// MapGenerator::MapGenerator(std::shared_ptr<PointSet> pset) : pset_(pset)
// {
// }

// MapGenerator::~MapGenerator() {}

// void MapGenerator::refine_planes()
// {
//     std::vector<std::shared_ptr<VertexGroup>> groups = pset_->groups();
//     const std::vector<Math::vec3> &points = pset_->points();

//     std::size_t num = groups.size();

//     float avg_max_dist = 0;
//     for (std::size_t i = 0; i < groups.size(); ++i)
//     {
//         auto g = groups[i];
//         pset_->fit_plane(g);

//         float g_max_dist = -FLT_MAX;
//         const Plane &plane = g->plane();
//         for (std::size_t j = 0; j < g->size(); ++j)
//         {
//             int idx = g->at(j);
//             const Math::vec3 &p = points[idx];
//             float sdist = plane.distance2(p);
//             g_max_dist = std::max(g_max_dist, std::sqrt(sdist));
//         }

//         avg_max_dist += g_max_dist;
//     }
//     avg_max_dist /= groups.size();
//     avg_max_dist /= 2.0f;

//     float theta = 10.0f;                               // in degree
//     theta = static_cast<float>(M_PI * theta / 180.0f); // in radian
//     bool merged = false;
//     do
//     {
//         merged = false;
//         std::sort(groups.begin(), groups.end(), VertexGroupCmpIncreasing());

//         for (std::size_t i = 0; i < groups.size(); ++i)
//         {
//             std::shared_ptr<VertexGroup> g1 = groups[i];
//             const Plane &plane1 = g1->plane();
//             const Math::vec3 &n1 = plane1.normal();
//             float num_threshold = g1->size() / 5.0f;
//             for (std::size_t j = i + 1; j < groups.size(); ++j)
//             {
//                 std::shared_ptr<VertexGroup> g2 = groups[j];
//                 const Plane &plane2 = g2->plane();
//                 const Math::vec3 &n2 = plane2.normal();
//                 if (std::abs(dot(n1, n2)) > std::cos(theta))
//                 {
//                     const std::list<unsigned int> &set1on2 = points_on_plane(g1, plane2, avg_max_dist);
//                     const std::list<unsigned int> &set2on1 = points_on_plane(g2, plane1, avg_max_dist);
//                     if (set1on2.size() > num_threshold || set2on1.size() > num_threshold)
//                     {
//                         merge(g1, g2);
//                         merged = true;
//                         break;
//                     }
//                 }
//             }
//             if (merged)
//                 break;
//         }
//     } while (merged);

//     std::sort(groups.begin(), groups.end(), VertexGroupCmpDecreasing());

//     if (num - groups.size() > 0)
//     {
//         Logger::out("-") << num - groups.size() << " planar segments merged" << std::endl;
//     }
// }

// static std::list<unsigned int> points_on_plane(std::shared_ptr<VertexGroup> g, const Plane &plane, float dist_threshold)
// {
//     std::list<unsigned int> result;

//     const std::vector<Math::vec3> &points = g->point_set()->points();
//     for (std::size_t i = 0; i < g->size(); ++i)
//     {
//         int idx = g->at(i);
//         const Math::vec3 &p = points[idx];

//         float sdist = plane.distance2(p);
//         float dist = std::sqrt(sdist);
//         if (dist < dist_threshold)
//             result.push_back(idx);
//     }
//     return result;
// }

// void MapGenerator::merge(std::shared_ptr<VertexGroup> g1, std::shared_ptr<VertexGroup> g2)
// {
//     std::vector<std::shared_ptr<VertexGroup>> &groups = pset_->groups();
//     const std::vector<Math::vec3> &points = pset_->points();

//     std::vector<unsigned int> points_indices;
//     points_indices.insert(points_indices.end(), g1->begin(), g1->end());
//     points_indices.insert(points_indices.end(), g2->begin(), g2->end());

//     std::shared_ptr<VertexGroup> g = std::make_shared<VertexGroup>();
//     g->insert(g->end(), points_indices.begin(), points_indices.end());
//     g->set_point_set(pset_);
//     g->set_color(Color::fused_color(g1->color(), static_cast<float>(g1->size()), g2->color(), static_cast<float>(g2->size())));
//     pset_->fit_plane(g);
//     groups.push_back(g);

//     std::vector<std::shared_ptr<VertexGroup>>::iterator pos = std::find(groups.begin(), groups.end(), g1);
//     if (pos != groups.end())
//     {
//         groups.erase(pos);
//     }
//     else
//     {
//         Logger::error("-") << "fatal error: vertex group doesn't exist" << std::endl;
//     }

//     pos = std::find(groups.begin(), groups.end(), g2);
//     if (pos != groups.end())
//     {
//         groups.erase(pos);
//     }
//     else
//     {
//         Logger::error("-") << "fatal error: vertex group doesn't exist" << std::endl;
//     }
// }

// std::shared_ptr<Map> MapGenerator::generate()
// {
//     if (!pset_)
//         return nil;

//     if (pset_->groups().empty())
//     {
//         Logger::warn("-") << "planar segments do not exist" << std::endl;
//         return nil;
//     }

//     collect_valid_planes();

//     std::shared_ptr<Map> bbox_mesh = construct_bbox_mesh();
//     std::shared_ptr<Map> mesh = compute_proxy_mesh(bbox_mesh);
//     if (!mesh)
//         return nil;

//     check_source_planes(mesh);

//     if (!facet_attrib_supporting_vertex_group_)
//         facet_attrib_supporting_vertex_group_ = mesh->facet_attribute_store()->create_handle<std::shared_ptr<VertexGroup>>("facet_supporting_vertex_group");
//     if (!facet_attrib_supporting_plane_)
//         facet_attrib_supporting_plane_ = mesh->facet_attribute_store()->create_handle<std::shared_ptr<Plane>>("FacetSupportingPlane");
//     if (!edge_source_planes_)
//         edge_source_planes_ = mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("EdgeSourcePlanes");
//     if (!vertex_source_planes_)
//         vertex_source_planes_ = mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("VertexSourcePlanes");

//     triplet_intersection();
//     pairwise_cut(mesh);
//     check_source_planes(mesh);

//     remove_degenerated_facets(mesh);
//     check_source_planes(mesh);

//     // facet_attrib_supporting_vertex_group_.unbind();
//     // facet_attrib_supporting_plane_.unbind();
//     // edge_source_planes_.unbind();
//     // vertex_source_planes_.unbind();

//     return mesh;
// }

// void MapGenerator::collect_valid_planes()
// {
//     supporting_planes_.clear();
//     plane_segments_.clear();
//     vertex_group_plane_.clear();

//     std::vector<std::shared_ptr<VertexGroup>> &groups = pset_->groups();
//     for (std::size_t i = 0; i < groups.size(); ++i)
//     {
//         std::shared_ptr<VertexGroup> g = groups[i];
//         pset_->fit_plane(g);

//         plane_segments_.push_back(g);
//         std::shared_ptr<Plane> plane = std::make_shared<Plane>(g->plane());
//         supporting_planes_.push_back(plane);
//         vertex_group_plane_[g] = plane;
//     }
// }

// std::shared_ptr<Map> MapGenerator::construct_bbox_mesh()
// {
//     Box3d box = pset_->bbox();
//     float delta = box.radius() * 0.05f;

//     std::shared_ptr<Map> mesh = std::make_shared<Map>();
//     std::shared_ptr<MapBuilder> builder = std::make_shared<MapBuilder>(mesh);

//     auto face_supporting_plane = mesh->facet_attribute_store()->create_handle<std::shared_ptr<Plane>>("FacetSupportingPlane");
//     auto edge_source_planes = mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("EdgeSourcePlanes");
//     auto vertex_source_planes = mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("VertexSourcePlanes");

//     float xmin = box.x_min() - delta, xmax = box.x_max() + delta;
//     float ymin = box.y_min() - delta, ymax = box.y_max() + delta;
//     float zmin = box.z_min() - delta, zmax = box.z_max() + delta;

//     builder->begin_surface();

//     builder->add_vertex(vec3(xmin, ymin, zmin)); // 0
//     builder->add_vertex(vec3(xmax, ymin, zmin)); // 1
//     builder->add_vertex(vec3(xmax, ymin, zmax)); // 2
//     builder->add_vertex(vec3(xmin, ymin, zmax)); // 3
//     builder->add_vertex(vec3(xmax, ymax, zmax)); // 4
//     builder->add_vertex(vec3(xmax, ymax, zmin)); // 5
//     builder->add_vertex(vec3(xmin, ymax, zmin)); // 6
//     builder->add_vertex(vec3(xmin, ymax, zmax)); // 7

//     builder->begin_facet();
//     builder->add_vertex_to_facet(0);
//     builder->add_vertex_to_facet(1);
//     builder->add_vertex_to_facet(2);
//     builder->add_vertex_to_facet(3);
//     builder->end_facet();
//     std::shared_ptr<MapTypes::Facet> f = builder->current_facet();
//     std::shared_ptr<Plane> plane = std::make_shared<Plane>(GeometryTypes::facet_plane(f));
//     supporting_planes_.push_back(plane);
//     face_supporting_plane->set(f->id, plane);

//     builder->begin_facet();
//     builder->add_vertex_to_facet(1);
//     builder->add_vertex_to_facet(5);
//     builder->add_vertex_to_facet(4);
//     builder->add_vertex_to_facet(2);
//     builder->end_facet();
//     f = builder->current_facet();
//     plane = std::make_shared<Plane>(GeometryTypes::facet_plane(f));
//     supporting_planes_.push_back(plane);
//     face_supporting_plane->set(f->id, plane);

//     builder->begin_facet();
//     builder->add_vertex_to_facet(1);
//     builder->add_vertex_to_facet(0);
//     builder->add_vertex_to_facet(6);
//     builder->add_vertex_to_facet(5);
//     builder->end_facet();
//     f = builder->current_facet();
//     plane = std::make_shared<Plane>(GeometryTypes::facet_plane(f));
//     supporting_planes_.push_back(plane);
//     face_supporting_plane->set(f->id, plane);

//     builder->begin_facet();
//     builder->add_vertex_to_facet(4);
//     builder->add_vertex_to_facet(5);
//     builder->add_vertex_to_facet(6);
//     builder->add_vertex_to_facet(7);
//     builder->end_facet();
//     f = builder->current_facet();
//     plane = std::make_shared<Plane>(GeometryTypes::facet_plane(f));
//     supporting_planes_.push_back(plane);
//     face_supporting_plane->set(f->id, plane);

//     builder->begin_facet();
//     builder->add_vertex_to_facet(0);
//     builder->add_vertex_to_facet(3);
//     builder->add_vertex_to_facet(7);
//     builder->add_vertex_to_facet(6);
//     builder->end_facet();
//     f = builder->current_facet();
//     plane = std::make_shared<Plane>(GeometryTypes::facet_plane(f));
//     supporting_planes_.push_back(plane);
//     face_supporting_plane->set(f->id, plane);

//     builder->begin_facet();
//     builder->add_vertex_to_facet(2);
//     builder->add_vertex_to_facet(4);
//     builder->add_vertex_to_facet(7);
//     builder->add_vertex_to_facet(3);
//     builder->end_facet();
//     f = builder->current_facet();
//     plane = std::make_shared<Plane>(GeometryTypes::facet_plane(f));
//     supporting_planes_.push_back(plane);
//     face_supporting_plane->set(f->id, plane);

//     builder->end_surface();

//     // assign the original planes for each edge
//     for (auto it = mesh->halfedges_begin(); it != mesh->halfedges_end(); it++)
//     {
//         std::shared_ptr<Plane> plane1 = *face_supporting_plane->get((*it)->id);
//         std::shared_ptr<Plane> plane2 = *face_supporting_plane->get((*it)->opposite()->id);
//         edge_source_planes->get((*it)->id)->insert(plane1);
//         edge_source_planes->get((*it)->id)->insert(plane2);
//     }

//     // assign the original planes for each vertex
//     for (auto it = mesh->vertices_begin(); it != mesh->vertices_end(); it++)
//     {
//         int cnt = 0;
//         auto step = (*it);
//         do
//         {
//             std::shared_ptr<Plane> plane = *face_supporting_plane->get((step)->halfedge()->vertex()->id);
//             vertex_source_planes->get(step->id)->insert(plane);
//             cnt++;
//         } while (step != (*it));
//         if (cnt != 3)
//             Logger::error("-") << "fatal_error. A bbox mesh corner does not relate to 3 planes" << std::endl;
//     }

//     return mesh;
// }

// std::shared_ptr<Map> MapGenerator::compute_proxy_mesh(std::shared_ptr<Map> bbox_mesh)
// {

//     auto bbox_mesh_face_supporting_plane = bbox_mesh->facet_attribute_store()->create_handle<std::shared_ptr<Plane>>("FacetSupportingPlane");
//     auto bbox_mesh_edge_source_planes = bbox_mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("EdgeSourcePlanes");
//     auto bbox_mesh_vertex_source_planes = bbox_mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("VertexSourcePlanes");

//     std::shared_ptr<Map> mesh = std::make_shared<Map>();
//     std::shared_ptr<MapBuilder> builder = std::make_shared<MapBuilder>(mesh);

//     auto color = mesh->facet_attribute_store()->create_handle<Color>("color");
//     auto facet_supporting_vertex_group = mesh->facet_attribute_store()->create_handle<std::shared_ptr<VertexGroup>>("facet_supporting_vertex_group");
//     auto face_supporting_plane = mesh->facet_attribute_store()->create_handle<std::shared_ptr<Plane>>("FacetSupportingPlane");
//     auto edge_source_planes = mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("EdgeSourcePlanes");
//     auto vertex_source_planes = mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("VertexSourcePlanes");

//     builder->begin_surface();
//     int idx = 0;
//     for (std::size_t i = 0; i < plane_segments_.size(); ++i)
//     {
//         std::shared_ptr<VertexGroup> g = plane_segments_[i];
//         std::shared_ptr<Plane> plane = vertex_group_plane_[g];

//         std::vector<vec3> points;
//         std::vector<std::set<std::shared_ptr<Plane>>> point_source_planes;
//         for (auto it = bbox_mesh->halfedges_begin(); it != bbox_mesh->halfedges_end(); it++)
//         {
//             const vec3 &s = (*it)->prev()->vertex()->point();
//             const vec3 &t = (*it)->vertex()->point();
//             Sign ss = plane->orient(s);
//             Sign st = plane->orient(t);
//             if ((ss == POSITIVE && st == NEGATIVE) || (ss == NEGATIVE && st == POSITIVE))
//             {
//                 vec3 p;
//                 if (plane->intersection(Line3d::from_two_points(s, t), p))
//                 {
//                     points.push_back(p);

//                     std::set<std::shared_ptr<Plane>> planes = *bbox_mesh_edge_source_planes->get((*it)->id);
//                     planes.insert(plane);
//                     point_source_planes.push_back(planes);
//                 }
//                 else
//                     Logger::error("-") << "fatal error. Should have intersection" << std::endl;
//             }
//             else
//             {
//                 if (ss == ZERO)
//                 {
//                     points.push_back(s);

//                     std::set<std::shared_ptr<Plane>> planes = *bbox_mesh_vertex_source_planes->get((*it)->prev()->vertex()->id);
//                     point_source_planes.push_back(planes);
//                 }
//                 else if (st == ZERO)
//                 {
//                     points.push_back(t);

//                     std::set<std::shared_ptr<Plane>> planes = *bbox_mesh_vertex_source_planes->get((*it)->vertex()->id);
//                     point_source_planes.push_back(planes);
//                 }
//                 else
//                 {
//                     // no intersection with the plane
//                 }
//             }
//         }

//         if (points.size() >= 3)
//         {
//             std::list<Point3> pts;
//             for (std::size_t i = 0; i < points.size(); ++i)
//             {
//                 const vec3 &p = points[i];
//                 vec2 q = plane->to_2d(p);
//                 pts.push_back(Point3(q.x, q.y, double(i))); // trick: put the point index as the 'z' component
//             }

//             typedef CGAL::Projection_traits_xy_3<K> Projection;

//             std::list<Point3> hull;
//             CGAL::convex_hull_2(pts.begin(), pts.end(), std::back_inserter(hull), Projection());

//             std::vector<vec3> ch;
//             std::vector<std::set<std::shared_ptr<Plane>>> ch_source_planes;

//             for (std::list<Point3>::iterator it = hull.begin(); it != hull.end(); ++it)
//             {
//                 int idx = int(it->z());
//                 ch.push_back(points[idx]);
//                 ch_source_planes.push_back(point_source_planes[idx]);
//             }

//             if (ch.size() >= 3)
//             {
//                 for (std::size_t j = 0; j < ch.size(); ++j)
//                 {
//                     builder->add_vertex(ch[j]);
//                     std::shared_ptr<MapTypes::Vertex> v = builder->current_vertex();
//                     vertex_source_planes->set(v->id, ch_source_planes[j]);
//                 }
//                 builder->begin_facet();
//                 for (int k = idx; k < idx + ch.size(); ++k)
//                     builder->add_vertex_to_facet(k);
//                 builder->end_facet();

//                 std::shared_ptr<Map::Facet> f = builder->current_facet();
//                 color->set(f->id, g->color());
//                 facet_supporting_vertex_group->set(f->id, g);
//                 face_supporting_plane->set(f->id, plane);

//                 // assign each edge the source planes

//                 {
//                     auto step = f->halfedge();
//                     do
//                     {
//                         edge_source_planes->get(step->id)->insert(plane);
//                         for (auto it = bbox_mesh->facets_begin(); it != bbox_mesh->facets_end(); it++)
//                         {
//                             std::shared_ptr<Plane> bbox_plane = *bbox_mesh_face_supporting_plane->get((*it)->id);
//                             if ((bbox_plane->distance2(step->vertex()->point()) < 1e-6) && (bbox_plane->distance2(step->prev()->vertex()->point()) < 1e-6))
//                             {
//                                 edge_source_planes->get(step->id)->insert(bbox_plane);
//                                 break;
//                             }
//                             std::set<std::shared_ptr<Plane>> tmp = *edge_source_planes->get(step->id);
//                             if (tmp.size() != 2)
//                             {
//                                 Logger::error("-") << "fatal error: edge_source_planes[h].size() != 2. Size = " << tmp.size() << std::endl;
//                             }
//                         }
//                     } while (step != f->halfedge());
//                 }

//                 idx += int(ch.size());
//             }
//             else
//             {
//                 Logger::error("-") << "fatal error. Check if this is a degenerate case" << std::endl;
//                 std::cout << "output CH points: " << ch.size() << std::endl;
//                 for (std::size_t i = 0; i < ch.size(); ++i)
//                 {
//                     const vec3 &p = ch[i];
//                     std::cout << "\t" << p << std::endl;
//                 }
//             }
//         }
//         else
//         {
//             Logger::error("-") << "fatal error. Check if this is a degenerate case" << std::endl;
//         }
//     }

//     builder->end_surface();

//     for (auto it = mesh->halfedges_begin(); it != mesh->halfedges_end(); it++)
//     {
//         const std::set<std::shared_ptr<Plane>> &tmp = *edge_source_planes->get((*it)->id);
//         if (tmp.size() == 2 && edge_source_planes->get((*it)->opposite()->id)->size() != 2)
//             edge_source_planes->set((*it)->opposite()->id, tmp);
//     }

//     check_source_planes(bbox_mesh);
//     check_source_planes(mesh);

//     return mesh;
// }

// static void check_source_planes(std::shared_ptr<Map> mesh)
// {
//     auto face_supporting_plane = mesh->facet_attribute_store()->create_handle<std::shared_ptr<Plane>>("FacetSupportingPlane");
//     auto edge_source_planes = mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("EdgeSourcePlanes");
//     auto vertex_source_planes = mesh->facet_attribute_store()->create_handle<std::set<std::shared_ptr<Plane>>>("VertexSourcePlanes");

//     for (auto it = mesh->facets_begin(); it != mesh->facets_end(); it++)
//     {
//         if (!face_supporting_plane->contain((*it)->id))
//             std::cerr << "fatal error: face_supporting_plane[it] == nil" << std::endl;
//     }

//     for (auto it = mesh->halfedges_begin(); it != mesh->halfedges_end(); it++)
//     {
//         const std::set<std::shared_ptr<Plane>> &tmp = *edge_source_planes->get((*it)->id);
//         if (tmp.size() != 2)
//             std::cerr << "fatal error: edge_source_planes[it].size() != 2. Size = " << tmp.size() << std::endl;
//     }

//     for (auto it = mesh->vertices_begin(); it != mesh->vertices_end(); it++)
//     {
//         const std::set<std::shared_ptr<Plane>> &tmp = *edge_source_planes->get((*it)->id);
//         if (tmp.size() != 3)
//             std::cerr << "vertex_source_planes[it].size() != 3. Size = " << tmp.size() << std::endl;
//     }
// }

// bool MapGenerator::intersection_plane_triplet(const std::shared_ptr<Plane> plane1, const std::shared_ptr<Plane> plane2, const std::shared_ptr<Plane> plane3, vec3 &p)
// {
//     if (!plane1 || !plane2 || !plane3)
//     {
//         Logger::error("-") << "null planes" << std::endl;
//         return false;
//     }
//     if (plane1 == plane2 || plane2 == plane3)
//     {
//         Logger::error("-") << "identical planes" << std::endl;
//         return false;
//     }

//     CGAL::Object obj = CGAL::intersection(to_cgal_plane(*plane1), to_cgal_plane(*plane2), to_cgal_plane(*plane3));

//     // pt is the intersection point of the 3 planes
//     if (const Point3 *pt = CGAL::object_cast<Point3>(&obj))
//     {
//         p = to_my_point(*pt);
//         return true;
//     }
//     else if (const Plane3 *plane = CGAL::object_cast<Plane3>(&obj))
//     {
//         Logger::warn("-") << "3 faces lie on the same supporting plane" << std::endl;
//         return false;
//     }
//     else if (const Line3 *line = CGAL::object_cast<Line3>(&obj))
//     {
//         Logger::warn("-") << "3 faces intersect at the same line" << std::endl;
//         return false;
//     }

//     return false;
// }

// void MapGenerator::triplet_intersection()
// {
//     triplet_intersection_.clear();

//     std::sort(supporting_planes_.begin(), supporting_planes_.end());

//     for (std::size_t i = 0; i < supporting_planes_.size(); ++i)
//     {
//         std::shared_ptr<Plane> plane1 = supporting_planes_[i];
//         for (std::size_t j = i + 1; j < supporting_planes_.size(); ++j)
//         {
//             std::shared_ptr<Plane> plane2 = supporting_planes_[j];
//             for (std::size_t k = j + 1; k < supporting_planes_.size(); ++k)
//             {
//                 std::shared_ptr<Plane> plane3 = supporting_planes_[k];

//                 my_assert(plane1 < plane2);
//                 my_assert(plane2 < plane3);

//                 vec3 p;
//                 if (intersection_plane_triplet(plane1, plane2, plane3, p))
//                 {
//                     auto new_point = std::make_shared<vec3>(p);
//                     triplet_intersection_[plane1][plane2][plane3] = new_point; // store the intersection in our data base
//                     intersecting_points_.push_back(new_point);
//                 }
//             }
//         }
//     }
// }

// void MapGenerator::pairwise_cut(std::shared_ptr<Map> mesh)
// {
//     std::vector<std::shared_ptr<MapTypes::Facet>> all_faces;
//     for (auto it = mesh->facets_begin(); it != mesh->facets_end(); it++)
//     {
//         all_faces.push_back(*it);
//     }

//     std::map<std::shared_ptr<MapTypes::Facet>, std::set<std::shared_ptr<Plane>>> face_cutters;
//     for (std::size_t i = 0; i < all_faces.size(); ++i)
//     {
//         std::shared_ptr<MapTypes::Facet> f = all_faces[i];
//         face_cutters[f] = collect_cutting_planes(f, mesh);
//     }

//     ProgressLogger progress(all_faces.size());
//     for (std::size_t i = 0; i < all_faces.size(); ++i)
//     {
//         std::shared_ptr<MapTypes::Facet> f = all_faces[i];

//         std::set<std::shared_ptr<Plane>> &cutting_planes = face_cutters[f];
//         if (cutting_planes.empty())
//             continue;

//         // f will be cut by all the intersecting_faces
//         // note: after each cut, the original face doesn't exist any more and it is replaced by multiple pieces.
//         //       then each piece will be cut by another face.
//         std::vector<std::shared_ptr<MapTypes::Facet>> faces_to_be_cut;
//         faces_to_be_cut.push_back(f);
//         while (!cutting_planes.empty())
//         {
//             std::set<std::shared_ptr<MapTypes::Facet>> new_faces;      // stores the new faces
//             std::set<std::shared_ptr<MapTypes::Facet>> remained_faces; // faces that will be cut later
//             std::shared_ptr<Plane> cutter = *(cutting_planes.begin());
//             for (std::size_t j = 0; j < faces_to_be_cut.size(); ++j)
//             {
//                 std::shared_ptr<MapTypes::Facet> current_face = faces_to_be_cut[j];
//                 std::vector<std::shared_ptr<MapTypes::Facet>> tmp = cut(current_face, cutter, mesh);
//                 new_faces.insert(tmp.begin(), tmp.end());
//                 if (tmp.empty())
//                 {
//                     remained_faces.insert(current_face);
//                 }
//             }
//             faces_to_be_cut = std::vector<std::shared_ptr<MapTypes::Facet>>(new_faces.begin(), new_faces.end());
//             faces_to_be_cut.insert(faces_to_be_cut.end(), remained_faces.begin(), remained_faces.end());
//             cutting_planes.erase(cutter);
//         }

//         progress.next();
//     }
// }

// std::set<std::shared_ptr<Plane>> MapGenerator::collect_cutting_planes(std::shared_ptr<MapTypes::Facet> face, std::shared_ptr<Map> mesh)
// {
//     std::set<std::shared_ptr<Plane>> cutting_planes;
//     for (auto it = mesh->facets_begin(); it != mesh->facets_end(); it++)
//     {
//         auto f = *it;
//         if (f != face)
//         {
//             std::shared_ptr<Plane> plane = *facet_attrib_supporting_plane_->get(f->id);
//             if (plane != *facet_attrib_supporting_plane_->get(f->id))
//             {
//                 if (do_intersect(f, *facet_attrib_supporting_plane_->get(f->id)))
//                     cutting_planes.insert(plane);
//             }
//         }
//     }
//     return cutting_planes;
// }

// // test if face f and plane intersect
// bool MapGenerator::do_intersect(std::shared_ptr<MapTypes::Facet> f, std::shared_ptr<Plane> plane)
// {
//     std::vector<Intersection> vts;
//     compute_intersections(f, plane, vts);

//     return (vts.size() > 1);
// }
