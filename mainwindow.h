
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Transformations.h"
#include "qgraphicsscene.h"
#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include "DrawingModel.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_rotateX_sliderMoved(int position);
    void on_rotateY_sliderMoved(int position);
    void on_rotateZ_sliderMoved(int position);

    void on_loadTexture_clicked();

    void on_translateX_sliderMoved(int position);

    void on_translateY_sliderMoved(int position);

    void on_translateZ_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *mainScene;

    QImage* texture;

    DrawingModel* drawingModel;

    TransformationMatrices tMatrices;
    std::vector<QVector4D> vertices;
    std::vector<QVector3D> triangles;
    std::vector<QVector2D> UVvertices;

    void redrawTexture();
};

#endif // MAINWINDOW_H
