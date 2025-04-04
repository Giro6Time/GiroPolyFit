#pragma once
#include <QApplication>
#include <QOpenGLWidget>
#include <iostream>
#include <QOpenGLFunctions_4_5_Core>
#include <QMatrix4x4>
#include <QColor>
#include "grid.h"
#include "camera.h"
#include "point_set_renderer.h"
#include "mesh_renderer.h"
#include "logger.h"
class Grid;
class Camera;
class PointSetRenderer;

namespace Ui
{
    class Canvas : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core, public std::enable_shared_from_this<Canvas>
    {
        Q_OBJECT
    public:
        explicit Canvas(QWidget *parent = nullptr);
        virtual ~Canvas() {}

    private slots:
        ///@note: 如果有必要，可以拓展这个类为观察者模式，这样就可以在timeout时有更通用的方法触发别的函数
        void onTimerTimeout();

    public:
        void onTransformChanged(QMatrix4x4 transform);
        void onPointCloudSettingChanged(float pointSize, const QColor &pointColor);
        void onMeshSettingsChanged(bool showGrid, bool showSurface, const QColor &gridColor, const QColor &surfaceColor);
        void onEnvironmentSettingsChanged(const QVector3D &lightPosition, const QColor &lightColor, const QColor &backgroundColor, float cameraSpeed);

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;

        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

    public:
        // interface

        /// @brief 添加pointset到渲染列表中
        /// @return pointset在列表中的索引
        void set_point_set(std::shared_ptr<PointSet> pset)
        {
            point_set_ = pset;
        }
        /// @brief 获取pointset
        /// @param index pointset在列表中的索引
        std::shared_ptr<PointSet> get_point_set(int index)
        {
            return point_set_;
        }

        /// @brief 添加网格体到渲染列表中
        /// @param map 用户传入的 Map 对象
        /// @return MeshRenderer 在列表中的索引
        void set_mesh(std::shared_ptr<Map> map)
        {
            mesh_renderer_->set_target(map);
        }

        QMatrix4x4 projection()
        {
            return projection_;
        }
        QMatrix4x4 view()
        {
            return view_;
        }
        std::vector<GLfloat> light_position() { return light_position_; }
        std::vector<GLfloat> light_color() { return light_color_; }
        QVector3D camera_position() { return camera_->getCameraPos(); }

    private:
        std::unique_ptr<Camera> camera_;
        std::unique_ptr<PointSetRenderer> pset_renderer_;
        QMatrix4x4 projection_;
        QMatrix4x4 view_;
        std::vector<GLfloat> light_position_; // 光源位置
        std::vector<GLfloat> light_color_;    // 光源颜色

        std::unique_ptr<QTimer> timer_;
        std::unique_ptr<Grid> grid_;

        std::shared_ptr<PointSet> point_set_;

        std::shared_ptr<MeshRenderer> mesh_renderer_;
        QColor backgroundColor;
        bool initialized = false;
        QMatrix4x4 model;
    };
}