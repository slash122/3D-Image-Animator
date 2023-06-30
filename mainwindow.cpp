#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qfiledialog.h"
#include <QDebug>
#include "qmessagebox.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1193,640);
    this->setWindowTitle("GFK Project N28");
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //Inicjalizacja controllera renderu
    drawingModel = new DrawingModel(ui->graphicsView);

    //Ustawianie default textur
    texture1 = new QImage(100,100, QImage::Format_RGB32);
    QPainter text1Paint(texture1);
    text1Paint.fillRect(0,0,100,100, Qt::white);

    texture2 = new QImage(100,100, QImage::Format_RGB32);
    QPainter text2Paint(texture2);
    text2Paint.fillRect(0,0,100,100, Qt::white);

    //________________________
    //Combo box initialization
    ComboBoxIsInitialized = false;
    ui->comboBoxTransType->addItem("Default transition");
    ui->comboBoxTransType->addItem("Slide from top");
    ui->comboBoxTransType->addItem("Slide from bottom");
    ui->comboBoxTransType->addItem("Slide from left side");
    ui->comboBoxTransType->addItem("Slide from right side");
    ui->comboBoxTransType->addItem("Approaching box");
    ui->comboBoxTransType->addItem("Leaving box");
    ui->comboBoxTransType->addItem("Darkening transition");
    ui->comboBoxTransType->addItem("Alpha transition");
    ui->comboBoxTransType->addItem("Y Rotation");
    ui->comboBoxTransType->addItem("X Rotation");
    ui->comboBoxTransType->addItem("Shutter");
    ui->comboBoxTransType->addItem("Tunnel");
    ui->comboBoxTransType->addItem("Cube");
    ui->comboBoxTransType->addItem("Ring");
    ui->comboBoxTransType->addItem("Blur transition");
    ui->comboBoxTransType->addItem("Pixel brightness transition");
    ComboBoxIsInitialized = true;
    //________________________
    ui->lineEditNumOfFrames->setInputMask("99");
    //
    transition = new Transition();
    transition->setModel(drawingModel);
    transition->setFirstTexture(texture1);
    transition->setSecondTexture(texture2);
    transition->initTransition( TransitionTypes::Default );

    currentFrame = 0;
    transition->drawFrame(currentFrame);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete texture1;
    delete texture2;
    delete drawingModel;
    delete transition;
}

void MainWindow::on_comboBoxTransType_currentIndexChanged(int index)
{
    if ( ComboBoxIsInitialized != false)
    {
        transition->initTransition( static_cast<TransitionTypes>(index) );
        transition->drawFrame(currentFrame);
    }
}


void MainWindow::on_pushButtonLoad1_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    if ( fileName != "")
    {
        texture1->load(fileName);
        transition->setFirstTexture(texture1);
        transition->drawFrame(currentFrame);
        ui->path1Image->setText(fileName);
    }
}

void MainWindow::on_pushButtonLoad2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    tr("Images (*.png *.xpm *.jpg)"));
    if ( fileName != "")
    {
        texture2->load(fileName);
        transition->setSecondTexture(texture2);
        transition->drawFrame(currentFrame);
        ui->path2Image->setText(fileName);
    }
}

void MainWindow::on_horizontalSliderFrameline_sliderMoved(int position)
{
    transition->drawFrame(position);
    currentFrame = position;
}


void MainWindow::on_pushButtonSave_clicked()
{
    QString lineText = ui->lineEditNumOfFrames->text();
    std::string lineString = lineText.toStdString();

    bool lineStringIsEmpty = true;
    int frameNum = 0;
    if (lineString != "")
    {
        lineStringIsEmpty = false;
        frameNum = std::stoi(lineString);
    }
    QMessageBox messageBox;


    if (frameNum < 3 || frameNum > 25 || lineStringIsEmpty == true)
    {
        messageBox.critical(0,"Error","Choose number of frames between 3 and 25");
        messageBox.setFixedSize(500,200);
        messageBox.show();
    }
    else
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Bitmap Sequence"),
                                                        QString(),
                                                        tr("Images"));

        QImage saveImage(drawingModel->getViewSize(),QImage::Format_ARGB32);
        QPainter savePainter(&saveImage);
        float frameInterval = 100. / (frameNum - 1);
        int frameNum = 0;
        for ( int i = 0; i <= 100; i+=frameInterval)
        {
            transition->drawFrame(i);
            drawingModel->getScene()->render(&savePainter);
            saveImage.save( fileName + QString::fromStdString("_" + std::to_string(frameNum) + ".bmp"));
            frameNum++;
        }

        transition->drawFrame(currentFrame);
    }
}
