#include "controlwidget.h"
#include <QLabel>
#include <QVector3D>
ControlWidget::ControlWidget(QWidget *parent) : QWidget(parent), currentModelType("")
{
    mainLayout = new QVBoxLayout(this);

    // Model type selection
    QHBoxLayout *modelTypeLayout = new QHBoxLayout();
    QLabel *modelTypeLabel = new QLabel("Model Type:");
    modelTypeComboBox = new QComboBox();
    modelTypeComboBox->addItems({"Point Cloud", "Mesh"});
    modelTypeLayout->addWidget(modelTypeLabel);
    modelTypeLayout->addWidget(modelTypeComboBox);
    mainLayout->addLayout(modelTypeLayout);

    connect(modelTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ControlWidget::onModelTypeChanged);

    // Transform controls (common for both model types)
    transformLayout = new QVBoxLayout();

    poistionLayout = new QHBoxLayout();
    QLabel *positionLabel = new QLabel("Position (X, Y, Z):");
    positionXSpinBox = new QDoubleSpinBox();
    positionYSpinBox = new QDoubleSpinBox();
    positionZSpinBox = new QDoubleSpinBox();

    rotationLayout = new QHBoxLayout();
    QLabel *rotationLabel = new QLabel("Rotation (X, Y, Z):");
    rotationXSpinBox = new QDoubleSpinBox();
    rotationYSpinBox = new QDoubleSpinBox();
    rotationZSpinBox = new QDoubleSpinBox();

    scaleLayout = new QHBoxLayout();
    QLabel *scaleLabel = new QLabel("Scale:");
    scaleXSpinBox = new QDoubleSpinBox();
    scaleYSpinBox = new QDoubleSpinBox();
    scaleZSpinBox = new QDoubleSpinBox();

    QPushButton *resetButton = new QPushButton("Reset Transform");
    poistionLayout->addWidget(positionLabel);
    poistionLayout->addWidget(positionXSpinBox);
    poistionLayout->addWidget(positionYSpinBox);
    poistionLayout->addWidget(positionZSpinBox);

    rotationLayout->addWidget(rotationLabel);
    rotationLayout->addWidget(rotationXSpinBox);
    rotationLayout->addWidget(rotationYSpinBox);
    rotationLayout->addWidget(rotationZSpinBox);

    scaleLayout->addWidget(scaleLabel);
    scaleLayout->addWidget(scaleXSpinBox);
    scaleLayout->addWidget(scaleYSpinBox);
    scaleLayout->addWidget(scaleZSpinBox);

    transformLayout->addLayout(poistionLayout);
    transformLayout->addSpacing(10);
    transformLayout->addLayout(rotationLayout);
    transformLayout->addSpacing(10);
    transformLayout->addLayout(scaleLayout);
    transformLayout->addWidget(resetButton);
    connect(positionXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);
    connect(positionYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);
    connect(positionZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);
    connect(rotationXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);
    connect(rotationYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);
    connect(rotationZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);
    connect(scaleXSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);
    connect(scaleYSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);
    connect(scaleZSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onTransformChanged);

    mainLayout->addLayout(transformLayout);

    // Stacked widget for model-specific settings
    settingsStackedWidget = new QStackedWidget();
    pointCloudSettingsWidget = new QWidget();
    meshSettingsWidget = new QWidget();
    environmentSettingsWidget = new QWidget();

    settingsStackedWidget->addWidget(pointCloudSettingsWidget);
    settingsStackedWidget->addWidget(meshSettingsWidget);
    settingsStackedWidget->addWidget(environmentSettingsWidget);

    mainLayout->addWidget(settingsStackedWidget);

    // Point Cloud Settings
    auto *pointCloudVBoxLayout = new QVBoxLayout(pointCloudSettingsWidget);
    auto *pointsizeVBoxLayout = new QVBoxLayout(pointCloudSettingsWidget);
    QLabel *pointSizeLabel = new QLabel("Point Size:");
    pointSizeSpinBox = new QDoubleSpinBox();
    pointSizeSpinBox->setRange(1.0, 100.0);
    pointSizeSpinBox->setFixedHeight(24);
    pointSizeSpinBox->setValue(5.0);

    pointColorButton = new QPushButton("Point Color");
    pointCloudVBoxLayout->addLayout(pointsizeVBoxLayout);
    pointsizeVBoxLayout->addWidget(pointSizeLabel);
    pointsizeVBoxLayout->addWidget(pointSizeSpinBox);
    pointCloudVBoxLayout->addWidget(pointColorButton);
    pointCloudVBoxLayout->addSpacing(80);

    connect(pointColorButton, &QPushButton::clicked, this, &ControlWidget::onPointColorButtonClicked);
    connect(pointSizeSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ControlWidget::onPointSizeValueChanged);

    // Mesh Settings
    meshSettingsWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    auto *meshVBoxLayout = new QVBoxLayout(meshSettingsWidget);
    showGridCheckBox = new QCheckBox("Show Grid");
    showSurfaceCheckBox = new QCheckBox("Show Surface");
    // TODO::明天见
    meshColorButton = new QPushButton("Grid Color");
    surfaceColorButton = new QPushButton("Surface Color");

    QPushButton *applyMeshButton = new QPushButton("Apply Mesh Settings");

    meshVBoxLayout->addWidget(showGridCheckBox);
    meshVBoxLayout->addWidget(showSurfaceCheckBox);
    meshVBoxLayout->addWidget(meshColorButton);
    meshVBoxLayout->addWidget(surfaceColorButton);
    meshVBoxLayout->addWidget(applyMeshButton);

    connect(meshColorButton, &QPushButton::clicked, this, &ControlWidget::onMeshColorButtonClicked);
    connect(surfaceColorButton, &QPushButton::clicked, this, &ControlWidget::onSurfaceColorButtonClicked);
    connect(applyMeshButton, &QPushButton::clicked, this, [this]()
            { emit meshSettingsChanged(showGridCheckBox->isChecked(), showSurfaceCheckBox->isChecked(),
                                       meshColorButton->palette().color(QPalette::Button),
                                       surfaceColorButton->palette().color(QPalette::Button)); });

    // Environment Settings
    auto *environmentVBoxLayout = new QVBoxLayout(environmentSettingsWidget);
    QLabel *lightPosLabel = new QLabel("Light Position (X, Y, Z):");
    lightPosLabel->setFixedHeight(24);
    lightPosXSpinBox = new QDoubleSpinBox();
    lightPosYSpinBox = new QDoubleSpinBox();
    lightPosZSpinBox = new QDoubleSpinBox();

    lightColorButton = new QPushButton("Light Color");
    backgroundColorButton = new QPushButton("Background Color");
    QLabel *cameraSpeedLabel = new QLabel("Camera Speed:");
    cameraSpeedLabel->setFixedHeight(24);
    cameraSpeedSpinBox = new QDoubleSpinBox();
    cameraSpeedSpinBox->setRange(0.1, 100.0);
    cameraSpeedSpinBox->setValue(1.0);

    QPushButton *applyEnvironmentButton = new QPushButton("Apply Environment Settings");

    environmentVBoxLayout->addWidget(lightPosLabel);
    environmentVBoxLayout->addWidget(lightPosXSpinBox);
    environmentVBoxLayout->addWidget(lightPosYSpinBox);
    environmentVBoxLayout->addWidget(lightPosZSpinBox);
    environmentVBoxLayout->addWidget(lightColorButton);
    environmentVBoxLayout->addWidget(backgroundColorButton);
    environmentVBoxLayout->addWidget(cameraSpeedLabel);
    environmentVBoxLayout->addWidget(cameraSpeedSpinBox);
    environmentVBoxLayout->addWidget(applyEnvironmentButton);

    connect(lightColorButton, &QPushButton::clicked, this, &ControlWidget::onLightColorButtonClicked);
    connect(backgroundColorButton, &QPushButton::clicked, this, &ControlWidget::onBackgroundColorButtonClicked);
    connect(applyEnvironmentButton, &QPushButton::clicked, this, [this]()
            { emit environmentSettingsChanged(
                  QVector3D(lightPosXSpinBox->value(), lightPosYSpinBox->value(), lightPosZSpinBox->value()),
                  lightColorButton->palette().color(QPalette::Button),
                  backgroundColorButton->palette().color(QPalette::Button),
                  cameraSpeedSpinBox->value()); });

    // Reset Transform Button
    connect(resetButton, &QPushButton::clicked, this, [this]()
            {
        positionXSpinBox->setValue(0.0);
        positionYSpinBox->setValue(0.0);
        positionZSpinBox->setValue(0.0);
        rotationXSpinBox->setValue(0.0);
        rotationYSpinBox->setValue(0.0);
        rotationZSpinBox->setValue(0.0); });

    // Model Type Change
    connect(modelTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
            {
        QString type = modelTypeComboBox->itemText(index);
        setModelType(type); });

    // Default to environment settings
    settingsStackedWidget->setCurrentIndex(2);

    // Log 区域和进度条
    logAndProgressLayout = new QVBoxLayout();
    logTextEdit = new QPlainTextEdit();
    logTextEdit->setReadOnly(true); // 设置为只读
    progressBar = new QProgressBar();
    progressBar->setRange(0, 100); // 设置进度条范围
    progressBar->setValue(0);      // 默认值为 0

    logAndProgressLayout->addWidget(logTextEdit);
    logAndProgressLayout->addWidget(progressBar);

    mainLayout->addLayout(logAndProgressLayout);
}

void ControlWidget::setModelType(const QString &modelType)
{
    if (currentModelType == modelType)
        return;

    currentModelType = modelType;

    if (modelType == "Point Cloud")
    {
        settingsStackedWidget->setCurrentWidget(pointCloudSettingsWidget);
    }
    else if (modelType == "Mesh")
    {
        settingsStackedWidget->setCurrentWidget(meshSettingsWidget);
    }
    else
    {
        settingsStackedWidget->setCurrentWidget(environmentSettingsWidget);
    }
}
void ControlWidget::onModelTypeChanged(int index)
{
    QString type = modelTypeComboBox->itemText(index);
    setModelType(type);
}

void ControlWidget::onTransformChanged()
{
    QVector3D position(positionXSpinBox->value(), positionYSpinBox->value(), positionZSpinBox->value());
    QVector3D rotation(rotationXSpinBox->value(), rotationYSpinBox->value(), rotationZSpinBox->value());
    QQuaternion quaternion = QQuaternion::fromEulerAngles(rotation.x(), rotation.y(), rotation.z());
    QVector3D scale(scaleXSpinBox->value(), scaleYSpinBox->value(), scaleZSpinBox->value());
    QMatrix4x4 modelMatrix;
    modelMatrix.translate(position);
    modelMatrix.rotate(quaternion);
    modelMatrix.scale(scale);
    emit modelTransformChanged(modelMatrix);
}

void ControlWidget::onPointSizeValueChanged(double value)
{
    // 点大小变化时的逻辑
    QColor color = pointColorButton->palette().color(QPalette::Button);
    emit pointCloudSettingsChanged(value, color);
}
void ControlWidget::onPointColorButtonClicked()
{
    QColor color = color = pointColorButton->palette().color(QPalette::Button);
    emit pointCloudSettingsChanged(pointSizeSpinBox->value(), color);
}
void ControlWidget::onShowGridToggled(bool checked)
{
    // 显示网格变化时的逻辑
    bool showSurface = showSurfaceCheckBox->isChecked();
    QColor gridColor = meshColorButton->palette().color(QPalette::Button);
    QColor surfaceColor = surfaceColorButton->palette().color(QPalette::Button);
    emit meshSettingsChanged(checked, showSurface, gridColor, surfaceColor);
}

void ControlWidget::onShowSurfaceToggled(bool checked)
{
    // 显示表面变化时的逻辑
    bool showGrid = showGridCheckBox->isChecked();
    QColor gridColor = meshColorButton->palette().color(QPalette::Button);
    QColor surfaceColor = surfaceColorButton->palette().color(QPalette::Button);
    emit meshSettingsChanged(showGrid, checked, gridColor, surfaceColor);
}

void ControlWidget::onMeshColorButtonClicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "select mesh color");
    if (color.isValid())
    {
        meshColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}
void ControlWidget::onSurfaceColorButtonClicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "select surface color");
    if (color.isValid())
    {
        surfaceColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}

void ControlWidget::onLightPositionXValueChanged(double value)
{
    // 更新光照 X 坐标
    QVector3D lightPosition(value, lightPosYSpinBox->value(), lightPosZSpinBox->value());
    emit environmentSettingsChanged(lightPosition, QColor(255, 255, 255), QColor(0, 0, 0), 1.0f); // 示例信号
}

void ControlWidget::onLightPositionYValueChanged(double value)
{
    // 更新光照 Y 坐标
    QVector3D lightPosition(lightPosXSpinBox->value(), value, lightPosZSpinBox->value());
    emit environmentSettingsChanged(lightPosition, QColor(255, 255, 255), QColor(0, 0, 0), 1.0f); // 示例信号
}
void ControlWidget::onLightColorButtonClicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "select surface color");
    if (color.isValid())
    {
        lightColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}

void ControlWidget::onBackgroundColorButtonClicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "select surface color");
    if (color.isValid())
    {
        backgroundColorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
    }
}

void ControlWidget::onLightPositionZValueChanged(double value)
{
    // 更新光照 Z 坐标
    QVector3D lightPosition(lightPosXSpinBox->value(), lightPosYSpinBox->value(), value);
    emit environmentSettingsChanged(lightPosition, QColor(255, 255, 255), QColor(0, 0, 0), 1.0f); // 示例信号
}

void ControlWidget::onCameraSpeedValueChanged(double value)
{
    // 更新相机速度
    emit environmentSettingsChanged(QVector3D(0, 0, 0), QColor(255, 255, 255), QColor(0, 0, 0), value);
}

void ControlWidget::onEditClicked()
{
    modelTypeComboBox->setCurrentIndex(0);
    setModelType("Mesh");
}
void ControlWidget::onEnvironmentSettingClicked()
{
    modelTypeComboBox->setCurrentIndex(2);
    setModelType("Environment");
}