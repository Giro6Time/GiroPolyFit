#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupMenuBar();
    setupToolBar();
    setupCentralWidget();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();

    // 文件菜单
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *openAction = fileMenu->addAction("Open");
    QAction *saveAction = fileMenu->addAction("Save");
    QAction *exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    // 视图菜单
    QMenu *viewMenu = menuBar->addMenu("View");
    QAction *resetViewAction = viewMenu->addAction("Reset View");

    // 其他菜单可以根据需要添加
}

void MainWindow::setupToolBar()
{
    QToolBar *toolBar = addToolBar("Main ToolBar");

    QPushButton *resetViewButton = new QPushButton("Reset View");
    toolBar->addWidget(resetViewButton);
    connect(resetViewButton, &QPushButton::clicked, this, []()
            {
                // 重置视图的逻辑
            });

    // 其他工具栏按钮可以根据需要添加
}

void MainWindow::setupCentralWidget()
{
    // 创建中心部件
    centerWidget = new QWidget(this);
    setCentralWidget(centerWidget);

    // 创建顶部选项栏
    topPanel = new QWidget(centerWidget);
    topPanel->setFixedHeight(30);                          // 设置固定高度
    topPanel->setStyleSheet("background-color: #f0f0f0;"); // 设置背景颜色

    // 创建左右编辑栏
    leftPanel = new QWidget(centerWidget);
    leftPanel->setMinimumWidth(200);                        // 设置最小宽度
    leftPanel->setStyleSheet("background-color: #e0e0e0;"); // 设置背景颜色

    rightPanel = new QWidget(centerWidget);
    rightPanel->setMinimumWidth(200);                        // 设置最小宽度
    rightPanel->setStyleSheet("background-color: #e0e0e0;"); // 设置背景颜色

    // 创建 OpenGL 画布
    canvas = new Ui::Canvas(centerWidget);
    canvas->setMinimumSize(600, 400); // 设置最小尺寸
    canvas->setFocusPolicy(Qt::StrongFocus);
    // 创建左右分割器
    leftRightSplitter = new QSplitter(Qt::Horizontal, centerWidget);
    leftRightSplitter->addWidget(leftPanel);
    leftRightSplitter->addWidget(canvas);
    leftRightSplitter->addWidget(rightPanel);

    // 设置左右分割器的初始比例
    leftRightSplitter->setStretchFactor(0, 1);
    leftRightSplitter->setStretchFactor(1, 3);
    leftRightSplitter->setStretchFactor(2, 1);

    // 创建上下分割器
    topBottomSplitter = new QSplitter(Qt::Vertical, centerWidget);
    topBottomSplitter->addWidget(topPanel);
    topBottomSplitter->addWidget(leftRightSplitter);

    // 设置上下分割器的初始比例
    topBottomSplitter->setStretchFactor(0, 0); // 顶部选项栏不拉伸
    topBottomSplitter->setStretchFactor(1, 1); // 中间部分拉伸

    // 设置中心部件的布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centerWidget);
    mainLayout->addWidget(topBottomSplitter);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 设置主窗口的布局
    setCentralWidget(centerWidget);
}
