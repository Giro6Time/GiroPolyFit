#pragma once
#include "common.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

#include "logger.h"
#include "math_types.h"
#include "point_set.h"
#include "vertex_group.h"
#include "progress.h"

class PointSet;
class VertexGroup;

class GIROPOLYFIT_API PointSetSerializer
{
public:
    static void load_vg(std::shared_ptr<PointSet> point_set, const std::string &file_name);
    static void save_vg(const std::shared_ptr<PointSet> point_set, const std::string &file_name);

    static void load_bvg(std::shared_ptr<PointSet> point_set, const std::string &file_name);
    static void save_bvg(const std::shared_ptr<PointSet> point_set, const std::string &file_name);

private:
    static std::shared_ptr<VertexGroup> read_ascii_group(std::istream &input);
    static void write_ascii_group(std::ostream &output, std::shared_ptr<VertexGroup> group);

    static std::shared_ptr<VertexGroup> read_binary_group(std::istream &input);
    static void write_binary_group(std::ostream &output, std::shared_ptr<VertexGroup> group);

    static std::vector<float> get_group_parameters(std::shared_ptr<VertexGroup> g);
};
