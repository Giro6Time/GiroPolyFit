#include "point_set_renderer.h"

void PointSetRenderer::draw(std::shared_ptr<PointSet> pset)
{
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glDisable(GL_MULTISAMPLE);

    draw_point_set_per_point_color(pset.get());
    // glBindVertexArray(pset->vao);
    // glDrawArrays(GL_POINTS, 0, pset->num_points);

    glEnable(GL_MULTISAMPLE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

void PointSetRenderer::draw_point_set_per_point_color(PointSet *pset)
{
    if (!pset)
        return;

    if (!pset->has_colors())
        return;

    int num = pset->points().size();
    if (num < 1)
        return;

    float *points = &(pset->points()[0].x);
    float *colors = &(pset->colors()[0].x);

    glPointSize(point_set_style_.size);

    // TODO: 等加了光照之后把！去掉
    if (false/*pset->has_normals()*/)
    {
        float *normals = &(pset->normals()[0].x);
        glEnable(GL_LIGHTING);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, points);
        glNormalPointer(GL_FLOAT, 0, normals);
        glColorPointer(3, GL_FLOAT, 0, colors);
        glDrawArrays(GL_POINTS, 0, num);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    else
    {
        glDisable(GL_LIGHTING); // always off for points without normals

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, points);
        glColorPointer(3, GL_FLOAT, 0, colors);
        glDrawArrays(GL_POINTS, 0, num);
        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_LIGHTING);
    }
}
