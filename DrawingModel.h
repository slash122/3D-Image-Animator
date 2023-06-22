
#ifndef DRAWINGMODEL_H
#define DRAWINGMODEL_H

#include "qgraphicsview.h"
#include "QImage"
#include "qvectornd.h"

struct Texel
{
    Texel() : pos(0,0) {}
    QVector2D pos;
    QRgb color;
    void set(const QVector2D& _pos,const QRgb& _color);
};

class DrawingModel
{
public:
    DrawingModel(QGraphicsView* view);
    void drawLines(const std::vector<QVector2D>& vertices, const std::vector<QVector2D>& vertexIndeces, QColor color = Qt::black);
    void mapTexture(const std::vector<QVector3D>& vertices, const std::vector<QVector3D>& triangles, const std::vector<QVector2D>& textureUVVertices, const QImage& texture);
    void endOfFrame();
    ~DrawingModel();

private:
    bool pointInTriangle(float x, float y, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2);
    QVector2D interpolateTextureCoordinates(float x, float y, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, const QVector2D& uv0, const QVector2D& uv1, const QVector2D& uv2);
    Texel sampleTexture(const QVector2D& uv, const QImage& texture);
    bool inRenderSpace(float x, float y);

    QGraphicsView* grView;
    QGraphicsScene* grScene;
    QSize grViewSize;
    QImage* renderingBuffer;
    QPainter* renderingPainter;
};

#endif // DRAWINGMODEL_H
