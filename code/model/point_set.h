#pragma once

#include <vector>
#include "basic_types.h"
#include "common.h"
#include "vertex_group.h"
#include "principal_axes.h"
class VertexGroup;
class GIROPOLYFIT_API PointSet
{
public:
    PointSet();
    ~PointSet();

    typedef Math::vec3 vec3;

public:
    std::vector<vec3> &points() { return points_; }
    std::vector<vec3> &colors() { return colors_; }
    std::vector<vec3> &normals() { return normals_; }
    std::vector<float> &planar_qualities() { return planar_qualities_; }
    const std::vector<vec3> &points() const { return points_; }
    const std::vector<vec3> &colors() const { return colors_; }
    const std::vector<vec3> &normals() const { return normals_; }
    const std::vector<float> &planar_qualities() const { return planar_qualities_; }
    std::vector<std::shared_ptr<VertexGroup>> &groups() { return groups_; };

    const bool has_colors() const { return !colors_.empty() && colors_.size() == points_.size(); }
    const bool has_normals() const { return !normals_.empty() && normals_.size() == points_.size(); }
    const bool has_planar_qualities() const { return !planar_qualities_.empty() && planar_qualities_.size() == points_.size(); }
    // void fit_plane(std::shared_ptr<VertexGroup> g);

    Box3d &bbox() { return bbox_; }
    const Box3d &bbox() const { return bbox_; }

private:
    std::vector<vec3> points_;
    std::vector<vec3> colors_;
    std::vector<vec3> normals_;
    std::vector<float> planar_qualities_;
    std::vector<std::shared_ptr<VertexGroup>> groups_;

    mutable bool bbox_is_valid_;
    mutable Box3d bbox_;
};

// void PointSet::fit_plane(std::shared_ptr<VertexGroup> g)
// {
//     PrincipalAxes3d pca;
//     pca.begin();
//     for (std::size_t j = 0; j < g->size(); ++j)
//     {
//         pca.add_point(points_[g->at(j)]);
//     }
//     pca.end();

//     Plane plane(pca.center(), pca.axis(2)); // eigen vectors have been normalized
//     g->set_plane(plane);
// }
