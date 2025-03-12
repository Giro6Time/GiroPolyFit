#pragma once
#include <QApplication>
#include <QWidget>
#include <iostream>
#include "canvas.h"
#include "logger.h"
#include "point_set.h"
#include "point_set_io.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv); // 创建一个QApplication对象

    Logger::initialize();

    auto pset = PointSetIO::read("D:/Study/Polyfit/GiroPolyFit/test/pointset.vg");

    Ui::Canvas canvas;
    canvas.add_point_set(pset);
    canvas.show();

    return app.exec(); // 进入事件循环
}
