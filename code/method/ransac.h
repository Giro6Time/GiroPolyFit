#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/Shape_detection/Efficient_RANSAC.h>
#include <CGAL/property_map.h>

#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/Shape_detection/Region_growing/Point_set.h>

#include <vector>
#include <string>
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::FT FT;
typedef Kernel::Point_3 Point;
typedef Kernel::Vector_3 Vector;

typedef boost::tuple<Point, Vector, int> PNI;
typedef std::vector<PNI> Point_vector;
typedef CGAL::Nth_of_tuple_property_map<0, PNI> Point_map;
typedef CGAL::Nth_of_tuple_property_map<1, PNI> Normal_map;
typedef CGAL::Nth_of_tuple_property_map<2, PNI> Plane_index_map;

typedef CGAL::Shape_detection::Efficient_RANSAC_traits<Kernel, Point_vector, Point_map, Normal_map> Traits;
typedef CGAL::Shape_detection::Efficient_RANSAC<Traits> Efficient_ransac;
typedef CGAL::Shape_detection::Plane<Traits> CPlane;
typedef CGAL::Shape_detection::Point_to_shape_index_map<Traits> Point_to_shape_index_map;

using Point_map_region_growing = CGAL::Compose_property_map<CGAL::Random_access_property_map<Point_vector>, Point_map>;
using Normal_map_region_growing = CGAL::Compose_property_map<CGAL::Random_access_property_map<Point_vector>, Normal_map>;

using Region_type = CGAL::Shape_detection::Point_set::Least_squares_plane_fit_region<Kernel, std::size_t, Point_map_region_growing, Normal_map_region_growing>;
using Neighbor_query = CGAL::Shape_detection::Point_set::Sphere_neighbor_query<Kernel, std::size_t, Point_map_region_growing>;
using Region_growing = CGAL::Shape_detection::Region_growing<Neighbor_query, Region_type>;

Point_vector convert_pointcloud_to_point_vector(const std::string &file_name);
Point_vector convert_to_point_vector_by_region_growing(const std::string &file_name);