#include <QApplication>
#include <QWidget>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // 创建一个QApplication对象

    QWidget window;                           // 创建一个QWidget窗口
    window.setWindowTitle("Qt5 Test Window"); // 设置窗口标题
    window.resize(400, 300);                  // 设置窗口大小

    QLabel *label = new QLabel(&window); // 创建一个QLabel标签
    label->setText("Hello, Qt5!");       // 设置标签文本
    label->move(100, 100);               // 移动标签到窗口中的位置

    window.show();     // 显示窗口
    return app.exec(); // 进入事件循环
}