#pragma once

#include "style.h"
#include "simple_attribute.h"
#include "common.h"
#include "basic_types.h"
#include "openglfunc.h"
#include "map.h"
#include <memory>
#include <QOpenGLShaderProgram>
#include <QMatrix>
#include <QOpenGLBuffer>
#include "style.h"
namespace Ui
{
    class Canvas;
};
class GIROPOLYFIT_API MeshRenderer
{

public:
    typedef Math::vec3 vec3;

public:
    MeshRenderer(Ui::Canvas *canvas);
    ~MeshRenderer();

    virtual void init();
    virtual void set_target(std::shared_ptr<Map> mesh);

    virtual void draw();
    virtual void draw_surface();

    // interface
    std::shared_ptr<Map> target() { return target_; }
    QMatrix4x4 model() { return model_; }
    const SurfaceStyle get_surface_style() { return surface_style_; }
    const EdgeStyle get_mesh_style() { return mesh_style_; }
    void set_surface_style(SurfaceStyle style) { surface_style_ = style; }
    void set_mesh_style(EdgeStyle style) { mesh_style_ = style; }

protected:
private:
    Ui::Canvas *canvas_;
    std::shared_ptr<Map> target_;
    SurfaceStyle surface_style_;
    EdgeStyle mesh_style_;
    EdgeStyle sharp_edge_style_;
    std::shared_ptr<AttributeHandle<Color>> facet_color_;
    std::shared_ptr<AttributeHandle<Math::vec3>> facet_normals_;
    std::shared_ptr<AttributeHandle<Math::vec3>> vertices_normals_;

    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<Color> colors;

    GLuint VAO = 0;
    GLuint VBO[3] = {0, 0, 0};

    QOpenGLShaderProgram shader_program;
    QMatrix4x4 model_;

    GLuint view_loc_ = -1;
    GLuint projection_loc_ = -1;
};