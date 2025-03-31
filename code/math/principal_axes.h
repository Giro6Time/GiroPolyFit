#include "basic_types.h"

class PrincipalAxes3d
{
public:
    typedef Math::vec3 vec3;

public:
    PrincipalAxes3d();
    void begin();
    void add_point(const vec3 &p, double weight = 1.0);
    void end();

    vec3 center() const;
    const vec3 &axis(int i) const;
    double eigen_value(int i) const;

private:
    double center_[3];
    vec3 axis_[3];
    double eigen_value_[3];

    double M_[6];
    int nb_points_;
    double sum_weights_;
};
