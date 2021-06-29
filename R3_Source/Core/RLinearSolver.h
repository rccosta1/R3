#ifndef RLinearSolver_H
#define RLinearSolver_H

#include "RMethod.h"

class RDomain;
class RRandomVariable;

class RLinearSolver : public RMethod
{
	Q_OBJECT

public:

    RLinearSolver(QObject *parent, QString name);
    virtual ~RLinearSolver();

    virtual int solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x) = 0;
    virtual int solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X) =0;


protected:

    int forwardSubstitution(QVector< QVector<double> > *L, QVector<double> *b, QVector<double> *y);
    int backwardSubstitution(bool lowerTriangular, QVector< QVector<double> > *U, QVector<double> *y, QVector<double> *x);
};

#endif
