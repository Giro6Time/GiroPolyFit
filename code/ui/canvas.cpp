
#include "canvas.h"

namespace Ui
{
    Canvas::Canvas(QWidget *parent)
        : QOpenGLWidget(parent),
          camera_(std::make_unique<Camera>(this)),
          pset_renderer_(std::make_unique<PointSetRenderer>()),
          mesh_renderer_(std::make_shared<MeshRenderer>(this)),
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

        light_position_ = {1.f, 5.f, 1.f, 1.f};
        light_color_ = {1.f, 1.f, 1.f, 1.f};
        glEnable(GL_LIGHTING);
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); /*GL_FALSE*/
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position_.data());
        glDisable(GL_LIGHTING);
        backgroundColor = QColor(0.2, 0.3, 0.3, 1.0);
    }

    void Canvas::resizeGL(int w, int h)
    {
        projection_.setToIdentity();
        projection_.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
    }

    void Canvas::paintGL()
    {
        glClearColor(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), backgroundColor.alphaF());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view_ = camera_->getViewMatrix();
        // 设置投影和视图矩阵
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(projection_.constData());
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(view_.constData());

        // 应用 model 矩阵
        QMatrix4x4 model;
        glPushMatrix();
        glMultMatrixf(model.data());

        // PointSetRenderer 和MeshRenderer 的数据结构不相同，写法有一定差异
        pset_renderer_->draw(point_set_);
        // 恢复之前的模型视图矩阵
        glPopMatrix();
        mesh_renderer_->draw();
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

    void Canvas::onTransformChanged(QMatrix4x4 transform)
    {
        model = transform;
        mesh_renderer_->model() = transform;
    }
    void Canvas::onPointCloudSettingChanged(float pointSize, const QColor &pointColor)
    {
        PointStyle style = pset_renderer_->get_point_style();
        style.color = Color(pointColor.redF(), pointColor.greenF(), pointColor.blueF(), pointColor.alphaF());
        style.size = pointSize;
        pset_renderer_->set_point_style(style);
    }
    void Canvas::onMeshSettingsChanged(bool showGrid, bool showSurface, const QColor &gridColor, const QColor &surfaceColor)
    {
        SurfaceStyle surfaceStyle = mesh_renderer_->get_surface_style();
        EdgeStyle meshStyle = mesh_renderer_->get_mesh_style();
        surfaceStyle.color = Color(surfaceColor.redF(), surfaceColor.greenF(), surfaceColor.blueF(), surfaceColor.alphaF());
        meshStyle.color = Color(gridColor.redF(), gridColor.greenF(), gridColor.blueF(), gridColor.alphaF());
    }
    void Canvas::onEnvironmentSettingsChanged(const QVector3D &lightPosition, const QColor &lightColor, const QColor &backgroundColor, float cameraSpeed)
    {
        light_position_[0] = lightPosition.x();
        light_position_[1] = lightPosition.y();
        light_position_[2] = lightPosition.z();

        light_color_[0] = lightColor.redF();
        light_color_[1] = lightColor.greenF();
        light_color_[2] = lightColor.blueF();
        light_color_[3] = lightColor.alphaF();
    }

}