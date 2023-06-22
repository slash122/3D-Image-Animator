
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1280,720);

    //Translation slidery
    ui->translateX->setMaximum(100); ui->translateX->setMinimum(-100); ui->translateX->setValue(0);
    ui->translateY->setMaximum(100); ui->translateY->setMinimum(-100); ui->translateY->setValue(0);
    ui->translateZ->setMaximum(100); ui->translateZ->setMinimum(0); ui->translateZ->setValue(0);

    //Inicjalizacja controllera renderu
    drawingModel = new DrawingModel(ui->graphicsView);
    QSize gSize = ui->graphicsView->size();

    //Ustawianie default texutry
    texture = new QImage(100,100, QImage::Format_RGB32);
    QPainter textPaint(texture);
    textPaint.fillRect(0,0,100,100, Qt::white);

    //Ramka pierwszej tekstury
    //   A---D
    //   | \ |
    //   B---C
    //
    vertices.push_back(QVector4D(-1,1,0,1)); //A
    vertices.push_back(QVector4D(-1,-1,0,1)); //B
    vertices.push_back(QVector4D(1,-1,0,1)); //C
    vertices.push_back(QVector4D(1,1,0,1)); //D

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
    transformVertices(tmpV, tMatrices);

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

    tMatrices.rotateY = rotate;
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

    tMatrices.rotateX = rotate;
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

    tMatrices.rotateZ = rotate;
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


void MainWindow::on_translateX_sliderMoved(int position)
{
    float position_f = (float) position / 10.;

    QMatrix4x4 translate( 1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      position_f, 0, 0, 1);

    tMatrices.translateX = translate;
    redrawTexture();
}

void MainWindow::on_translateY_sliderMoved(int position)
{
    float position_f = (float) position / 10.;

    QMatrix4x4 translate( 1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, position_f, 0, 1);

    tMatrices.translateY = translate;
    redrawTexture();
}

void MainWindow::on_translateZ_sliderMoved(int position)
{
    float position_f = (float) position / 10.;

    QMatrix4x4 translate( 1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, position_f,
                         0, 0, position_f, 1);

    tMatrices.translateZ = translate;
    redrawTexture();
}

