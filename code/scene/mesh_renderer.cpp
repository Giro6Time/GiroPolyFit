#include "mesh_renderer.h"
#include "logger.h"
#include "canvas.h"
#include "map.h"
#include "math_types.h"
#include "utils.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glu.h>
#else
#include <gl/GLU.h>
#endif
static GLUquadric *g_quadric = 0;

MeshRenderer::MeshRenderer(Ui::Canvas *canvas)
    : canvas_(canvas),
      surface_style_(),
      mesh_style_(),
      sharp_edge_style_()
{
    surface_style_.visible = true;
    surface_style_.color = Color(1.f, 1.f, 1.f, 1.f);

    mesh_style_.visible = true;
    mesh_style_.color = Color(0.8f, 0.35f, 0.0f, 1.0f);
    mesh_style_.width = 1;

    sharp_edge_style_.visible = true;
    sharp_edge_style_.color = Color(0.8f, 0.35f, 0.f, 1.f);
    sharp_edge_style_.width = 1;

    model_.setToIdentity();
}
MeshRenderer::~MeshRenderer()
{
    // 删除 VAO
    if (VAO)
    {
        OPENGL_EXTRA_FUNCTIONS->glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    // 删除 VBO
    if (VBO[0] != 0 || VBO[1] != 0 || VBO[2] != 0) // 检查 VBO 是否有效
    {
        OPENGL_FUNCTIONS->glDeleteBuffers(3, VBO);
        std::fill(std::begin(VBO), std::end(VBO), 0); // 将 VBO 数组重置为 0
    }
    // 清空 CPU 数据
    vertices.clear();
    normals.clear();
    colors.clear();
}

void MeshRenderer::set_target(std::shared_ptr<Map> mesh)
{
    target_ = mesh;
    // 删除 VAO
    if (VAO)
    {
        OPENGL_EXTRA_FUNCTIONS->glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    // 删除 VBO
    if (VBO[0] != 0 || VBO[1] != 0 || VBO[2] != 0) // 检查 VBO 是否有效
    {
        OPENGL_FUNCTIONS->glDeleteBuffers(3, VBO);
        std::fill(std::begin(VBO), std::end(VBO), 0); // 将 VBO 数组重置为 0
    }
    // 清空 CPU 数据
    vertices.clear();
    normals.clear();
    colors.clear();
}

void MeshRenderer::init()
{
    // 获取属性句柄
    facet_color_ = target_->facet_attribute_store()->create_handle<Color>("color");
    facet_normals_ = target_->facet_attribute_store()->create_handle<Math::vec3>("facet_normals");
    my_assert(facet_color_ != nullptr);
    my_assert(facet_normals_ != nullptr);
    // 确保法线生成
    if (!target_->facet_attribute_store()->has_attribute("facet_normals"))
    {
        target_->compute_facet_normals();
    }
    if (true)
    {

        // 准备数据
        for (auto it = target_->facets_begin(); it != target_->facets_end(); it++)
        {
            vec3 n = *facet_normals_->get((*it)->id);

            std::shared_ptr<Color> c = facet_color_->get((*it)->id);

            auto jt = (*it)->halfedge();
            do
            {
                vec3 &v = jt->vertex()->point();
                vertices.push_back(v);
                colors.push_back(*c); // push进去的是vertex color
                normals.push_back(n);
                jt = jt->next();
            } while (jt != (*it)->halfedge());
        }

        // 加载shader
        bool success = shader_program.addShaderFromSourceCode(QOpenGLShader::Vertex, FileUtils::load_text_source("./shaders/mesh_ver.glsl").c_str());
        if (!success)
        {
            Logger::error("MeshRenderer") << "Failed to load vertex shader!" << std::endl
                                          << shader_program.log().toStdString() << std::endl;
        }
        success = shader_program.addShaderFromSourceCode(QOpenGLShader::Fragment, FileUtils::load_text_source("./shaders/mesh_frag.glsl").c_str());
        if (!success)
        {
            Logger::error("MeshRenderer") << "Failed to load frag shader!" << std::endl
                                          << shader_program.log().toStdString() << std::endl;
        }
        success = shader_program.link();
        if (!success)
        {
            Logger::error("MeshRenderer") << "Failed to link shaders!" << std::endl
                                          << shader_program.log().toStdString() << std::endl;
        }
        // 生成 VAO 和 VBO
        OPENGL_EXTRA_FUNCTIONS->glGenVertexArrays(1, &VAO);
        OPENGL_FUNCTIONS->glGenBuffers(3, VBO);

        // 绑定 VAO
        OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);

        // 绑定并填充顶点数据到 VBO[0]
        OPENGL_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        OPENGL_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), vertices.data(), GL_STATIC_DRAW);
        OPENGL_FUNCTIONS->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
        OPENGL_FUNCTIONS->glEnableVertexAttribArray(0);

        // 绑定并填充法线数据到 VBO[1]
        OPENGL_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        OPENGL_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), normals.data(), GL_STATIC_DRAW);
        OPENGL_FUNCTIONS->glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(vec3), (void *)0);
        OPENGL_FUNCTIONS->glEnableVertexAttribArray(1);

        // 绑定并填充颜色数据到 VBO[2]
        OPENGL_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
        OPENGL_FUNCTIONS->glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(Color), colors.data(), GL_STATIC_DRAW);
        OPENGL_FUNCTIONS->glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Color), (void *)0);
        OPENGL_FUNCTIONS->glEnableVertexAttribArray(2);

        OPENGL_FUNCTIONS->glBindBuffer(GL_ARRAY_BUFFER, 0);
        OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);
    }
}

void MeshRenderer::draw()
{
    if (!target_)
        return;
    if (!VAO)
        init();
    if (surface_style_.visible)
    {
        if (mesh_style_.visible)
        {
            glDisable(GL_LIGHTING);
            glEnable(GL_POLYGON_OFFSET_FILL);
            // z-fighting 避免
            glPolygonOffset(0.5f, -0.0001f);
        }

        draw_surface();

        if (mesh_style_.visible)
        {
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
    }
}
int cnt = 0;
void MeshRenderer::draw_surface()
{
    if (true)
    {
        // Logger::warn("OpenGL Error") << cnt++ << std::endl;
        shader_program.bind();

        shader_program.setUniformValue("model", model_);
        shader_program.setUniformValue("projection", canvas_->projection());
        shader_program.setUniformValue("view", canvas_->view());
        shader_program.setUniformValue("lightPos", canvas_->light_position()[0], canvas_->light_position()[1], canvas_->light_position()[2]);
        shader_program.setUniformValue("lightColor", canvas_->light_color()[0], canvas_->light_color()[1], canvas_->light_color()[2]);
        shader_program.setUniformValue("viewPos", canvas_->camera_position());

        // 绑定 VAO 并绘制
        OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(VAO);
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            Logger::warn("OpenGL Error") << VAO << "Error code: " << err << std::endl;
        }
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        OPENGL_EXTRA_FUNCTIONS->glUseProgram(0);
        OPENGL_EXTRA_FUNCTIONS->glBindVertexArray(0);

        shader_program.release();
    }

    // glEnable(GL_MULTISAMPLE);

    // glEnable(GL_LIGHTING);
    // glColor4fv(surface_style_.color.data());

    // glShadeModel(GL_FLAT);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // for (auto it = target_->facets_begin(); it != target_->facets_end(); it++)
    // {
    //     const vec3 &n = *facet_normals_->get((*it)->id);

    //     glNormal3fv(n.data());
    //     glColor4fv(facet_color_->get((*it)->id)->data());

    //     glBegin(GL_POLYGON);
    //     auto jt = (*it)->halfedge();
    //     do
    //     {
    //         const vec3 &p = jt->vertex()->point();
    //         glVertex3fv(p.data());
    //         jt = jt->next();
    //     } while (jt != (*it)->halfedge());
    //     glEnd();
    // }
}
