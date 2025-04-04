#pragma once
#include <QApplication>
#include <QWidget>
#include <iostream>
#include "canvas.h"
#include "logger.h"
#include "point_set.h"
#include "point_set_io.h"
#include "mainwindow.h"

#include "map_io.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv); // 创建一个QApplication对象

    Logger::initialize();

    MainWindow w;
    w.show();
    return app.exec(); // 进入事件循环
}
