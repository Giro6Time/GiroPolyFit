#pragma once
#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QPushButton>
#include <QWidget>
#include "canvas.h" // 假设 Canvas 类定义在 canvas.h 中

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    void setupMenuBar();
    void setupToolBar();
    void setupCentralWidget();
    void connectSignalToSlot();
public slots:
    void CreateMapFromPointset();
private:
    QSplitter *leftRightSplitter; // 左右分割器
    QSplitter *topBottomSplitter; // 上下分割器
    QWidget *centerWidget;        // 中心部件
    QVBoxLayout *centerLayout;    // 中心布局
    QHBoxLayout *topLayout;       // 顶部布局
public:
    Ui::Canvas *canvas;  // OpenGL 画布
    QWidget *leftPanel;  // 左侧编辑栏
    QWidget *rightPanel; // 右侧编辑栏
    QWidget *topPanel;   // 顶部选项栏
};
