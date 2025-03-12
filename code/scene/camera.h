#pragma once

#include <QMatrix4x4>
#include <QVector3D>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QObject>
#include <QSet>
#include <QTimer>
#include <memory>
#include "common.h"
class GIROPOLYFIT_API Camera : public QObject
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    ~Camera();

public:
    void handleKeyPress(QKeyEvent *event);
    void handleKeyRelease(QKeyEvent *event);
    void handleMousePress(QMouseEvent *event);
    void handleMouseMove(QMouseEvent *event);

    QMatrix4x4 getViewMatrix() const;
    QVector3D getCameraPos() const { return cameraPos_; }

public slots:
    void updateCameraPosition();

private:
    QMatrix4x4 projection_;
    QMatrix4x4 view_;
    QVector3D cameraPos_;
    QVector3D cameraFront_;
    QVector3D cameraUp_;

    QPoint lastMousePosition_;
    QSet<int> pressedKeys_;
};
