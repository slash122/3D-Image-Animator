
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1280,720);

    drawingModel = new DrawingModel(ui->graphicsView);
    QSize gSize = ui->graphicsView->size();

    texture = new QImage(100,100, QImage::Format_RGB32);
    QPainter textPaint(texture);
    textPaint.fillRect(0,0,100,100, Qt::white);

    //   A---D
    //   | \ |
    //   B---C
    //
    vertices.push_back(QVector4D(-0.3,0.3,0,1)); //A
    vertices.push_back(QVector4D(-0.3,-0.3,0,1)); //B
    vertices.push_back(QVector4D(0.3,-0.3,0,1)); //C
    vertices.push_back(QVector4D(0.3,0.3,0,1)); //D

    triangles.push_back(QVector3D(2,1,0)); //CBA
    triangles.push_back(QVector3D(2,0,3)); //CAD

    UVvertices.push_back(QVector2D(0,0)); //A
    UVvertices.push_back(QVector2D(0,1)); //B
    UVvertices.push_back(QVector2D(1,1)); //C
    UVvertices.push_back(QVector2D(1,0)); //D

    std::vector<QVector3D> vertScreen = toScreen(vertices, gSize);
    drawingModel->mapTexture(vertScreen,triangles,UVvertices,*texture);
}


void MainWindow::redrawTexture()
{

    QSize gSize = ui->graphicsView->size();
    drawingModel->endOfFrame();

    std::vector<QVector4D> tmpV = vertices;
    transformVertices(tmpV, rotMatrices);
    std::vector<QVector3D> vertScreen = toScreen(tmpV, gSize);
    drawingModel->mapTexture(vertScreen,triangles,UVvertices,*texture);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_rotateY_sliderMoved(int position)
{
    position *= 3;
    double radians = position * 3.1415 / 180.0f;

    QMatrix4x4 rotate( cos(radians), 0, sin(radians), 0,
                      0, 1, 0, 0,
                      -sin(radians), 0, cos(radians), 0,
                      0, 0, 0, 1);

    rotMatrices.rotateY = rotate;
    redrawTexture();
}


void MainWindow::on_rotateX_sliderMoved(int position)
{
    position *= 3;
    double radians = position * 3.1415 / 180.0f;

    QMatrix4x4 rotate( 1, 0, 0, 0,
                      0, cos(radians), -sin(radians), 0,
                      0, sin(radians), cos(radians), 0,
                      0, 0, 0, 1);

    rotMatrices.rotateX = rotate;
    redrawTexture();
}


void MainWindow::on_rotateZ_sliderMoved(int position)
{
    position *= 3;
    double radians = position * 3.1415 / 180.0f;

    QMatrix4x4 rotate( cos(radians), -sin(radians), 0, 0,
                      sin(radians), cos(radians), 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);

    rotMatrices.rotateZ = rotate;
    redrawTexture();
}


void MainWindow::on_loadTexture_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    texture->load(fileName);
    redrawTexture();
}

