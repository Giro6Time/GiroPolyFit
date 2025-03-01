#include <QApplication>
#include <iostream>

#include "test.cpp"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // 创建一个QApplication对象

#if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    my_assert(true);

#endif

    QWidget *window = new QWidget(); // 创建一个QWidget窗口
    std::shared_ptr<MyUi::Form> ui(new MyUi::Form(window));
    ui->setupUi(window);
    return app.exec(); // 进入事件循环
}
