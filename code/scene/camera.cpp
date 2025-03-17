#include "camera.h"
#include "point_set_io.h"
#include "point_set.h"
#include "point_set_renderer.h"
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
    float sensitivity = 0.1f;
    float xOffset = event->x() - lastMousePosition_.x();
    float yOffset = lastMousePosition_.y() - event->y();
    lastMousePosition_ = event->pos();

    xOffset *= sensitivity;
    yOffset *= sensitivity;

    QMatrix4x4 rotation;
    QVector3D right = QVector3D::crossProduct(cameraFront_, cameraUp_).normalized();
    rotation.rotate(xOffset, cameraUp_);
    rotation.rotate(yOffset, right);

    cameraFront_ = rotation * cameraFront_;
    cameraFront_.normalize();
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