#include "Transformations.h"

TransformationMatrices::TransformationMatrices()
{
    rotateX.setToIdentity();
    rotateY.setToIdentity();
    rotateZ.setToIdentity();

    translateX.setToIdentity();
    translateY.setToIdentity();
    translateZ.setToIdentity();
}

QMatrix4x4 TransformationMatrices::getRotation() const
{
    return rotateX * rotateY * rotateZ;
}

QMatrix4x4 TransformationMatrices::getTranslation() const
{
    return translateX * translateY * translateZ;
}

QMatrix4x4 TransformationMatrices::getTransformation() const
{
    return getRotation() * getTranslation();
}



void transformVertices(std::vector<QVector4D>& vertices,const TransformationMatrices& tMatrices)
{
    for (auto i = vertices.begin(); i != vertices.end(); i++)
    {
        QVector4D vertex(i->x(),i->y(),i->z(),i->w());
        vertex = vertex * tMatrices.getTransformation();
        i->setX(vertex.x()); i->setY(vertex.y());
        i->setZ(vertex.z()); i->setW(vertex.w());
    }
}

std::vector<QVector3D> toScreen(std::vector<QVector4D>& vertices,const QSize& scrSize)
{
    std::vector<QVector3D> screenVert;
    for (auto i = vertices.begin(); i != vertices.end(); i++)
    {
        QVector3D tmp;
        double invZ = 1. / (i->z() + 0.7);
        tmp.setX( i->x() * invZ * scrSize.width()/2 + scrSize.width()/2 );
        tmp.setY( - i->y() * invZ * scrSize.height()/2 + scrSize.height()/2 );
        tmp.setZ( i->z() + 0.7 );
        //Throw off w afterwards
        screenVert.push_back(tmp);
    }
    return screenVert;
}

