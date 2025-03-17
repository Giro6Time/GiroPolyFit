#include "grid.h"

Grid::Grid(int size, float spacing)
    : size_(size), spacing_(spacing), max_size_(size * 1.5f)
{
}

void Grid::draw(const QMatrix4x4 &projection, const QVector3D &cameraPos)
{
    glDisable(GL_LIGHTING);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    int halfGridSize = size_ * spacing_ / 2.0f;
    // 根据相机的高度动态调整网格的密度
    halfGridSize = Numeric::min((int)(halfGridSize * (cameraPos.y() / 10.0f + 0.5f)), max_size_);

    glLineWidth(0.5f);
    glColor3f(0.5f, 0.5f, 0.5f); // 灰色
    // 计算网格的中心位置，四舍五入到最近的网格点
    float centerX = std::round(cameraPos.x() / spacing_) * spacing_;
    float centerZ = std::round(cameraPos.z() / spacing_) * spacing_;

    glBegin(GL_LINES);
    for (int i = -halfGridSize; i <= halfGridSize; ++i)
    {
        glVertex3f(i + centerX, 0.0f, -halfGridSize + centerZ);
        glVertex3f(i + centerX, 0.0f, halfGridSize + centerZ);
        glVertex3f(-halfGridSize + centerX, 0.0f, i + centerZ);
        glVertex3f(halfGridSize + centerX, 0.0f, i + centerZ);
    }
    glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glDisable(GL_LINE_SMOOTH);
}