
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Transformations.h"
#include "Transition.h"
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
    void on_comboBoxTransType_currentIndexChanged(int index);

    void on_pushButtonLoad1_clicked();

    void on_pushButtonLoad2_clicked();

    void on_horizontalSliderFrameline_sliderMoved(int position);

    void on_pushButtonSave_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *mainScene;

    DrawingModel* drawingModel;
    Transition* transition;

    TransitionTypes type;
    QImage* texture1;
    QImage* texture2;

    int currentFrame;
    bool ComboBoxIsInitialized;
};

#endif // MAINWINDOW_H
