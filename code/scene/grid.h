#pragma once
#include <openglfunc.h>
#include <QMatrix4x4>
#include "common.h"
#include "basic_types.h"
class GIROPOLYFIT_API Grid
{
public:
    Grid(int size = 10, float spacing = 1.0f);
    void draw(const QMatrix4x4 &projection, const QVector3D &cameraPos);

private:
    int size_;
    int max_size_;
    float spacing_;
};
