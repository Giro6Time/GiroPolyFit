#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/Shape_detection/Efficient_RANSAC.h>
#include <CGAL/property_map.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
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
void buzhidao()
{
    Point_vector points;
    const std::string input_file = CGAL::data_file_path("points_3/cube.pwn");
    std::ifstream input_stream(input_file.c_str());
    if (input_stream.fail())
    {
        std::cerr << "failed open file \'" << input_file << "\'" << std::endl;
        return;
    }
    input_stream.close();
    Efficient_ransac ransac;
    ransac.set_input(points);
    ransac.add_shape_factory<CPlane>();
    ransac.detect();

    Efficient_ransac::Plane_range planes = ransac.planes();
    std::size_t num_planes = planes.size();

    Point_to_shape_index_map shape_index_map(points, planes);
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        // Uses the get function from the property map that accesses the 3rd element of the tuple.
        int plane_index = get(shape_index_map, i);
        points[i].get<2>() = plane_index;
    }
}