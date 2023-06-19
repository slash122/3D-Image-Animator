#include "Transformations.h"

RotationMatrices::RotationMatrices()
{
    rotateX.setToIdentity();
    rotateY.setToIdentity();
    rotateZ.setToIdentity();
}

QMatrix4x4 RotationMatrices::getRotation() const
{
    return rotateX * rotateY * rotateZ;
}



void transformVertices(std::vector<QVector4D>& vertices,const RotationMatrices& rotMatrices)
{
    for (auto i = vertices.begin(); i != vertices.end(); i++)
    {
        QVector4D vertex(i->x(),i->y(),i->z(),i->w());
        vertex = vertex * rotMatrices.getRotation();
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

