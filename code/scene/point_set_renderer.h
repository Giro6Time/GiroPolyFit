#pragma once
#include "point_set.h"
#include "openglfunc.h"
#include "style.h"
#include <memory>

class GIROPOLYFIT_API PointSetRenderer
{
public:
    void set_point_style(const PointStyle &style) { point_set_style_ = style; }

public:
    virtual void draw(std::shared_ptr<PointSet> pset);

protected:
    void draw_point_set_per_point_color(PointSet *pset);

    PointStyle point_set_style_;
};