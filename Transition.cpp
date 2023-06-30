#include "Transition.h"
#include <QGraphicsPixmapItem>
#include "QGraphicsBlurEffect"

QT_BEGIN_NAMESPACE
    extern Q_WIDGETS_EXPORT void qt_blurImage( QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0 );
QT_END_NAMESPACE

void Transition::setFirstTexture(QImage* tex1)
{
    texture1 = tex1;
    scaledTex1 = texture1->scaled(drawingModel->getViewSize());
    scaledTex1 = scaledTex1.convertToFormat(QImage::Format_ARGB32);
}
void Transition::setSecondTexture(QImage* tex2)
{
    texture2 = tex2;
    scaledTex2 = texture2->scaled(drawingModel->getViewSize());
    scaledTex2 = scaledTex2.convertToFormat(QImage::Format_ARGB32);
}


void Transition::setModel(DrawingModel* model)
{
    drawingModel = model;
}


void Transition::initTransition(TransitionTypes type)
{
    switch ( currentType )
    {
    case TransitionTypes::RotationY:
        clearRotationY();
        break;
    case TransitionTypes::RotationX:
        clearRotationX();
        break;
    case TransitionTypes::Shutter:
        clearShutter();
        break;
    case TransitionTypes::Cube:
        clearCube();
        break;
    default:
        break;
    }

    currentType = type;

    switch ( currentType )
    {
    case TransitionTypes::RotationY:
        initRotationY();
        break;
    case TransitionTypes::RotationX:
        initRotationX();
        break;
    case TransitionTypes::Shutter:
        initShutter();
        break;
    case TransitionTypes::Cube:
        initCube();
        break;
    default:
        break;
    }
}

void Transition::drawFrame(const int& frame)
{
    switch(currentType)
    {
    case TransitionTypes::Default:
        drawFrameDefault(frame);
        break;
    case TransitionTypes::SlideTop:
        drawFrameSlideTop(frame);
        break;
    case TransitionTypes::SlideBottom:
        drawFrameSlideBottom(frame);
        break;
    case TransitionTypes::SlideLeft:
        drawFrameSlideLeft(frame);
        break;
    case TransitionTypes::SlideRight:
        drawFrameSlideRight(frame);
        break;
    case TransitionTypes::ApproachingBox:
        drawFrameApproachingBox(frame);
        break;
    case TransitionTypes::LeavingBox:
        drawFrameLeavingBox(frame);
        break;
    case TransitionTypes::Darkening:
        drawFrameDarkening(frame);
        break;
    case TransitionTypes::Alpha:
        drawFrameAlpha(frame);
        break;
    case TransitionTypes::RotationY:
        drawFrameRotationY(frame);
        break;
    case TransitionTypes::RotationX:
        drawFrameRotationX(frame);
        break;
    case TransitionTypes::Shutter:
        drawFrameShutter(frame);
        break;
    case TransitionTypes::Tunnel:
        drawFrameTunnel(frame);
        break;
    case TransitionTypes::Cube:
        drawFrameCube(frame);
        break;
    case TransitionTypes::Ring:
        drawFrameRing(frame);
        break;
    case TransitionTypes::Blur:
        drawFrameBlur(frame);
        break;
    case TransitionTypes::PixelBrightness:
        drawFramePixelBrightness(frame);
        break;

    default:
        break;
    }
}

//__________________
//DEFAULT TRANSITION

void Transition::drawFrameDefault(const int& frame)
{
    drawingModel->getScene()->clear();

    if ( frame < 50)
    {
        drawingModel->getScene()->addPixmap( QPixmap::fromImage(scaledTex1) );
    }
    else
    {
        drawingModel->getScene()->addPixmap( QPixmap::fromImage(scaledTex2) );
    }
}

//____________________
//SLIDE TOP TRANSITION

void Transition::drawFrameSlideTop(const int& frame)
{
    drawingModel->getScene()->clear();

    int slideOffset;
    slideOffset = ( (double) frame / 100. ) * scaledTex2.height();

    QImage frameImage(scaledTex1);
    QPainter framePainter(&frameImage);
    framePainter.drawImage(QPoint(0,0), scaledTex2, QRect( 0, scaledTex2.height()-slideOffset, scaledTex2.width(), scaledTex2.height() ));

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}

//____________________
//SLIDE BOTTOM TRANSITION

void Transition::drawFrameSlideBottom(const int& frame)
{
    drawingModel->getScene()->clear();

    int slideOffset;
    slideOffset = ( (double) frame / 100. ) * scaledTex2.height();

    QImage frameImage(scaledTex1);
    QPainter framePainter(&frameImage);
    framePainter.drawImage(QPoint(0, scaledTex1.height()-slideOffset), scaledTex2, QRect( 0, 0, scaledTex2.width(), slideOffset ));

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}

//____________________
//SLIDE LEFT TRANSITION

void Transition::drawFrameSlideLeft(const int& frame)
{
    drawingModel->getScene()->clear();

    int slideOffset;
    slideOffset = ( (double) frame / 100. ) * scaledTex2.width();

    QImage frameImage(scaledTex1);
    QPainter framePainter(&frameImage);
    framePainter.drawImage(QPoint(0, 0), scaledTex2, QRect( scaledTex2.width()-slideOffset, 0, scaledTex2.width(), scaledTex2.height() ));

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}

//____________________
//SLIDE RIGHT TRANSITION


void Transition::drawFrameSlideRight(const int& frame)
{
    drawingModel->getScene()->clear();

    int slideOffset;
    slideOffset = ( (double) frame / 100. ) * scaledTex2.width();

    QImage frameImage(scaledTex1);
    QPainter framePainter(&frameImage);
    framePainter.drawImage(QPoint( scaledTex2.width()-slideOffset, 0), scaledTex2, QRect( 0, 0, slideOffset, scaledTex2.height() ));

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}

//____________________
//APPROACHING BOX TRANSITION

void Transition::drawFrameApproachingBox(const int& frame)
{
    drawingModel->getScene()->clear();

    int BoxWidth, BoxHeight;
    BoxWidth = ( (double) frame / 100. ) * scaledTex2.width();
    BoxHeight = ( (double) frame / 100. ) * scaledTex2.height();

    QImage frameImage(scaledTex1);
    QPainter framePainter(&frameImage);
    QRect BoxRect( scaledTex1.width()/2 - BoxWidth/2, scaledTex1.height()/2 - BoxHeight/2, BoxWidth, BoxHeight);
    framePainter.drawImage(BoxRect, scaledTex2);

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}

//____________________
//LEAVING BOX TRANSITION

void Transition::drawFrameLeavingBox(const int& frame)
{
    drawingModel->getScene()->clear();

    int BoxWidth, BoxHeight;
    BoxWidth = ( (double) (100 - frame) / 100. ) * scaledTex2.width();
    BoxHeight = ( (double) (100 - frame) / 100. ) * scaledTex2.height();

    QImage frameImage(scaledTex2);
    QPainter framePainter(&frameImage);
    QRect BoxRect( scaledTex1.width()/2 - BoxWidth/2, scaledTex1.height()/2 - BoxHeight/2, BoxWidth, BoxHeight);
    framePainter.drawImage(BoxRect, scaledTex1);

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}

//____________________
//DARKENING TRANSITION

void Transition::drawFrameDarkening(const int& frame)
{
    drawingModel->getScene()->clear();
    QImage frameImage;

    if ( frame <= 50)
    {
        frameImage = scaledTex1;

        float fFrame = static_cast<float>(frame);
        float darkening = fFrame / 50.;

        for (int y = 0; y < frameImage.height(); ++y) {
            QRgb *line = reinterpret_cast<QRgb*>(frameImage.scanLine(y));
            for (int x = 0; x < frameImage.width(); ++x) {
                QRgb &rgb = line[x];
                float Red =  qRed(rgb);
                float Green =  qGreen(rgb);
                float Blue =  qBlue(rgb);
                rgb = qRgb(Red - Red*darkening, Green - Green*darkening, Blue - Blue*darkening);
            }
        }
    }
    else
    {
        frameImage = scaledTex2;

        float fFrame = static_cast<float>(frame);
        float invDarkening = (fFrame - 50.) / 50.;

        for (int y = 0; y < frameImage.height(); ++y) {
            QRgb *line = reinterpret_cast<QRgb*>(frameImage.scanLine(y));
            for (int x = 0; x < frameImage.width(); ++x) {
                QRgb &rgb = line[x];
                float Red =  qRed(rgb);
                float Green =  qGreen(rgb);
                float Blue =  qBlue(rgb);
                rgb = qRgb(Red*invDarkening, Green*invDarkening, Blue*invDarkening);
            }
        }
    }

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}

//____________________
//ALPHA TRANSITION

void Transition::drawFrameAlpha(const int& frame)
{
    drawingModel->getScene()->clear();
    QImage frameImage = scaledTex1;

    float alphaProc = ( 100 - (float) frame ) / 100. * 255.;

    for (int y = 0; y < frameImage.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(frameImage.scanLine(y));
        for (int x = 0; x < frameImage.width(); ++x) {
            QRgb &rgb = line[x];
            float newAlpha =  alphaProc;
            rgb = qRgba(qRed(rgb), qGreen(rgb), qBlue(rgb), newAlpha);
        }
    }

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(scaledTex2) );
    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}

//____________________
//Y ROTATION TRANSITION

void Transition::initRotationY()
{
    vertices1.push_back(QVector4D(-1,1,0,1)); //A
    vertices1.push_back(QVector4D(-1,-1,0,1)); //B
    vertices1.push_back(QVector4D(1,-1,0,1)); //C
    vertices1.push_back(QVector4D(1,1,0,1)); //D

    vertices2.push_back(QVector4D(-1,-1,0,1)); //B
    vertices2.push_back(QVector4D(-1,1,0,1)); //A
    vertices2.push_back(QVector4D(1,1,0,1)); //D
    vertices2.push_back(QVector4D(1,-1,0,1)); //C


    triangles.push_back(QVector3D(2,1,0)); //CBA
    triangles.push_back(QVector3D(2,0,3)); //CAD


    UVvertices.push_back(QVector2D(0,0)); //A
    UVvertices.push_back(QVector2D(0,1)); //B
    UVvertices.push_back(QVector2D(1,1)); //C
    UVvertices.push_back(QVector2D(1,0)); //D

    drawingModel->endOfFrame();
}

void Transition::drawFrameRotationY(const int& frame)
{
    drawingModel->endOfFrame();
    float radians = (float) frame / 100. * M_PI;
    if ( frame <= 50)
    {
        float translationZ = (float) frame / 50. * 2.;
        QMatrix4x4 rotate( cos(radians), 0, sin(radians), 0,
                          0, 1, 0, 0,
                          -sin(radians), 0, cos(radians), 0,
                          0, 0, 0, 1);

        QMatrix4x4 translate( 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, translationZ,
                             0, 0, translationZ, 1);

        TransformationMatrices tMatrices;
        tMatrices.translateZ = translate;
        tMatrices.rotateY = rotate;

        std::vector<QVector4D> tmpV = vertices1;
        transformVertices(tmpV, tMatrices);

        std::vector<QVector3D> vertScreen = toScreen(tmpV, drawingModel->getViewSize());
        drawingModel->mapTexture(vertScreen,triangles,UVvertices,scaledTex1);
    }
    else
    {
        float invTranslationZ = 2. - ( ( (float) frame - 50. ) / 50. * 2. );
        QMatrix4x4 rotate( cos(radians), 0, sin(radians), 0,
                          0, 1, 0, 0,
                          -sin(radians), 0, cos(radians), 0,
                          0, 0, 0, 1);

        QMatrix4x4 translate( 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, invTranslationZ,
                             0, 0, invTranslationZ, 1);

        TransformationMatrices tMatrices;
        tMatrices.translateZ = translate;
        tMatrices.rotateY = rotate;

        std::vector<QVector4D> tmpV = vertices2;
        transformVertices(tmpV, tMatrices);

        std::vector<QVector3D> vertScreen = toScreen(tmpV, drawingModel->getViewSize());
        drawingModel->mapTexture(vertScreen,triangles,UVvertices,scaledTex2);
    }
}

void Transition::clearRotationY()
{
    drawingModel->endOfFrame();
    vertices1.erase(vertices1.begin(), vertices1.end());
    vertices2.erase(vertices2.begin(), vertices2.end());
    triangles.erase(triangles.begin(), triangles.end());
    UVvertices.erase(UVvertices.begin(), UVvertices.end());
}

//____________________
//X ROTATION TRANSITION

void Transition::initRotationX()
{
    vertices1.push_back(QVector4D(-1,1,0,1)); //A
    vertices1.push_back(QVector4D(-1,-1,0,1)); //B
    vertices1.push_back(QVector4D(1,-1,0,1)); //C
    vertices1.push_back(QVector4D(1,1,0,1)); //D

    vertices2.push_back(QVector4D(-1,-1,0,1)); //B
    vertices2.push_back(QVector4D(-1,1,0,1)); //A
    vertices2.push_back(QVector4D(1,1,0,1)); //D
    vertices2.push_back(QVector4D(1,-1,0,1)); //C

    triangles.push_back(QVector3D(2,1,0)); //CBA
    triangles.push_back(QVector3D(2,0,3)); //CAD

    UVvertices.push_back(QVector2D(0,0)); //A
    UVvertices.push_back(QVector2D(0,1)); //B
    UVvertices.push_back(QVector2D(1,1)); //C
    UVvertices.push_back(QVector2D(1,0)); //D

    drawingModel->endOfFrame();
}

void Transition::drawFrameRotationX(const int& frame)
{
    drawingModel->endOfFrame();
    float radians = (float) frame / 100. * M_PI;
    if ( frame <= 50)
    {
        float translationZ = (float) frame / 50. * 2.;
        QMatrix4x4 rotate( 1, 0, 0, 0,
                          0, cos(radians), -sin(radians), 0,
                          0, sin(radians), cos(radians), 0,
                          0, 0, 0, 1);

        QMatrix4x4 translate( 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, translationZ,
                             0, 0, translationZ, 1);

        TransformationMatrices tMatrices;
        tMatrices.translateZ = translate;
        tMatrices.rotateX = rotate;

        std::vector<QVector4D> tmpV = vertices1;
        transformVertices(tmpV, tMatrices);

        std::vector<QVector3D> vertScreen = toScreen(tmpV, drawingModel->getViewSize());
        drawingModel->mapTexture(vertScreen,triangles,UVvertices,scaledTex1);
    }
    else
    {
        float invTranslationZ = 2. - ( ( (float) frame - 50. ) / 50. * 2. );
        QMatrix4x4 rotate( 1, 0, 0, 0,
                          0, cos(radians), -sin(radians), 0,
                          0, sin(radians), cos(radians), 0,
                          0, 0, 0, 1);

        QMatrix4x4 translate( 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, invTranslationZ,
                             0, 0, invTranslationZ, 1);

        TransformationMatrices tMatrices;
        tMatrices.translateZ = translate;
        tMatrices.rotateX = rotate;

        std::vector<QVector4D> tmpV = vertices2;
        transformVertices(tmpV, tMatrices);

        std::vector<QVector3D> vertScreen = toScreen(tmpV, drawingModel->getViewSize());
        drawingModel->mapTexture(vertScreen,triangles,UVvertices,scaledTex2);
    }
}

void Transition::clearRotationX()
{
    drawingModel->endOfFrame();
    vertices1.erase(vertices1.begin(), vertices1.end());
    vertices2.erase(vertices2.begin(), vertices2.end());
    triangles.erase(triangles.begin(), triangles.end());
    UVvertices.erase(UVvertices.begin(), UVvertices.end());
}

//____________________
//SHUTTER TRANSITION

void Transition::initShutter()
{
    vertices1.push_back(QVector4D(-1,1,0,1)); //A
    vertices1.push_back(QVector4D(-1,-1,0,1)); //B
    vertices1.push_back(QVector4D(1,-1,0,1)); //C
    vertices1.push_back(QVector4D(1,1,0,1)); //D

    triangles.push_back(QVector3D(2,1,0)); //CBA
    triangles.push_back(QVector3D(2,0,3)); //CAD

    UVvertices.push_back(QVector2D(0,0)); //A
    UVvertices.push_back(QVector2D(0,1)); //B
    UVvertices.push_back(QVector2D(1,1)); //C
    UVvertices.push_back(QVector2D(1,0)); //D

    drawingModel->endOfFrame();
}

void Transition::drawFrameShutter(const int& frame)
{
    drawingModel->endOfFrame();
    float radians;
    if ( frame < 50)
    {
        radians =  (float) frame / 50. * 3 * M_PI / 4;
        QMatrix4x4 rotateY( cos(-radians), 0, sin(-radians), 0,
                          0, 1, 0, 0,
                          -sin(-radians), 0, cos(-radians), 0,
                          0, 0, 0, 1);

        TransformationMatrices rotMatrix;
        rotMatrix.rotateY = rotateY;

        std::vector<QVector4D> tmpV = vertices1;
        transformVertices(tmpV, rotMatrix);

        TransformationMatrices translationMatrices;
        float translationZ = 1 - (tmpV[3].z()+1);
        QMatrix4x4 translateZ( 1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, translationZ,
                              0, 0, translationZ, 1);

        float translationX = 1 - tmpV[3].x();
        QMatrix4x4 translateX( 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, 0,
                             translationX, 0, 0, 1);
        translationMatrices.translateX = translateX;
        translationMatrices.translateZ = translateZ;
        transformVertices(tmpV, translationMatrices);

        std::vector<QVector3D> vertScreen = toScreen(tmpV, drawingModel->getViewSize());
        drawingModel->mapTexture(vertScreen,triangles,UVvertices,scaledTex1);
    }
    else
    {
        radians = 3 * M_PI / 4 - ((float) frame - 50. ) / 50. * 3 * M_PI / 4;
        QMatrix4x4 rotateY( cos(radians), 0, sin(radians), 0,
                           0, 1, 0, 0,
                           -sin(radians), 0, cos(radians), 0,
                           0, 0, 0, 1);

        TransformationMatrices rotMatrix;
        rotMatrix.rotateY = rotateY;

        std::vector<QVector4D> tmpV = vertices1;
        transformVertices(tmpV, rotMatrix);

        TransformationMatrices translationMatrices;
        float translationZ = 1 - (tmpV[0].z()+1);
        QMatrix4x4 translateZ( 1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, translationZ,
                              0, 0, translationZ, 1);

        float translationX = -1 - tmpV[0].x();
        QMatrix4x4 translateX( 1, 0, 0, 0,
                              0, 1, 0, 0,
                              0, 0, 1, 0,
                              translationX, 0, 0, 1);
        translationMatrices.translateX = translateX;
        translationMatrices.translateZ = translateZ;
        transformVertices(tmpV, translationMatrices);

        std::vector<QVector3D> vertScreen = toScreen(tmpV, drawingModel->getViewSize());
        drawingModel->mapTexture(vertScreen,triangles,UVvertices,scaledTex2);
    }
}

void Transition::clearShutter()
{
    drawingModel->endOfFrame();
    vertices1.erase(vertices1.begin(), vertices1.end());
    triangles.erase(triangles.begin(), triangles.end());
    UVvertices.erase(UVvertices.begin(), UVvertices.end());
}

//____________________
//TUNNEL TRANSITION

void Transition::drawFrameTunnel(const int& frame)
{
    drawingModel->getScene()->clear();

    int BoxWidth, BoxHeight;
    QImage frameImage(scaledTex1.size(),QImage::Format_ARGB32);
    QPainter framePainter(&frameImage);
    framePainter.fillRect(0, 0, frameImage.width(), frameImage.height(), Qt::white);

    if (frame < 50)
    {
        BoxWidth = ( (double) (50. - frame) / 50. ) * scaledTex2.width();
        BoxHeight = ( (double) (50. - frame) / 50. ) * scaledTex2.height();

        QRect BoxRect( scaledTex1.width()/2 - BoxWidth/2, scaledTex1.height()/2 - BoxHeight/2, BoxWidth, BoxHeight);
        framePainter.drawImage(BoxRect, scaledTex1);
    }
    else
    {
        BoxWidth = ( ((double) frame - 50.) / 50. ) * scaledTex2.width();
        BoxHeight = ( ((double) frame - 50.) / 50. ) * scaledTex2.height();

        QRect BoxRect( scaledTex1.width()/2 - BoxWidth/2, scaledTex1.height()/2 - BoxHeight/2, BoxWidth, BoxHeight);
        framePainter.drawImage(BoxRect, scaledTex2);
    }

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );

}

//____________________
//CUBE TRANSITION

void Transition::initCube()
{
    vertices1.push_back(QVector4D(-1,1,-1,1)); //A
    vertices1.push_back(QVector4D(-1,-1,-1,1)); //B
    vertices1.push_back(QVector4D(1,-1,-1,1)); //C
    vertices1.push_back(QVector4D(1,1,-1,1)); //D

    vertices2.push_back(QVector4D(1,1,-1,1)); //A
    vertices2.push_back(QVector4D(1,-1,-1,1)); //B
    vertices2.push_back(QVector4D(1,-1,1,1)); //C
    vertices2.push_back(QVector4D(1,1,1,1)); //D

    triangles.push_back(QVector3D(2,1,0)); //CBA
    triangles.push_back(QVector3D(2,0,3)); //CAD


    UVvertices.push_back(QVector2D(0,0)); //A
    UVvertices.push_back(QVector2D(0,1)); //B
    UVvertices.push_back(QVector2D(1,1)); //C
    UVvertices.push_back(QVector2D(1,0)); //D

    drawingModel->endOfFrame();
}

void Transition::drawFrameCube(const int& frame)
{
    drawingModel->endOfFrame();
    float radians = (float) frame / 100. * M_PI / 2.;
    //float xOffset = (float) frame / 100.;

    QMatrix4x4 rotate( cos(-radians), 0, sin(-radians), 0,
                      0, 1, 0, 0,
                      -sin(-radians), 0, cos(-radians), 0,
                      0, 0, 0, 1);
    if ( frame < 50)
    {
        float translationZ = (float) frame / 50. * 2.;
        QMatrix4x4 translate( 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, translationZ+1,
                             0, 0, translationZ+1, 1);

        TransformationMatrices tMatrices;
        tMatrices.translateZ = translate;
        tMatrices.rotateY = rotate;

        std::vector<QVector4D> tmpV1 = vertices1;
        std::vector<QVector4D> tmpV2 = vertices2;

        transformVertices(tmpV1, tMatrices);
        transformVertices(tmpV2, tMatrices);


        std::vector<QVector3D> vertScreen1 = toScreen(tmpV1, drawingModel->getViewSize());
        std::vector<QVector3D> vertScreen2 = toScreen(tmpV2, drawingModel->getViewSize());

        drawingModel->mapTexture(vertScreen2,triangles,UVvertices,scaledTex2);
        drawingModel->mapTexture(vertScreen1,triangles,UVvertices,scaledTex1);
    }
    else
    {
        float invTranslationZ = 2. - ( ( (float) frame - 50. ) / 50. * 2. );
        QMatrix4x4 translate( 1, 0, 0, 0,
                             0, 1, 0, 0,
                             0, 0, 1, invTranslationZ+1,
                             0, 0, invTranslationZ+1, 1);

        TransformationMatrices tMatrices;
        tMatrices.translateZ = translate;
        tMatrices.rotateY = rotate;

        std::vector<QVector4D> tmpV1 = vertices1;
        std::vector<QVector4D> tmpV2 = vertices2;

        transformVertices(tmpV1, tMatrices);
        transformVertices(tmpV2, tMatrices);

        std::vector<QVector3D> vertScreen1 = toScreen(tmpV1, drawingModel->getViewSize());
        std::vector<QVector3D> vertScreen2 = toScreen(tmpV2, drawingModel->getViewSize());

         drawingModel->mapTexture(vertScreen1,triangles,UVvertices,scaledTex1);
        drawingModel->mapTexture(vertScreen2,triangles,UVvertices,scaledTex2);
    }
}

void Transition::clearCube()
{
    drawingModel->endOfFrame();
    vertices1.erase(vertices1.begin(), vertices1.end());
    vertices2.erase(vertices2.begin(), vertices2.end());
    triangles.erase(triangles.begin(), triangles.end());
    UVvertices.erase(UVvertices.begin(), UVvertices.end());
}

//____________________
//RING TRANSITION

void Transition::drawFrameRing(const int& frame)
{
    drawingModel->getScene()->clear();

    int Box1Width, Box1Height;
    int Box2Width, Box2Height;
    QImage frameImage(scaledTex1.size(),QImage::Format_ARGB32);
    QPainter framePainter(&frameImage);
    framePainter.fillRect(0, 0, frameImage.width(), frameImage.height(), Qt::white);

    float quarterWidth = scaledTex1.width()/4;
    float angle =  frame / 100. * M_PI;

    Box1Width = ( (double) (100. - frame ) / 100. ) * scaledTex1.width();
    Box1Height = ( (double) (100. - frame ) / 100. ) * scaledTex1.height();

    float offsetX1 = cos(3.*M_PI/2.+angle) * (quarterWidth+10);
    QRect Box1Rect( scaledTex1.width()/2 - Box1Width/2 + offsetX1, scaledTex1.height()/2 - Box1Height/2, Box1Width, Box1Height);

    Box2Width =  ((double) frame / 100. ) * scaledTex2.width();
    Box2Height =  ((double) frame / 100. ) * scaledTex2.height();

    float offsetX2 = cos(M_PI/2.+angle) * (quarterWidth+10);
    QRect Box2Rect( scaledTex2.width()/2 - Box2Width/2 + offsetX2, scaledTex2.height()/2 - Box2Height/2, Box2Width, Box2Height);

    if (frame < 50)
    {
        if (Box1Width >= 0)
            framePainter.drawImage(Box2Rect, scaledTex2);
        if (Box2Width >= 0)
            framePainter.drawImage(Box1Rect, scaledTex1);
    }
    else
    {
        if (Box1Width >= 0)
            framePainter.drawImage(Box1Rect, scaledTex1);
        if (Box2Width >= 0)
            framePainter.drawImage(Box2Rect, scaledTex2);
    }

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );

}

//____________________
//BLUR TRANSITION

void Transition::drawFrameBlur(const int& frame)
{
    drawingModel->getScene()->clear();
    QImage frameImage1 = scaledTex1;
    QImage frameImage2 = scaledTex2;

    float blurRadius;
    if (frame < 40)
    {
        blurRadius = (float) frame / 40. * 50.;
        QPainter painter(&frameImage1);
        QImage blurredImage = scaledTex1;
        qt_blurImage(&painter, blurredImage, blurRadius, false, false);
        drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage1) );

    }
    else if (frame >= 40 && frame < 60 )
    {
        QPainter painter1(&frameImage1);
        QPainter painter2(&frameImage2);
        QImage blurredImage1 = scaledTex1;
        QImage blurredImage2 = scaledTex2;
        qt_blurImage(&painter1, blurredImage1, 50., false, false);
        qt_blurImage(&painter2, blurredImage2, 50., false, false);

        float alphaProc = ( 20. - ( (float) frame - 40.) ) / 20. * 255.;

        for (int y = 0; y < frameImage1.height(); ++y) {
            QRgb *line = reinterpret_cast<QRgb*>(frameImage1.scanLine(y));
            for (int x = 0; x < frameImage1.width(); ++x) {
                QRgb &rgb = line[x];
                float newAlpha =  alphaProc;
                rgb = qRgba(qRed(rgb), qGreen(rgb), qBlue(rgb), newAlpha);
            }
        }
        drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage2) );
        drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage1) );

    }
    else
    {
        blurRadius = 50. - ( (float) frame - 60. ) / 40. * 50.;
        QPainter painter(&frameImage2);
        QImage blurredImage = scaledTex2;
        qt_blurImage(&painter, blurredImage, blurRadius, false, false);
        drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage2) );
    }

}

//____________________
//PIXEL BRIGHTNESS TRANSITION

void Transition::drawFramePixelBrightness(const int& frame)
{
    drawingModel->getScene()->clear();
    QImage frameImage = scaledTex1;

    float lumThreshold = ( 100 - (float) frame ) / 100. * 255.;

    for (int y = 0; y < frameImage.height(); ++y) {
        QRgb *line1 = reinterpret_cast<QRgb*>(frameImage.scanLine(y));
        QRgb *line2 = reinterpret_cast<QRgb*>(scaledTex2.scanLine(y));
        for (int x = 0; x < frameImage.width(); ++x) {
            QRgb &rgb1 = line1[x];
            QRgb &rgb2 = line2[x];
            float luminance1 =  0.2126*qRed(rgb1)+0.7152*qGreen(rgb1)+0.0722*qBlue(rgb1);
            if (luminance1 > lumThreshold)
                rgb1 = rgb2;
        }
    }

    drawingModel->getScene()->addPixmap( QPixmap::fromImage(frameImage) );
}
