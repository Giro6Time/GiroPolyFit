#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/Shape_detection/Efficient_RANSAC.h>
#include <CGAL/property_map.h>
#include <vector>
#include <string>
#include "logger.h"
#include "progress.h"
#include <ransac.h>
#include <boost/range/irange.hpp>
#include <CGAL/Timer.h>

Point_vector convert_pointcloud_to_point_vector(const std::string &file_name)
{
    Logger::out("PointConvert") << file_name << std::endl;
    Point_vector points;
    const std::string input_file = CGAL::data_file_path(file_name);
    std::ifstream input_stream(input_file.c_str());
    if (input_stream.fail())
    {
        Logger::error("PointConvert") << "failed open file \'" << input_file << "\'" << std::endl;
        return points;
    }
    input_stream.close();
    if (!CGAL::IO::read_points(input_file.c_str(), std::back_inserter(points),
                               CGAL::parameters::point_map(Point_map()).normal_map(Normal_map())))
    {
        Logger::error("PointConvert") << "Error: cannot read file " << input_file << std::endl;
        return points;
    }
    else
        Logger::out("PointConvert") << " Done. " << points.size() << " points. " << std::endl;

    Efficient_ransac ransac;

    ransac.set_input(points);
    ransac.add_shape_factory<CPlane>();
    ransac.detect();

    Efficient_ransac::Plane_range planes = ransac.planes();
    std::size_t num_planes = planes.size();
    Logger::out("PointConvert") << " Done. " << num_planes << " planes. " << std::endl;

    Point_to_shape_index_map shape_index_map(points, planes);
    ProgressLogger progress(num_planes);
    for (std::size_t i = 0; i < points.size(); ++i)
    {
        progress.next();
        // Uses the get function from the property map that accesses the 3rd element of the tuple.
        int plane_index = get(shape_index_map, i);
        points[i].get<2>() = plane_index;
    }
    return points;
}

Point_vector convert_to_point_vector_by_region_growing(const std::string &file_name)
{
    Point_vector points;

    // Load point set from a file.
    const std::string input_file = CGAL::data_file_path(file_name);
    std::ifstream input_stream(input_file.c_str());
    if (input_stream.fail())
    {
        Logger::error("PointConvert") << "Failed open file \'" << input_file << "\'" << std::endl;
        return points;
    }
    input_stream.close();
    Logger::out("PointConvert") << "Loading point cloud: " << input_file << "...";

    CGAL::Timer t;
    t.start();
    if (!CGAL::IO::read_points(input_file.c_str(), std::back_inserter(points),
                               CGAL::parameters::point_map(Point_map()).normal_map(Normal_map())))
    {

        Logger::out("PointConvert") << "Error: cannot read file " << input_file << std::endl;
        return points;
    }
    else
        Logger::error("PointConvert") << " Done. " << points.size() << " points. Time: "
                                      << t.time() << " sec." << std::endl;

    // Shape detection.

    // Default parameter values for the data file cube.pwn.
    const FT search_sphere_radius = FT(20);
    const FT max_distance_to_plane = FT(20);
    const FT max_accepted_angle = FT(25);
    const std::size_t min_region_size = 200;

    Point_map_region_growing point_map_rg(CGAL::make_random_access_property_map(points));
    Normal_map_region_growing normal_map_rg(CGAL::make_random_access_property_map(points));

    // Create instances of the classes Neighbor_query and Region_type.
    Neighbor_query neighbor_query(
        boost::irange<std::size_t>(0, points.size()), CGAL::parameters::sphere_radius(search_sphere_radius).point_map(point_map_rg));

    Region_type region_type(
        CGAL::parameters::
            maximum_distance(max_distance_to_plane)
                .maximum_angle(max_accepted_angle)
                .minimum_region_size(min_region_size)
                .point_map(point_map_rg)
                .normal_map(normal_map_rg));

    // Create an instance of the region growing class.
    Region_growing region_growing(
        boost::irange<std::size_t>(0, points.size()), neighbor_query, region_type);

    Logger::out("PointConvert") << "Extracting planes...";
    std::vector<typename Region_growing::Primitive_and_region> regions;
    t.reset();
    region_growing.detect(std::back_inserter(regions));
    Logger::out("PointConvert") << " Done. " << regions.size() << " planes extracted. Time: "
                                << t.time() << " sec." << std::endl;

    // Stores the plane index of each point as the third element of the tuple.
    for (std::size_t i = 0; i < points.size(); ++i)
        // Uses the get function from the property map that accesses the 3rd element of the tuple.
        points[i].get<2>() = static_cast<int>(get(region_growing.region_map(), i));
    return points;
}