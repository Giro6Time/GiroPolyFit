#pragma once
#include <QApplication>
#include <QOpenGLWidget>
#include <iostream>
#include <QOpenGLFunctions_4_5_Core>
#include <QMatrix4x4>
#include "grid.h"
#include "camera.h"
#include "point_set_renderer.h"

class Grid;
class Camera;
class PointSetRenderer;

namespace Ui
{
    class Canvas : public QOpenGLWidget, public QOpenGLFunctions_4_5_Core
    {
        Q_OBJECT
    public:
        explicit Canvas(QWidget *parent = nullptr);
        virtual ~Canvas() {}

    private slots:
        ///@note: 如果有必要，可以拓展这个类为观察者模式，这样就可以在timeout时有更通用的方法触发别的函数
        void onTimerTimeout();

    protected:
        void initializeGL() override;
        void resizeGL(int w, int h) override;
        void paintGL() override;

        void keyPressEvent(QKeyEvent *event) override;
        void keyReleaseEvent(QKeyEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

    public:
        /// @brief 添加pointset到渲染列表中
        /// @return pointset在列表中的索引
        size_t add_point_set(std::shared_ptr<PointSet> pset)
        {
            point_sets_.push_back(pset);
            return point_sets_.size() - 1;
        }
        /// @brief 获取pointset
        /// @param index pointset在列表中的索引
        std::shared_ptr<PointSet> get_point_set(int index)
        {
            return point_sets_[index];
        }

    private:
        std::unique_ptr<Camera> camera_;
        std::unique_ptr<PointSetRenderer> renderer_;
        QMatrix4x4 projection_;
        std::unique_ptr<QTimer> timer_;
        std::unique_ptr<Grid> grid_;

        std::vector<std::shared_ptr<PointSet>> point_sets_;
    };
}