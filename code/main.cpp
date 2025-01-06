#include <QApplication>
#include <iostream>

#include "test.cpp"
#include "logger.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // 创建一个QApplication对象

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QWidget *window = new QWidget(); // 创建一个QWidget窗口
    MyUi::Form ui(window);
    return app.exec(); // 进入事件循环
}