#pragma once
#include <QApplication>
#include <QOpenGLWidget>
#include <iostream>
#include <QOpenGLFunctions_4_5_Core>
#include <QMatrix4x4>
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
        /// @brief 移除指定的点集对象
        /// @param pset 用户传入的 PointSet 对象
        void remove_point_set_by_object(std::shared_ptr<PointSet> pset)
        {
            auto it = std::find(point_sets_.begin(), point_sets_.end(), pset);
            if (it != point_sets_.end())
            {
                point_sets_.erase(it); // 移除找到的点集
            }
            else
            {
                Logger::warn("Canvas") << "PointSet not found in point_sets_" << std::endl;
            }
        }

        /// @brief 添加网格体到渲染列表中
        /// @param map 用户传入的 Map 对象
        /// @return MeshRenderer 在列表中的索引
        size_t add_mesh(std::shared_ptr<Map> map)
        {
            auto renderer = std::make_shared<MeshRenderer>(map, shared_from_this()); // 创建 MeshRenderer，初始化将被延迟到initializeGL中执行
            mesh_renderers_.push_back(renderer);                                     // 添加到渲染列表
            return mesh_renderers_.size() - 1;                                       // 返回索引
        }

        /// @brief 移除指定索引的网格体
        /// @param index MeshRenderer 在列表中的索引
        void remove_mesh(size_t index)
        {
            if (index < mesh_renderers_.size())
            {
                mesh_renderers_.erase(mesh_renderers_.begin() + index); // 移除指定索引的 MeshRenderer
            }
            else
            {
                Logger::warn("Canvas") << "Invalid index: " << index << std::endl;
            }
        }

        /// @brief 移除与指定 Map 对象关联的网格体
        /// @param map 用户传入的 Map 对象
        void remove_mesh_by_map(std::shared_ptr<Map> map)
        {
            auto it = std::find_if(mesh_renderers_.begin(), mesh_renderers_.end(),
                                   [&map](const std::shared_ptr<MeshRenderer> &renderer)
                                   {
                                       return renderer->target() == map; // 假设 MeshRenderer 提供 get_map() 方法
                                   });
            if (it != mesh_renderers_.end())
            {
                mesh_renderers_.erase(it); // 移除找到的 MeshRenderer
            }
            else
            {
                Logger::warn("Canvas") << "try to remove mesh but failed to find." << std::endl;
            }
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

        std::vector<std::shared_ptr<PointSet>> point_sets_;

        std::vector<std::shared_ptr<MeshRenderer>> mesh_renderers_;
        bool initialized = false;
    };
}