#pragma once
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QStandardItem>
#include <QPainter>
#include <QMap>
#include <QLabel>
#include <memory>
#include <map_io.h>
#include <vector>
#include "map.h"
#include "point_set.h"
#include "point_set_io.h"
using MapPtr = std::shared_ptr<Map>;
using PSetPtr = std::shared_ptr<PointSet>;
class MapCache
{
public:
    struct Model
    {
        MapPtr map;
        PSetPtr pset;
    };
    Model getModel(const QString &filePath)
    {
        if (cache.contains(filePath))
            return cache[filePath];
        else
            return Model();
    }

    Model addModel(const QString &filePath, const MapPtr map, const PSetPtr pset)
    {
        Model model;
        model.map = map;
        model.pset = pset;
        cache[filePath] = model;
        return model;
    }

private:
    QMap<QString, Model> cache;
};

class StyledListItemWidget : public QWidget
{
    Q_OBJECT
public:
    StyledListItemWidget(const QString &text, QListWidget *parent = nullptr)
        : QWidget(parent)
    {

        // 分割文件名和后缀
        QFileInfo fileInfo(text);
        QString fileName = fileInfo.fileName();
        int dotPos = fileName.lastIndexOf('.');
        if (dotPos != -1 && dotPos < fileName.length() - 1) // 确保至少有一个字符在点之后
        {
            baseName = fileInfo.baseName(); // 更安全的方法获取基本名称
            extension = fileInfo.suffix();  // 更安全的方法获取后缀
            displayText = fileName;         // 保留完整文件名用于其他用途（可选）
        }
        else
        {
            baseName = fileName;
            extension = "";
        }
        setupUi();
    }

protected:
    void setupUi()
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(8); // 图标和文字之间的间距
        // 创建图标标签
        QLabel *iconLabel = new QLabel(this);
        iconLabel->setFixedSize(24, 24); // 设置图标大小
        QString iconName;
        if (extension == "ply")
            iconName = "./assets/img/cloud.png";
        else if (extension == "obj")
            iconName = "./assets/img/obj.png";
        iconLabel->setPixmap(QIcon(iconName).pixmap(24, 24)); // 设置图标
        layout->addWidget(iconLabel);

        // 创建文字标签
        // 创建文字布局
        QVBoxLayout *textLayout = new QVBoxLayout();
        textLayout->setSpacing(4); // baseName 和 extension 之间的间距

        // 创建 baseName 标签
        QLabel *baseNameLabel = new QLabel(this);
        baseNameLabel->setText(baseName);
        baseNameLabel->setFont(QFont("Arial", 16, QFont::Bold));                      // 加粗，较大字体
        baseNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);                // 左对齐
        baseNameLabel->setStyleSheet("background-color: transparent; color: black;"); // 设置背景透明
        textLayout->addWidget(baseNameLabel);

        // 创建 extension 标签
        QLabel *extensionLabel = new QLabel(this);
        extensionLabel->setText(extension.isEmpty() ? "No Extension" : extension);
        extensionLabel->setFont(QFont("Arial", 12, QFont::Normal));                   // 不加粗，较小字体
        extensionLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);               // 左对齐
        extensionLabel->setStyleSheet("background-color: transparent; color: gray;"); // 设置背景透明
        textLayout->addWidget(extensionLabel);

        // 将文字布局添加到主布局
        layout->addLayout(textLayout);

        // 设置主布局
        setLayout(layout);
        setStyleSheet("background-color: transparent;");
    }
    QString baseName;
    QString extension;
    QString displayText;
};

class FileListMap : public QListWidget
{
    Q_OBJECT
public:
    explicit FileListMap(QWidget *parent = nullptr)
        : QListWidget(parent)
    {
        setStyleSheet("QListWidget { border-radius: 10px; margin: 5px; }"
                      "QListWidget::item { border-radius: 5px;}"
                      "QListWidget::item:hover { background-color: rgb(140, 197, 195); }"
                      "QListWidget::item:selected { background-color: rgb(127, 179, 177); }"
                      "QListWidget::item:last-child { margin-bottom: 0px; }"); /* 最后一个 item 不需要间距 */
        setSpacing(3);
        connect(this, &QListWidget::itemSelectionChanged, this, &FileListMap::onItemSelected);
    }

signals:
    void
    itemSelected(const QString &filePath);

private slots:
    void onItemSelected()
    {
        QListWidgetItem *selectedItem = currentItem();
        if (selectedItem)
            emit itemSelected(selectedItem->text());
    }
};

class MyListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setupUi();
    }

    QString getCurrPath()
    {
        std::cout << m_fileList->currentRow() << std::endl;
        if (m_fileList->currentRow() >= 0)
            return paths[(m_fileList->currentRow())];
        return "";
    }

public slots:
    void openFile()
    {
        QString filePath = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Map Files (*.obj *.ply)"));
        if (!filePath.isEmpty())
        {
            if (parseAndGenerateMap(filePath))
            {
                addItem(filePath);
                emit fileAdded(filePath);
            }
        }
    }

    bool parseAndGenerateMap(const QString &filePath)
    {
        // 在这里解析文件并生成三维模型
        // 假设 generateMap 函数可以生成 Map 指针
        QFileInfo fileInfo(filePath);
        QString fileExtension = fileInfo.suffix().toLower();
        if (fileExtension == "obj")
        {
            MapPtr map = MapIO::read(filePath.toStdString());
            if (map)
            {
                emit modelGenerated(filePath, m_modelCache.addModel(filePath, map, nullptr));
                return true;
            }
        }
        else
        {
            PSetPtr pset = PointSetIO::read(filePath.toStdString());
            if (pset)
            {
                emit modelGenerated(filePath, m_modelCache.addModel(filePath, nullptr, pset));
                return true;
            }
        }
        return false;
    }

signals:
    void fileAdded(const QString &filePath);
    void modelGenerated(const QString &filePath, const MapCache::Model &model);

private:
    void setupUi()
    {
        m_fileList = new FileListMap(this);
        m_addButton = new QPushButton("Add File", this);
        connect(m_addButton, &QPushButton::clicked, this, &MyListWidget::openFile);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(m_fileList);
        layout->addWidget(m_addButton);
        setLayout(layout);

        m_fileList->setResizeMode(QListView::Adjust);
        m_fileList->setSpacing(10);
        m_modelCache = MapCache();
    }

    void addItem(const QString &filePath)
    {

        StyledListItemWidget *itemWidget = new StyledListItemWidget(filePath, m_fileList);
        QListWidgetItem *item = new QListWidgetItem();
        m_fileList->addItem(item);
        item->setSizeHint(QSize(m_fileList->width(), 60));
        item->setIcon(QIcon("cloud.png"));
        m_fileList->setItemWidget(item, itemWidget);
        paths.push_back(filePath);
    }

    FileListMap *m_fileList;
    QPushButton *m_addButton;
    std::vector<QString> paths;
    MapCache m_modelCache;
};