#ifndef RMatrixOperations_H
#define RMatrixOperations_H

#include "RMethod.h"
#include "RDomain.h"



#ifdef  OSG

#include <osg/Vec3>
#include <osg/Array>

#endif 



class RMatrixOperations : public RMethod
{

public:
    RMatrixOperations(RDomain *theDomain);
    ~RMatrixOperations();

    double max(QVector<double> const *vector);
    double maxAbs(QVector<double> const *vector);
    double min(QVector<double> const *vector);

    double norm(QVector<double> const *vector);
    double normSquared(QVector<double> const *vector);

    double dotProduct(QVector<double> const *vector1, QVector<double> const *vector2);
    int crossProduct(QVector<double> const *vector1, QVector<double> const *vector2, QVector<double> *result);
    int outerProduct(QVector<double> const *a, QVector<double> const *b, QVector< QVector<double> > *result);

    int multiplyMatrixVector(bool transpose, double alpha, QVector< QVector<double> > const *matrix, QVector<double> const *vector, double  beta, QVector<double> *result);
    int multiplyMatrixMatrix(bool transpose1, bool transpose2, double alpha, QVector< QVector<double> > const *matrix1, QVector< QVector<double> > const *matrix2, double beta, QVector< QVector<double> > *result);

    QVector< QVector<double> > multiplyMatrixMatrixReturnMatrix(bool transpose1, bool transpose2, double alpha, QVector< QVector<double> > const *matrix1, QVector< QVector<double> > const *matrix2, double beta); //SG add

    int scaleMatrix(double factor, QVector< QVector<double> > const *m, QVector< QVector<double> > *result);
    int scaleMatrix(double factor, QVector< QVector<double> > *result);
    QVector< QVector<double> > scaleMatrixReturnMatrix(double factor,const QVector< QVector<double> > *matrix);
    int scaleVector(double factor, QVector<double> const *v, QVector<double> *result);

    int addVectors(QVector<double> const *a, QVector<double> const *b, QVector<double> *result);
    int addVectortoVector(QVector<double> const *a, QVector<double>  *b);
    int subtractVectors(QVector<double> const *a, QVector<double> const *b, QVector<double> *result);

    int addMatrices(QVector< QVector<double> > const *a, QVector< QVector<double> > const *b, QVector< QVector<double> > *result);
    int addMatrixToMatrix(QVector< QVector<double> > const *a, QVector< QVector<double> > *result);
    int subtractMatrices(QVector< QVector<double> > const *a, QVector< QVector<double> > const *b, QVector< QVector<double> > *result);

    int maxMatrix(QVector< QVector<double> > const *m,  double *result); //SG add
    int minMatrix(QVector< QVector<double> > const *m,  double *result); //SG add

    double traceMatrix(QVector< QVector<double> > const *m); //SG add



#ifdef  OSG

    void convexHull(QList<osg::Vec3>& thePointsList);

    osg::Vec3 computePolygonNormal( const osg::Vec3Array* polygon );

    QList<osg::Vec2> get2DProjection(QList<osg::Vec3> thePointsList);

    osg::Vec3 computePolygonCentroid( const std::vector<osg::Vec3>& polygon );

    bool checkOpenPolygonConvexity( const std::vector<osg::Vec2>& polygon );

#endif 


private:

    int  compare(const void *vp1, const void *vp2);



};

#endif
