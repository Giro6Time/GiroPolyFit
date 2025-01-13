#include <vector>
#include "math_types.h"
#include "common.h"

class GIROPOLYFIT_API PointSet
{
public:
    PointSet();
    ~PointSet();

public:
    std::vector<vec3> &points() { return points_; }
    std::vector<vec3> &colors() { return colors_; }
    std::vector<vec3> &normals() { return normals_; }
    std::vector<float> &planar_qualities() { return planar_qualities_; }
    const std::vector<vec3> &points() const { return points_; }
    const std::vector<vec3> &colors() const { return colors_; }
    const std::vector<vec3> &normals() const { return normals_; }
    const std::vector<float> &planar_qualities() const
    {
        return planar_qualities_;
    }

private:
    std::vector<vec3> points_;
    std::vector<vec3> colors_;
    std::vector<vec3> normals_;
    std::vector<float> planar_qualities_;

    mutable bool bbox_is_valid_;
    // mutable Box3d bbox_;
};