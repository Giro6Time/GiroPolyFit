#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

class Navigator : public QWidget
{
    Q_OBJECT

public:
    explicit Navigator(QWidget *parent = nullptr) : QWidget(parent)
    {
        // 创建布局
        QWidget *mainWidget = new QWidget(this);
        mainWidget->setFixedWidth(500);
        QHBoxLayout *layout = new QHBoxLayout(mainWidget);
        layout->setSpacing(0);                  // 设置按钮之间的间距
        layout->setContentsMargins(0, 0, 0, 0); // 设置边距

        // 创建按钮并连接信号
        QPushButton *newButton = createButton("New", "./assets/img/new.png", &Navigator::newClicked);
        QPushButton *saveButton = createButton("Save", "./assets/img/save.png", &Navigator::saveClicked);
        QPushButton *convertButton = createButton("Convert", "./assets/img/convert.png", &Navigator::convertClicked);
        QPushButton *editButton = createButton("Edit", "./assets/img/edit.png", &Navigator::editClicked);
        QPushButton *environmentButton = createButton("Environment Setting", "./assets/img/settings.png", &Navigator::settingsClicked);
        QPushButton *aboutButton = createButton("About", "./assets/img/about.png", &Navigator::onAboutClicked);
        layout->addWidget(newButton);
        layout->addWidget(saveButton);
        layout->addWidget(convertButton);
        layout->addWidget(editButton);
        layout->addWidget(environmentButton);
        layout->addWidget(aboutButton);
    }

signals:
    void newClicked();
    void saveClicked();
    void convertClicked();
    void editClicked();
    void settingsClicked();
    void aboutClicked();

private slots:
    void onAboutClicked()
    {
        if (aboutWindow)
        {
            aboutWindow->show();
            return;
        } // 创建关于窗口
        aboutWindow = new QWidget();
        QVBoxLayout *aboutLayout = new QVBoxLayout(aboutWindow);

        QLabel *titleLabel = new QLabel("qq", aboutWindow);
        titleLabel->setAlignment(Qt::AlignCenter);
        aboutLayout->addWidget(titleLabel);

        QLabel *infoLabel = new QLabel("qqq", aboutWindow);
        infoLabel->setAlignment(Qt::AlignCenter);
        aboutLayout->addWidget(infoLabel);

        aboutWindow->setWindowTitle("qq");
        aboutWindow->resize(300, 200);
        aboutWindow->show();
    }

private:
    QPushButton *createButton(const QString &text, const QString &iconPath, void (Navigator::*signal)())
    {
        QPushButton *button = new QPushButton(this);
        button->setStyleSheet("text-align: center; padding: 5px;");
        QSize fixedSize(70, 70); // 宽度和高度可根据需要调整
        button->setFixedSize(fixedSize);

        // 创建图标
        QIcon icon(iconPath);

        // 设置按钮布局为垂直排列
        QVBoxLayout *buttonLayout = new QVBoxLayout(button);
        buttonLayout->setSpacing(0);
        buttonLayout->setContentsMargins(0, 0, 0, 0);

        QLabel *iconLabel = new QLabel(button);
        iconLabel->setStyleSheet("background-color: transparent;");
        iconLabel->setPixmap(icon.pixmap(45, 45));
        iconLabel->setAlignment(Qt::AlignCenter);

        QLabel *textLabel = new QLabel(text, button);
        textLabel->setStyleSheet("background-color: transparent;");
        textLabel->setAlignment(Qt::AlignCenter);
        buttonLayout->addWidget(iconLabel);
        buttonLayout->addWidget(textLabel);

        // 连接按钮点击信号
        if (signal)
        {
            connect(button, &QPushButton::clicked, this, signal);
        }

        return button;
    }

    QWidget *aboutWindow = nullptr;
};