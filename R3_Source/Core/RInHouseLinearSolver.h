#ifndef RInHouseLinearSolver_H
#define RInHouseLinearSolver_H

#include "RLinearSolver.h"


class RInHouseLinearSolver : public RLinearSolver
{
	Q_OBJECT

public:
    RInHouseLinearSolver(QObject *parent, QString name);
    ~RInHouseLinearSolver();

    int solveLinearSOE(QVector< QVector<double> > *A, QVector<double> *b, QVector<double> *x);

    int cholesky(QVector< QVector<double> > *inputMatrix, QVector< QVector<double> > *cholesky, QVector< QVector<double> > *inverseCholesky);

    int solveLinearSOE(QVector< QVector<double> > *A, QVector< QVector<double> > *B, QVector< QVector<double> > *X);



private:

    int ludcmp(QVector< QVector<double> > *a, QVector<int> *order);
    void solvlu(const QVector< QVector<double> > *a, const QVector<double> *b, QVector<double> *x, const QVector<int> *order);
    static int pivot(QVector< QVector<double> > *a, QVector<int> *order, int jcol);
    int lcm(int x,int y);
};

#endif
