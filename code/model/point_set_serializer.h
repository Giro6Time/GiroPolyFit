#pragma once
#include "common.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "logger.h"
#include "math_types.h"
#include "point_set.h"

class PointSet;
class VertexGroup;

class GIROPOLYFIT_API PointSetSerializer
{
public:
    static void load_vg(PointSet *point_set, const std::string &file_name);
    static void save_vg(const PointSet *point_set, const std::string &file_name);

    static void load_bvg(PointSet *point_set, const std::string &file_name);
    static void save_bvg(const PointSet *point_set, const std::string &file_name);

private:
    static VertexGroup *read_ascii_group(std::istream &input);
    static void write_ascii_group(std::ostream &output, VertexGroup *group);

    static VertexGroup *read_binary_group(std::istream &input);
    static void write_binary_group(std::ostream &output, VertexGroup *group);
};
