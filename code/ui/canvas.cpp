
#include "canvas.h"

namespace Ui
{
    Canvas::Canvas(QWidget *parent)
        : QOpenGLWidget(parent),
          camera_(std::make_unique<Camera>(this)),
          pset_renderer_(std::make_unique<PointSetRenderer>()),
          grid_(std::make_unique<Grid>(100))

    {
        timer_ = std::make_unique<QTimer>(this);
        connect(timer_.get(), SIGNAL(timeout()), this, SLOT(onTimerTimeout()));
        timer_->start(16); // 60fps
    }

    void Canvas::onTimerTimeout()
    {
        camera_->updateCameraPosition();
        update();
    }

    void Canvas::initializeGL()
    {
        initializeOpenGLFunctions();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_MULTISAMPLE);
// glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
#ifndef NDEBUG
        glEnable(GL_DEBUG_OUTPUT);
#endif

        light_position_ = {10.f, 10.f, 10.f, 1.f};
        light_color_ = {1.f, 1.f, 1.f, 1.f};

        // 初始化所有 MeshRenderer
        for (auto &mesh_renderer : mesh_renderers_)
        {
            mesh_renderer->init();
        }
    }

    void Canvas::resizeGL(int w, int h)
    {
        projection_.setToIdentity();
        projection_.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
    }

    void Canvas::paintGL()
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view_ = camera_->getViewMatrix();
        light_position_[0] -= 0.05f;
        // 设置投影和视图矩阵
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projection_.constData());
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(view_.constData());

        // PointSetRenderer 和MeshRenderer 的数据结构不相同，写法有一定差异
        for (auto it = point_sets_.begin(); it != point_sets_.end(); it++)
        {
            pset_renderer_->draw(*it);
        }
        for (auto it = mesh_renderers_.begin(); it != mesh_renderers_.end(); it++)
        {
            (*it)->draw();
        }
        grid_->draw(projection_, camera_->getCameraPos());
    }

    void Canvas::keyPressEvent(QKeyEvent *event)
    {
        camera_->handleKeyPress(event);
    }

    void Canvas::keyReleaseEvent(QKeyEvent *event)
    {
        camera_->handleKeyRelease(event);
    }

    void Canvas::mousePressEvent(QMouseEvent *event)
    {
        camera_->handleMousePress(event);
    }

    void Canvas::mouseMoveEvent(QMouseEvent *event)
    {
        camera_->handleMouseMove(event);
    }

}