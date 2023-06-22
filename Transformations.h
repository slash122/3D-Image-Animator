
#ifndef TRANSFORMATIONS_H
#define TRANSFORMATIONS_H

#include "qmatrix4x4.h"
#include "qvectornd.h"

struct TransformationMatrices
{
    TransformationMatrices();

    QMatrix4x4 rotateX;
    QMatrix4x4 rotateY;
    QMatrix4x4 rotateZ;

    QMatrix4x4 translateX;
    QMatrix4x4 translateY;
    QMatrix4x4 translateZ;

    QMatrix4x4 getRotation() const;
    QMatrix4x4 getTranslation() const;
    QMatrix4x4 getTransformation() const;
};


void transformVertices(std::vector<QVector4D>& vertices,const TransformationMatrices& tMatrices);


std::vector<QVector3D> toScreen(std::vector<QVector4D>& vertices,const QSize& scrSize);

#endif // TRANSFORMATIONS_H
