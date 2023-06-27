
#ifndef TRANSITION_H
#define TRANSITION_H
#include "DrawingModel.h"
#include "Transformations.h"

enum TransitionTypes{
    Default = 0,
    SlideTop = 1,
    SlideBottom = 2,
    SlideLeft = 3,
    SlideRight = 4,
    ApproachingBox = 5,
    LeavingBox = 6,
    Darkening = 7,
    Alpha = 8,
    RotationY = 9,
    RotationX = 10,
    Shutter = 11,
    Tunnel = 12,
    Cube = 13,
    Ring = 14,
    Blur = 15,
    PixelBrightness = 16,
};
//by Glib Avrutin
class Transition
{
public:
    ~Transition() {}
    void drawFrame(const int& frame);
    void setModel(DrawingModel* model);
    void setFirstTexture(QImage* tex1);
    void setSecondTexture(QImage* tex2);
    void initTransition(TransitionTypes type);

private:
    TransitionTypes currentType;

    //Default
    void drawFrameDefault(const int& frame);

    //SlideTop
    void drawFrameSlideTop(const int& frame);

    //SlideBottom
    void drawFrameSlideBottom(const int& frame);
    void initSlideBottom();

    //SlideLeft
    void drawFrameSlideLeft(const int& frame);

    //SlideRight
    void drawFrameSlideRight(const int& frame);

    //ApproachingBox
    void drawFrameApproachingBox(const int& frame);

    //LeavingBox
    void drawFrameLeavingBox(const int& frame);

    //Darkening
    void drawFrameDarkening(const int& frame);

    //Alpha
    void drawFrameAlpha(const int& frame);

    //RotationY
    void drawFrameRotationY(const int& frame);
    void initRotationY();
    void clearRotationY();

    //RotationX
    void drawFrameRotationX(const int& frame);
    void initRotationX();
    void clearRotationX();

    //Shutter
    void drawFrameShutter(const int& frame);
    void initShutter();
    void clearShutter();

    //Tunnel
    void drawFrameTunnel(const int& frame);

    //Cube
    void drawFrameCube(const int& frame);
    void initCube();
    void clearCube();

    //Ring
    void drawFrameRing(const int& frame);

    //Blur
    void drawFrameBlur(const int& frame);

    //PixelBrightness
    void drawFramePixelBrightness(const int& frame);

    QImage* texture1;
    QImage* texture2;
    DrawingModel* drawingModel;

    std::vector<QVector4D> vertices1;
    std::vector<QVector4D> vertices2;
    std::vector<QVector3D> triangles;
    std::vector<QVector2D> UVvertices;

    QImage scaledTex1;
    QImage scaledTex2;
};

TransitionTypes getType(int index);
#endif // TRANSITION_H
