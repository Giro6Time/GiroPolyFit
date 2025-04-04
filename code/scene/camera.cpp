#include "camera.h"
#include "point_set_io.h"
#include "point_set.h"
#include "point_set_renderer.h"
#include <QtMath>
Camera::Camera(QWidget *parent)
    : cameraPos_(0.0f, 0.0f, 3.0f),
      cameraFront_(0.0f, 0.0f, -1.0f),
      cameraUp_(0.0f, 1.0f, 0.0f)
{
}

Camera::~Camera()
{
}
void Camera::handleKeyPress(QKeyEvent *event)
{
    pressedKeys_.insert(event->key());
}

void Camera::handleKeyRelease(QKeyEvent *event)
{
    pressedKeys_.remove(event->key());
}

void Camera::handleMousePress(QMouseEvent *event)
{
    lastMousePosition_ = event->pos();
}
void Camera::handleMouseMove(QMouseEvent *event)
{
    if (lastMousePosition_ == QPoint()) // 防止第一帧鼠标未移动时的错误计算
    {
        lastMousePosition_ = event->pos();
        return;
    }
    float sensitivity = 0.1f; // 鼠标灵敏度
    float xOffset = event->x() - lastMousePosition_.x();
    float yOffset = lastMousePosition_.y() - event->y(); // 注意 Y 坐标是反的
    lastMousePosition_ = event->pos();

    xOffset *= sensitivity;
    yOffset *= sensitivity;

    // 更新 yaw 和 pitch
    yaw_ += xOffset;
    pitch_ += yOffset;

    // 限制俯仰角范围（-89.0° 到 89.0°）
    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    else if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    // 计算新的前向向量
    float yawRad = qDegreesToRadians(yaw_);
    float pitchRad = qDegreesToRadians(pitch_);

    QVector3D front;
    front.setX(std::cos(yawRad) * std::cos(pitchRad));
    front.setY(std::sin(pitchRad));
    front.setZ(std::sin(yawRad) * std::cos(pitchRad));
    cameraFront_ = front.normalized();
}
void Camera::updateCameraPosition()
{
    float cameraSpeed = 0.05f;
    if (pressedKeys_.contains(Qt::Key_W))
        cameraPos_ += cameraSpeed * cameraFront_;
    if (pressedKeys_.contains(Qt::Key_S))
        cameraPos_ -= cameraSpeed * cameraFront_;
    if (pressedKeys_.contains(Qt::Key_A))
        cameraPos_ -= QVector3D::crossProduct(cameraFront_, cameraUp_).normalized() * cameraSpeed;
    if (pressedKeys_.contains(Qt::Key_D))
        cameraPos_ += QVector3D::crossProduct(cameraFront_, cameraUp_).normalized() * cameraSpeed;
}

QMatrix4x4 Camera::getViewMatrix() const
{
    QMatrix4x4 view;
    view.lookAt(cameraPos_, cameraPos_ + cameraFront_, cameraUp_);
    return view;
}