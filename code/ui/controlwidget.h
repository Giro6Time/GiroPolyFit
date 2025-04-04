#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QStackedWidget>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QMatrix4x4>
class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);

    // Public slots to receive model type and data
    void setModelType(const QString &modelType); // "Point Cloud" or "Mesh"
    void setModelData(const QVariant &data);     // Optional, for future use

signals:

    void modelTransformChanged(const QMatrix4x4 &transform);
    void pointCloudSettingsChanged(float pointSize, const QColor &pointColor);
    void meshSettingsChanged(bool showGrid, bool showSurface, const QColor &gridColor, const QColor &surfaceColor);
    void environmentSettingsChanged(const QVector3D &lightPosition, const QColor &lightColor, const QColor &backgroundColor, float cameraSpeed);
public slots:
    void onEditClicked();
    void onEnvironmentSettingClicked();
private slots:
    void onModelTypeChanged(int index);
    void onTransformChanged();
    void onPointSizeValueChanged(double value);
    void onPointColorButtonClicked();
    void onShowGridToggled(bool checked);
    void onShowSurfaceToggled(bool checked);
    void onMeshColorButtonClicked();
    void onSurfaceColorButtonClicked();
    void onLightPositionXValueChanged(double value);
    void onLightPositionYValueChanged(double value);
    void onLightPositionZValueChanged(double value);
    void onLightColorButtonClicked();
    void onBackgroundColorButtonClicked();
    void onCameraSpeedValueChanged(double value);

private:
    QVBoxLayout *mainLayout;
    QVBoxLayout *transformLayout;
    QHBoxLayout *poistionLayout;
    QDoubleSpinBox *positionXSpinBox, *positionYSpinBox, *positionZSpinBox;
    QHBoxLayout *rotationLayout;
    QDoubleSpinBox *rotationXSpinBox, *rotationYSpinBox, *rotationZSpinBox;
    QHBoxLayout *scaleLayout;
    QDoubleSpinBox *scaleXSpinBox, *scaleYSpinBox, *scaleZSpinBox;
    QPushButton *resetTransformButton;

    QStackedWidget *settingsStackedWidget;
    QWidget *pointCloudSettingsWidget;
    QDoubleSpinBox *pointSizeSpinBox;
    QPushButton *pointColorButton;

    QWidget *meshSettingsWidget;
    QCheckBox *showGridCheckBox, *showSurfaceCheckBox;
    QPushButton *meshColorButton, *surfaceColorButton;

    QWidget *environmentSettingsWidget;
    QDoubleSpinBox *lightPosXSpinBox, *lightPosYSpinBox, *lightPosZSpinBox;
    QPushButton *lightColorButton;
    QPushButton *backgroundColorButton;
    QDoubleSpinBox *cameraSpeedSpinBox;

    QString currentModelType;

    // Log 区域和进度条
    QVBoxLayout *logAndProgressLayout;
    QPlainTextEdit *logTextEdit;
    QProgressBar *progressBar;

    QComboBox *modelTypeComboBox;
};
