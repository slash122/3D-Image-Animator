
#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "qmatrix4x4.h"
#include "qvectornd.h"

struct RotationMatrices
{
    RotationMatrices();

    QMatrix4x4 rotateX;
    QMatrix4x4 rotateY;
    QMatrix4x4 rotateZ;

    QMatrix4x4 getRotation() const;
};


void transformVertices(std::vector<QVector4D>& vertices,const RotationMatrices& rotMatrices);


std::vector<QVector3D> toScreen(std::vector<QVector4D>& vertices,const QSize& scrSize);

#endif // TRANSFORMATIONS_H
