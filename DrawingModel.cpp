#include "DrawingModel.h"

//Texel

void Texel::set(const QVector2D& _pos,const QRgb& _color)
{
    pos = _pos;
    color = _color;
}



//DrawingModel

DrawingModel::DrawingModel(QGraphicsView* view) : grView(view)
{
    grScene = new QGraphicsScene();
    grView->setScene(grScene);
    grViewSize.setWidth(grView->width());
    grViewSize.setHeight(grView->height());
    renderingBuffer = new QImage( grViewSize.width(), grViewSize.height(), QImage::Format_RGB32);
    renderingPainter = new QPainter(renderingBuffer);
    renderingPainter->fillRect(0, 0, renderingBuffer->width(), renderingBuffer->height(), Qt::white );
}

void DrawingModel::drawLines(const std::vector<QVector2D>& vertices, const std::vector<QVector2D>& vertexIndeces, QColor color)
{
    for ( auto i : vertexIndeces)
    {
        QVector2D v1 = vertices[i.x()];
        QVector2D v2 = vertices[i.y()];
        renderingPainter->setPen(QPen(color,1));
        renderingPainter->drawLine(v1.x(),v1.y(),v2.x(),v2.y());
        grScene->addPixmap( QPixmap::fromImage(*renderingBuffer));
    }
}

void DrawingModel::mapTexture(const std::vector<QVector3D>& vertices, const std::vector<QVector3D>& triangles, const std::vector<QVector2D>& textureUVVertices, const QImage& texture)
{
    for ( auto triangle : triangles)
    {
        QVector3D v0 = vertices[triangle.x()];
        QVector3D v1 = vertices[triangle.y()];
        QVector3D v2 = vertices[triangle.z()];

        QVector2D uv0 = textureUVVertices[triangle.x()];
        QVector2D uv1 = textureUVVertices[triangle.y()];
        QVector2D uv2 = textureUVVertices[triangle.z()];

        float minX = std::min( {v0.x(), v1.x(), v2.x()} );
        float minY = std::min( {v0.y(), v1.y(), v2.y()} );
        float maxX = std::max( {v0.x(), v1.x(), v2.x()} );
        float maxY = std::max( {v0.y(), v1.y(), v2.y()} );

        for (int x = minX; x < maxX; x+=1)
        {
            for (int y = minY; y < maxY; y+=1)
            {
                if ( pointInTriangle(x, y, v0, v1, v2) )
                {
                    QVector2D uv = interpolateTextureCoordinates( x, y, v0, v1, v2, uv0, uv1, uv2);
                    Texel texel = sampleTexture(uv, texture);

                    renderingBuffer->setPixelColor( x, y, texel.color);
                }
            }

        }
    }
    grScene->addPixmap( QPixmap::fromImage(*renderingBuffer));
}

void DrawingModel::endOfFrame()
{
    grScene->clear();
    renderingPainter->fillRect(0, 0, renderingBuffer->width(), renderingBuffer->height(), Qt::white );
}

//____________________Private methods_________________________//

bool DrawingModel::pointInTriangle(float x, float y, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2)
{
    QVector2D noZ_v0( v0.x(), v0.y());
    QVector2D noZ_v1( v1.x(), v1.y());
    QVector2D noZ_v2( v2.x(), v2.y());

    //Barycentric calculation
    QVector2D bv0 = noZ_v2 - noZ_v0;
    QVector2D bv1 = noZ_v1 - noZ_v0;
    QVector2D bv2 = QVector2D(x,y) - noZ_v0;

    float dot00 = QVector2D::dotProduct(bv0, bv0);
    float dot01 = QVector2D::dotProduct(bv0, bv1);
    float dot02 = QVector2D::dotProduct(bv0, bv2);
    float dot11 = QVector2D::dotProduct(bv1, bv1);
    float dot12 = QVector2D::dotProduct(bv1, bv2);

    //Compute barycentric
    float invDenom = 1. / ( dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    return  (u >= 0) && (v >= 0) && (u+v <= 1);
}

QVector2D DrawingModel::interpolateTextureCoordinates(float x, float y, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, const QVector2D& uv0, const QVector2D& uv1, const QVector2D& uv2)
{
    float w0 = 1. / v0.z();
    float w1 = 1. / v1.z();
    float w2 = 1. / v2.z();

    float alpha = ( (v1.y() - v2.y()) * (x - v2.x()) + (v2.x() - v1.x()) * ( y - v2.y()) )
                  / ( (v1.y() - v2.y()) * (v0.x() - v2.x()) + (v2.x() - v1.x()) * (v0.y() - v2.y()) );
    float beta = ( (v2.y() - v0.y()) * (x - v2.x()) + (v0.x() - v2.x()) * ( y - v2.y()) )
                 / ( (v1.y() - v2.y()) * (v0.x() - v2.x()) + (v2.x() - v1.x()) * (v0.y() - v2.y()) );

    float gamma = (1 - alpha) - beta;

    float perspectiveCorrectionFactor = 1. / ( alpha * w0 + beta * w1 + gamma * w2);


    QVector2D uv = (alpha * uv0 * w0 + beta * uv1 * w1 + gamma * uv2 * w2) * perspectiveCorrectionFactor;
    return uv;
}

Texel DrawingModel::sampleTexture(const QVector2D& uv, const QImage& texture)
{
    int tx = uv.x() * (texture.width() - 1);
    int ty = uv.y() * (texture.height() - 1);

    Texel texel;
    texel.set( QVector2D(tx,ty), texture.pixel(tx, ty) );
    return texel;
}
