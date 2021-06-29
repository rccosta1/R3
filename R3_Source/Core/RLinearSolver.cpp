#include "RLinearSolver.h"
#include "RMethod.h"

RLinearSolver::RLinearSolver(QObject *parent, QString name)
    : RMethod(parent, name)
{
}


RLinearSolver::~RLinearSolver()
{
}


int RLinearSolver::forwardSubstitution(QVector< QVector<double> > *L, QVector<double> *b, QVector<double> *y)
{
    // Determine system size
    int n = L->size();


    // Rough check that L is actually lower-triangular
    if ((*L)[0][n-1]) {
        qCritical() << this->objectName() << "is supposed to get a lower-triangular matrix";
    }


    // Forward substitution algorithm
    double sum;
    for (int i=0; i<n; i++) {

        sum = 0.0;
        for (int j=0; j<i; j++) {

            sum += (*L)[i][j] * (*y)[j];
        }

        if ((*L)[i][i] != 0) {

            (*y)[i] = ((*b)[i] - sum) / (*L)[i][i];
        }
        else {
            qCritical() << "Error in" << this->objectName();
        }
    }

    return 0;
}

int RLinearSolver::backwardSubstitution(bool lowerTriangular, QVector< QVector<double> > *U, QVector<double> *y, QVector<double> *x)
{
    // Determine system size
    int n = U->size();


    // Rough check that U is actually upper-triangular
    if ((*U)[n-1][0]) {
        qCritical() << this->objectName() << "is supposed to get an upper-triangular matrix";
    }


    // Backward substitution algorithm
    double sum;
    for (int i=n-1; i>=0; i--) {

        sum = 0.0;
        for (int j=i+1; j<n; j++) {

            if (lowerTriangular) {
                sum += (*U)[j][i] * (*x)[j];
            }
            else {
                sum += (*U)[i][j] * (*x)[j];
            }
        }

        if ((*U)[i][i] != 0) {

            (*x)[i] = ((*y)[i] - sum) / (*U)[i][i];
        }
        else {
            qCritical() << "Error in" << this->objectName();
        }
    }

    return 0;
}


