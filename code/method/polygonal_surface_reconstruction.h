#pragma once
#include "ransac.h"
#include "CGAL/Polygonal_surface_reconstruction.h"
#include "map.h"
#include <CGAL/GLPK_mixed_integer_program_traits.h>
#include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
typedef CGAL::GLPK_mixed_integer_program_traits<double> MIP_Solver;

typedef CGAL::Polygonal_surface_reconstruction<Kernel> Polygonal_surface_reconstruction;
typedef CGAL::Surface_mesh<Point> Surface_mesh;

namespace PMP = CGAL::Polygon_mesh_processing;


Surface_mesh convert_to_mesh(Point_vector& points);
std::shared_ptr<Map> convert_to_map(Surface_mesh& model);