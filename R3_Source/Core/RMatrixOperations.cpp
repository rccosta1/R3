#include "RMatrixOperations.h"
#include "RDomain.h"

#include "QList"
#include <math.h>
#include <assert.h>
#include <stack>
#include <functional>
#include <stdlib.h>


#ifdef  OSG

#include <osg/Vec3>
#include <osg/Array>

#endif




RMatrixOperations::RMatrixOperations(RDomain *theDomain)
    : RMethod(theDomain, 0)
{
}

RMatrixOperations::~RMatrixOperations()
{

}


double RMatrixOperations::max(QVector<double> const *vector)
{
    // Check for null pointers
    if (vector == 0) {
        qCritical() << "A null pointer came into the max-function in the matrix operations.";
        return -1;
    }


    // Declare and initilize the max-value to be returned from this method
    double theMax = vector->at(0);

    // Loop over all components of the vector
    for (int i=1; i<vector->size(); i++) {

        if (vector->at(i) > theMax)

            theMax = vector->at(i);
    }

    return theMax;
}




double RMatrixOperations::maxAbs(QVector<double> const *vector)
{
    // Check for null pointers
    if (vector == 0) {
        qCritical() << "A null pointer came into the maxAbs-function in the matrix operations.";
        return -1;
    }



    // Declare and initilize the max-value to be returned from this method
    double theMax = qAbs(vector->at(0));

    // Loop over all components of the vector
    for (int i=1; i<vector->size(); i++) {

        if (qAbs(vector->at(i)) > theMax)

            theMax = qAbs(vector->at(i));
    }

    return theMax;
}




double RMatrixOperations::min(QVector<double> const *vector)
{
    // Check for null pointers
    if (vector == 0) {
        qCritical() << "A null pointer came into the min-function in the matrix operations.";
        return -1;
    }



    // Declare and initilize the max-value to be returned from this method
    double theMin = vector->at(0);

    // Loop over all components of the vector
    for (int i=1; i<vector->size(); i++) {

        if (vector->at(i) < theMin)

            theMin = vector->at(i);
    }

    return theMin;
}




double RMatrixOperations::norm(QVector<double> const *vector)
{
    // Check for null pointers
    if (vector == 0) {
        qCritical() << "A null pointer came into the norm-function in the matrix operations.";
        return -1;
    }



    // Declare and initilize the norm to be returned from this method
    double theNorm = 0.0;

    // Loop over all components of the vector to compute the sum of squares
    for (int i=0; i<vector->size(); i++) {
        theNorm += vector->at(i)*vector->at(i);
    }

    // Take the square root of the sum of squares, namely the norm
    return sqrt(theNorm);

}

double RMatrixOperations::normSquared(QVector<double> const *vector)
{
    // Check for null pointers
    if (vector == 0) {
        qCritical() << "A null pointer came into the norm-function in the matrix operations.";
        return -1;
    }


    // Declare and initilize the result to be returned from this method
    double theNormSquared = 0.0;

    // Loop over all components of the vector to compute the sum of squares
    for (int i=0; i<vector->size(); i++) {

        theNormSquared += vector->at(i)*vector->at(i);
    }


    // Take the square root of the sum of squares, namely the norm
    return theNormSquared;

}

double RMatrixOperations::dotProduct(QVector<double> const *vector1, QVector<double> const *vector2)
{
    // Check for null pointers
    if (vector1 == 0 || vector2 == 0) {
        qCritical() << "A null pointer came into the dotProduct-function in the matrix operations.";
        return -1;
    }



    // Check for dimensional consistency...
    if (vector1->size()!=vector2->size())
    {
        qCritical()<<"Error: The vector dimensions do not match for dot product.";
        return 0.0;
    } else {
        // Evaluate dot product
        double theDotProduct = 0.0;
        for (int i=0; i<vector1->size(); i++) {
            theDotProduct += (*vector1)[i]*(*vector2)[i];
        }

        return theDotProduct;
    }
}



int RMatrixOperations::crossProduct(QVector<double> const *vector1, QVector<double> const *vector2, QVector<double> *result)
{
    // Check for null pointers
    if (vector1 == 0 || vector2 == 0 || result == 0) {
        qCritical() << "A null pointer came into the crossProduct-function in the matrix operations.";
        return -1;
    }



    // Check for dimensional consistency...
    if (vector1->size()!=3 || vector2->size()!= 3 || result->size()!=3)
    {
        qCritical()<<"Error: The vectors in a cross-product must all the 3-dimensional.";
        return -1;

    } else {

        (*result)[0] = (*vector1)[1] * (*vector2)[2] - (*vector1)[2] * (*vector2)[1];
        (*result)[1] = (*vector1)[2] * (*vector2)[0] - (*vector1)[0] * (*vector2)[2];
        (*result)[2] = (*vector1)[0] * (*vector2)[1] - (*vector1)[1] * (*vector2)[0];

   }

    return 0;
}



int RMatrixOperations::multiplyMatrixVector(bool transpose, double alpha, QVector< QVector<double> > const *matrix, QVector<double> const *vector, double  beta, QVector<double> *result)
{
    // Check for null pointers
    if (matrix == 0 || vector == 0 || result == 0) {
        qCritical() << "A null pointer came into the multiplyMatrixVector-function in the matrix operations.";
        return -1;
    }


    // This method is equivalent to the function "gsl_blas_dgemv" in GNU Scientific Library:

    // Figure out the matrix and vector sizes
    int rowsMatrix = matrix->size();
    int colsMatrix = matrix[0][0].size();
    int sizeVector = vector->size();
    int sizeResult = result->size();



    // Check for dimensional consistency
    int rowsmat, colsmat;
    if (transpose) {colsmat = rowsMatrix; rowsmat = colsMatrix;} else {colsmat = colsMatrix; rowsmat = rowsMatrix;}
    if (colsmat != sizeVector || rowsmat != sizeResult) {
        qCritical() << "Failed to multiply a matrix and a vector due to dimensional inconsistency";

        return -1;
    }

    auto matrixtranspose = make_unique<QVector< QVector<double> >>(colsMatrix,QVector<double>(rowsMatrix));

    auto resultcopy = make_unique<QVector< double>> (sizeResult);
    auto vectorcopy = make_unique<QVector< double>> (sizeVector);

    for(int i=0; i < sizeResult; i++)
    {
        (*resultcopy)[i]=result->at(i);
    }

    for(int i=0; i < sizeVector; i++)
    {
        (*vectorcopy)[i]=vector->at(i);
    }

    double sum = 0.0;

    if (transpose)
    { for (int i=0; i<rowsMatrix; i++){
            for (int j=0; j<colsMatrix; j++){
                (*matrixtranspose)[j][i]=(*matrix)[i][j];
            }
        }
        for (int i = 0; i < rowsmat; i++)
        {    sum=0.0;
            for (int j = 0; j < colsmat; j++){
                sum += alpha*(*matrixtranspose)[i][j]*vectorcopy->at(j);
            }
            sum += beta*resultcopy->at(i);
            (*result)[i]=sum;
        }

    } else {
        for (int i = 0; i < rowsMatrix; i++){

            for (int j = 0; j < colsMatrix; j++){

                sum += alpha*(*matrix)[i][j]*(*vectorcopy)[j];

            }
            sum += beta*(*resultcopy)[i];
            (*result)[i] = sum;
            sum=0.0;
        }
    }

    return 0;
}


int RMatrixOperations::multiplyMatrixMatrix(bool transpose1, bool transpose2, double alpha, QVector< QVector<double> > const *matrix1, QVector< QVector<double> > const *matrix2, double beta, QVector< QVector<double> > *result)
{
    // Check for null pointers
    if (matrix1 == 0 || matrix2 == 0 || result == 0) {
        qCritical() << "A null pointer came into the multiplyMatrixMatrix-function in the matrix operations.";
        return -1;
    }

    // This method is equivalent to the function "gsl_blas_dgemm" in GNU Scientific Library:

    // Figure out the matrix sizes
    int rowsMatrix1 = matrix1->size();
    int colsMatrix1 = matrix1[0][0].size();

    int rowsMatrix2 = matrix2->size();
    int colsMatrix2 = matrix2[0][0].size();

    int rowsResult = result->size();
    int colsResult = result[0][0].size();


    // Check for dimensional consistency
    int rows1, cols1, rows2, cols2;
    if (transpose1) {cols1 = rowsMatrix1; rows1 = colsMatrix1;} else {cols1 = colsMatrix1; rows1 = rowsMatrix1;}
    if (transpose2) {cols2 = rowsMatrix2; rows2 = colsMatrix2;} else {cols2 = colsMatrix2; rows2 = rowsMatrix2;}
    if (cols1 != rows2 || rows1 != rowsResult || cols2 != colsResult) {
        qCritical() << "Failed to multiply two matrices due to dimensional inconsistency";

        return -1;
    }


    auto copymatrix1      =make_unique<QVector< QVector<double> >>(rowsMatrix1,QVector<double>(colsMatrix1));
    auto copymatrix2      =make_unique<QVector< QVector<double> >>(rowsMatrix2,QVector<double>(colsMatrix2));
    auto matrixtranspose1 =make_unique<QVector< QVector<double> >>(colsMatrix1,QVector<double>(rowsMatrix1));
    auto matrixtranspose2 =make_unique<QVector< QVector<double> >>(colsMatrix2,QVector<double>(rowsMatrix2));
    auto resultcopy       =make_unique<QVector< QVector<double> >> (rowsResult,QVector<double>(colsResult));

    for(int i=0; i<rowsMatrix1; ++i)
    {for (int j=0; j<colsMatrix1; ++j)
            (*copymatrix1)[i][j]=(*matrix1)[i][j];
    }
    for(int i=0; i<rowsMatrix2; ++i)
    {for (int j=0; j<colsMatrix2; ++j)
            (*copymatrix2)[i][j]=(*matrix2)[i][j];
    }
    for(int i=0; i<rowsResult; ++i)
    {for (int j=0; j<colsResult; ++j)
            (*resultcopy)[i][j]=(*result)[i][j];
    }


    for (int i=0; i<rowsMatrix1; ++i){
        for (int j=0; j<colsMatrix1; ++j){
            (*matrixtranspose1)[j][i]=(*matrix1)[i][j];
        }
    }

    for (int i=0; i<rowsMatrix2; ++i){
        for(int j=0; j<colsMatrix2; ++j){
            (*matrixtranspose2)[j][i]=(*matrix2)[i][j];
        }
    }


    double sum=0;

    auto sizeMatTrans2 = matrixtranspose2.get()[0][0].size();
    auto sizeMatTrans1 = matrixtranspose1.get()[0][0].size();
    auto sizeCopyMat1 = copymatrix1.get()[0][0].size();
    auto sizeCopyMat2 = copymatrix2.get()[0][0].size();

    if (transpose1 && transpose2)
    {
        for (int i = 0; i < matrixtranspose1->size(); ++i)
        {   for (int j = 0; j < sizeMatTrans2; ++j)
            { sum=0.0;
                for (int k = 0; k < sizeMatTrans1; k++){
                    sum += alpha*(*matrixtranspose1)[i][k]*(*matrixtranspose2)[k][j];
                }
                sum += beta*(*resultcopy)[i][j];
                (*result)[i][j]=sum;

            }
        }

    } else if (transpose1 && !transpose2)

    {
        sum=0.0;
        for (int i = 0; i < matrixtranspose1->size(); ++i)
        {   for (int j = 0; j < sizeCopyMat2; ++j)
            { sum=0.0;
                for (int k = 0; k < sizeMatTrans1; k++){
                    sum += alpha*(*matrixtranspose1)[i][k]*(*copymatrix2)[k][j];
                }
                sum += beta*(*resultcopy)[i][j];
                (*result)[i][j]=sum;

            }
        }

    } else if (!transpose1 && transpose2)
    {
        sum=0.0;
        for (int i = 0; i < copymatrix1->size(); ++i)
        {   for (int j = 0; j < sizeMatTrans2; ++j)
            { sum=0.0;
                for (int k = 0; k < sizeCopyMat1; k++){
                    sum += alpha*(*copymatrix1)[i][k]*(*matrixtranspose2)[k][j];
                }
                sum += beta*(*resultcopy)[i][j];
                (*result)[i][j]=sum;

            }
        }
    }
    else if (!transpose1 && !transpose2)
    {
        sum=0.0;
        for (int i = 0; i < copymatrix1->size(); ++i)
        {   for (int j = 0; j < sizeCopyMat2; ++j)
            { sum=0.0;
                for (int k = 0; k < sizeCopyMat1; k++){

                    sum += alpha*(*copymatrix1)[i][k]*(*copymatrix2)[k][j];
                }
                sum += beta*(*resultcopy)[i][j];
                (*result)[i][j]=sum;

            }
        }

    }

    return 0;

}


 QVector< QVector<double> > RMatrixOperations::multiplyMatrixMatrixReturnMatrix(bool transpose1, bool transpose2, double alpha, QVector< QVector<double> > const *matrix1, QVector< QVector<double> > const *matrix2, double beta)  //SG add
{
    // Check for null pointers
    if (matrix1 == 0 || matrix2 == 0)
    {
        qCritical() << "A null pointer came into the multiplyMatrixMatrix-function in the matrix operations.";
    }

    // This method is equivalent to the function "gsl_blas_dgemm" in GNU Scientific Library:

    // Figure out the matrix sizes
    int rowsMatrix1 = matrix1->size();
    int colsMatrix1 = matrix1[0][0].size();

    int rowsMatrix2 = matrix2->size();
    int colsMatrix2 = matrix2[0][0].size();

    unique_ptr<QVector< QVector<double> >> result = nullptr;

    if (transpose1) { result                    = make_unique<QVector< QVector<double> >>(colsMatrix1,QVector<double>(colsMatrix2));}
    else if (transpose2) { result               = make_unique<QVector< QVector<double> >>(rowsMatrix1,QVector<double>(rowsMatrix2));}
    else if (transpose1 && transpose2) {result  = make_unique<QVector< QVector<double> >>(colsMatrix1,QVector<double>(rowsMatrix2));}
    else
    {
        result = make_unique< QVector< QVector<double> >>(rowsMatrix1,QVector<double>(colsMatrix2));
    }

    int rowsResult = result->size();
    int colsResult = result.get()[0][0].size();

    // Check for dimensional consistency
    int rows1, cols1, rows2, cols2;
    if (transpose1) {cols1 = rowsMatrix1; rows1 = colsMatrix1;} else {cols1 = colsMatrix1; rows1 = rowsMatrix1;}
    if (transpose2) {cols2 = rowsMatrix2; rows2 = colsMatrix2;} else {cols2 = colsMatrix2; rows2 = rowsMatrix2;}
    if (cols1 != rows2 || rows1 != rowsResult || cols2 != colsResult) {
        qCritical() << "Failed to multiply two matrices due to dimensional inconsistency";

        return *result;
    }


    auto copymatrix1        = make_unique<QVector< QVector<double> >>(rowsMatrix1,QVector<double>(colsMatrix1));
    auto copymatrix2        = make_unique<QVector< QVector<double> >>(rowsMatrix2,QVector<double>(colsMatrix2));
    auto matrixtranspose1   = make_unique<QVector< QVector<double> >>(colsMatrix1,QVector<double>(rowsMatrix1));
    auto matrixtranspose2   = make_unique<QVector< QVector<double> >>(colsMatrix2,QVector<double>(rowsMatrix2));
    auto resultcopy         = make_unique<QVector< QVector<double> >> (rowsResult,QVector<double>(colsResult));

    for(int i=0; i<rowsMatrix1; ++i)
    {for (int j=0; j<colsMatrix1; ++j)
            (*copymatrix1)[i][j]=(*matrix1)[i][j];
    }
    for(int i=0; i<rowsMatrix2; ++i)
    {for (int j=0; j<colsMatrix2; ++j)
            (*copymatrix2)[i][j]=(*matrix2)[i][j];
    }
    for(int i=0; i<rowsResult; ++i)
    {for (int j=0; j<colsResult; ++j)
            (*resultcopy)[i][j]=(*result)[i][j];
    }


    for (int i=0; i<rowsMatrix1; ++i){
        for (int j=0; j<colsMatrix1; ++j){
            (*matrixtranspose1)[j][i]=(*matrix1)[i][j];
        }
    }

    for (int i=0; i<rowsMatrix2; ++i){
        for(int j=0; j<colsMatrix2; ++j){
            (*matrixtranspose2)[j][i]=(*matrix2)[i][j];
        }
    }


    double sum = 0.0;

    auto sizematTrans2 = matrixtranspose2.get()[0][0].size();
    auto sizematTrans1 = matrixtranspose1.get()[0][0].size();
    auto sizecopymatrix1 = copymatrix1.get()[0][0].size();
    auto sizecopymatrix2 = copymatrix2.get()[0][0].size();

    if (transpose1 && transpose2)
    {
        for (int i = 0; i < matrixtranspose1->size(); ++i)
        {   for (int j = 0; j < sizematTrans2; ++j)
            { sum=0.0;
                for (int k = 0; k < sizematTrans1; k++){
                    sum += alpha*(*matrixtranspose1)[i][k]*(*matrixtranspose2)[k][j];
                }
                sum += beta*(*resultcopy)[i][j];
                (*result)[i][j]=sum;

            }
        }

    } else if (transpose1 && !transpose2)

    {
        sum=0.0;
        for (int i = 0; i < matrixtranspose1->size(); ++i)
        {   for (int j = 0; j < sizecopymatrix2; ++j)
            { sum=0.0;
                for (int k = 0; k < sizematTrans1; k++){
                    sum += alpha*(*matrixtranspose1)[i][k]*(*copymatrix2)[k][j];
                }
                sum += beta*(*resultcopy)[i][j];
                (*result)[i][j]=sum;

            }
        }

    } else if (!transpose1 && transpose2)
    {
        sum=0.0;
        for (int i = 0; i < copymatrix1->size(); ++i)
        {   for (int j = 0; j < sizematTrans2; ++j)
            { sum=0.0;
                for (int k = 0; k < sizecopymatrix1; k++){
                    sum += alpha*(*copymatrix1)[i][k]*(*matrixtranspose2)[k][j];
                }
                sum += beta*(*resultcopy)[i][j];
                (*result)[i][j]=sum;

            }
        }
    }
    else if (!transpose1 && !transpose2)
    {
        sum=0.0;
        for (int i = 0; i < copymatrix1->size(); ++i)
        {   for (int j = 0; j < sizecopymatrix2; ++j)
            { sum=0.0;
                for (int k = 0; k < sizecopymatrix1; k++){

                    sum += alpha*(*copymatrix1)[i][k]*(*copymatrix2)[k][j];
                }
                sum += beta*(*resultcopy)[i][j];
                (*result)[i][j]=sum;

            }
        }

    }

    return (*result);

}


int RMatrixOperations::addMatrices(QVector< QVector<double> > const *a, QVector< QVector<double> > const *b, QVector< QVector<double> > *result)
{
    // Check for null pointers
    if (a == 0 || b == 0 || result == 0) {
        qCritical() << "A null pointer came into the addMatrices-function in the matrix operations.";
        return -1;
    }




    // Get sizes
    int rowsa = a->size();
    int colsa = a[0][0].size();

    int rowsb = b->size();
    int colsb = b[0][0].size();

    int rowsresult = result->size();
    int colsresult = result[0][0].size();


    // Check for dimensional consistency
    if (rowsa != rowsb || rowsb != rowsresult || colsa != colsb || colsb != colsresult) {
        qCritical() << "Failed to add matrices due to dimensional inconsistency";
        return -1;
    }


    // result = a + b
    for (int i=0; i<rowsa; i++) {
        for (int j=0; j<colsa; j++) {
            (*result)[i][j] = (*a)[i][j] + (*b)[i][j];
        }
    }

    return 0;
}




int RMatrixOperations::addMatrixToMatrix(QVector< QVector<double> > const *a, QVector< QVector<double> > *result)
{
    // Check for null pointers
    if (a == 0 || result == 0) {
        qCritical() << "A null pointer came into the add Matrices-function in the matrix operations.";
        return -1;
    }



    // Get sizes
    int rowsa = a->size();
    int colsa = a[0][0].size();


    int rowsresult = result->size();
    int colsresult = result[0][0].size();


    // Check for dimensional consistency
    if (rowsa != rowsresult || colsa != colsresult) {
        qCritical() << "Failed to add matrices due to dimensional inconsistency";
        return -1;
    }


    // result = a + b
    for (int i=0; i<rowsa; i++) {
        for (int j=0; j<colsa; j++) {
            (*result)[i][j] += (*a)[i][j];
        }
    }

    return 0;
}




int RMatrixOperations::subtractMatrices(QVector< QVector<double> > const *a, QVector< QVector<double> > const *b, QVector< QVector<double> > *result)
{
    // Check for null pointers
    if (a == 0 || b == 0 || result == 0) {
        qCritical() << "A null pointer came into the subtractMatrices-function in the matrix operations.";
        return -1;
    }

    // Get sizes
    int rowsa = a->size();
    int colsa = a[0][0].size();

    int rowsb = b->size();
    int colsb = b[0][0].size();

    int rowsresult = result->size();
    int colsresult = result[0][0].size();


    // Check for dimensional consistency
    if (rowsa != rowsb || rowsb != rowsresult || colsa != colsb || colsb != colsresult) {
        qCritical() << "Failed to subtract matrices due to dimensional inconsistency";
        return -1;
    }


    // result = a - b
    for (int i=0; i<rowsa; i++) {
        for (int j=0; j<colsa; j++) {
            (*result)[i][j] = (*a)[i][j] - (*b)[i][j];
        }
    }

    return 0;
}


int RMatrixOperations::addVectors(QVector<double> const *a, QVector<double> const *b, QVector<double> *result)
{
    // Check for null pointers
    if (a == 0 || b == 0 || result == 0) {
        qCritical() << "A null pointer came into the addVectors-function in the matrix operations.";
        return -1;
    }




    // Get vector sizes
    int asize = a->size();
    int bsize = b->size();
    int resultsize = result->size();


    // Check for dimensional consistency
    if (asize != bsize || bsize != resultsize) {
        qCritical() << "Failed to subtract vectors due to dimensional inconsistency";
        return -1;
    }


    // result = a + b
    for (int i=0; i<a->size(); i++) {
        (*result)[i] = a->at(i)+b->at(i);
    }

    return 0;
}



int RMatrixOperations::addVectortoVector(QVector<double> const *a, QVector<double>  *b)
{
    // Check for null pointers
    if (a == 0 || b == 0) {
        qCritical() << "A null pointer came into the addVectors-function in the matrix operations.";
        return -1;
    }




    // Get vector sizes
    int asize = a->size();
    int bsize = b->size();


    // Check for dimensional consistency
    if (asize != bsize) {
        qCritical() << "Failed to subtract vectors due to dimensional inconsistency";
        return -1;
    }


    // b = a + b
    for (int i=0; i<a->size(); i++)
    {
        (*b)[i] += a->at(i);
    }

    return 0;
}


int RMatrixOperations::subtractVectors(QVector<double> const *a, QVector<double> const *b, QVector<double> *result)
{
    // Check for null pointers
    if (a == 0 || b == 0 || result == 0) {
        qCritical() << "A null pointer came into the subtractVectors-function in the matrix operations.";
        return -1;
    }





    // Get vector sizes
    int asize = a->size();
    int bsize = b->size();
    int resultsize = result->size();


    // Check for dimensional consistency
    if (asize != bsize || bsize != resultsize) {
        qCritical() << "Failed to subtract vectors due to dimensional inconsistency";
        return -1;
    }


    // result = a - b
    for (int i=0; i<a->size(); i++) {
        (*result)[i] = a->at(i)-b->at(i);
    }

    return 0;
}


int RMatrixOperations::outerProduct(QVector<double> const *a, QVector<double> const *b, QVector< QVector<double> > *result)
{
    // Check for null pointers
    if (a == 0 || b == 0 || result == 0) {
        qCritical() << "A null pointer came into the outerProduct-function in the matrix operations.";
        return -1;
    }



    // Get vector and matrix sizes
    int asize = a->size();
    int bsize = b->size();
    int rowsMatrix = result->size();
    int colsMatrix = result[0][0].size();


    // Check for dimensional consistency
    if (asize != rowsMatrix || bsize != colsMatrix) {
        qCritical() << "Failed to carry out vector outer product due to dimensional inconsistency";
        return -1;
    }

    // m = a * b^T
    for (int i=0; i<rowsMatrix; i++) {
        for (int j=0; j<colsMatrix; j++) {
            (*result)[i][j] = a->at(i) * b->at(j);
        }
    }

    return 0;
}

int RMatrixOperations::scaleMatrix(double factor, QVector< QVector<double> > const *m, QVector< QVector<double> > *result)
{
    // Get sizes
    int rowsm = m->size();
    int colsm = m[0][0].size();

    int rowsresult = result->size();
    int colsresult = result[0][0].size();


    // Check for dimensional consistency
    if (rowsm != rowsresult || colsm != colsresult) {
        qCritical() << "Failed to scale matrix due to dimensional inconsistency";
        return -1;
    }

    for (int i=0; i<rowsm; i++) {
        for (int j=0; j<colsm; j++) {
            (*result)[i][j] = factor * (*m)[i][j];
        }
    }

    return 0;
}

QVector< QVector<double> > RMatrixOperations::scaleMatrixReturnMatrix(double factor,const QVector< QVector<double> > *matrix)
{

    int rowsm = matrix->size();
    int colsm = matrix[0][0].size();


    QVector< QVector<double> > result(rowsm, QVector<double>(colsm));

    for (int i=0; i<rowsm; i++) {
        for (int j=0; j<colsm; j++) {
            result[i][j] = factor * (*matrix)[i][j];
        }
    }

    return result;
}

int RMatrixOperations::scaleMatrix(double factor, QVector< QVector<double> > *result)
{
    // Get size
    int rowsm = result->size();
    int colsm = result[0][0].size();


    for (int i=0; i<rowsm; ++i) {
        for (int j=0; j<colsm; ++j) {
            (*result)[i][j] = factor * (*result)[i][j];
        }
    }

    return 0;
}



int RMatrixOperations::scaleVector(double factor, QVector<double> const *v, QVector<double> *result)
{
    // Check for null pointers
    if (v == 0 || result == 0) {
        qCritical() << "A null pointer came into the scaleVector-function in the matrix operations.";
        return -1;
    }



    // Get sizes
    int vSize = v->size();
    int resultSize = result->size();


    // Check for dimensional consistency
    if (vSize != resultSize) {
        qCritical() << "Failed to scale vector due to dimensional inconsistency";
        return -1;
    }

    for (int i=0; i<vSize; i++) {
            (*result)[i] = factor * (*v)[i];
    }

    return 0;
}


int RMatrixOperations::maxMatrix(QVector< QVector<double> > const *m,  double *result)  //SG add
{
    // Check for null pointers
    if (m == 0 || result == 0) {
        qCritical() << "A null pointer came into the maxMatrix-function in the matrix operations.";
        return -1;
    }

    double max = 0.0;

    // Get sizes
    int rowsm = m->size();
    int colsm = m[0][0].size();

for (int i=0;i<rowsm;i++)
{
    for (int j=0;j<colsm;j++)
    {

        if ((*m)[i][j]>max)
        {
            max = (*m)[i][j];
        }

    }
}


  (*result) = max;

    return 0;
}



int RMatrixOperations::minMatrix(QVector< QVector<double> > const *m,  double *result)   //SG add
{
    // Check for null pointers
    if (m == 0 || result == 0) {
        qCritical() << "A null pointer came into the maxMatrix-function in the matrix operations.";
        return -1;
    }

    double min = 0.0;

    // Get sizes
    // Get sizes
    int rowsm = m->size();
    int colsm = m[0][0].size();

for (int i=0;i<rowsm;i++)
{
    for (int j=0;j<colsm;j++)
    {

        if ((*m)[i][j] < min)
        {
            min = (*m)[i][j];
        }

    }
}


  (*result) = min;

    return 0;
}


double RMatrixOperations::traceMatrix(QVector< QVector<double> > const *m) //SG add
{
    double trace = 0.0;

    // Get sizes
    int rowsm = m->size();
    int colsm = m[0][0].size();

    for (int i=0;i<rowsm;i++)
    {
        for (int j=0;j<colsm;j++)
        {
            if (i==j)
            {
                trace+= (*m)[i][j];
            }
        }

    }

    return trace;

}




#ifdef  OSG

// Gets the convex hull of a set of N points --> Graham sweep algo.
void RMatrixOperations::convexHull(QList<osg::Vec3>& thePointsList)
{
    typedef osg::Vec2 Point;

    // A globle point needed for  sorting points with reference to the first point Used in compare function
    Point p0;

    int n = thePointsList.size();

    //first convert the 3-D points into 2-D projection..
    QList<osg::Vec2> points = get2DProjection(thePointsList);

    //Save the original points layout
    QList<osg::Vec2> OrigPoints = points;

    // A lamda function to find next to top in a stack
    auto nextToTop = [&](std::stack<Point> S)
    {
        Point p = S.top();
        S.pop();
        Point res = S.top();
        S.push(p);
        return res;
    };

    // A lamda function to swap two points
   /* auto swap = [&](Point p1, Point p2)
    {
        Point temp = p1;
        p1 = p2;
        p2 = temp;
    };*/

    // A utility function to return square of distance between p1 and p2
    std::function<int(Point,Point)> distSq = [](Point p1, Point p2)
    {
        return (p1.x() - p2.x())*(p1.x() - p2.x()) + (p1.y() - p2.y())*(p1.y() - p2.y());
    };

     /*
      *     To find orientation of ordered triplet (p, q, r).
      *     The function returns following values
      *     0 --> p, q and r are colinear
      *     1 --> Clockwise
      *     2 --> Counterclockwise
      */

    std::function<int(Point,Point,Point)> orientation = [](Point p, Point q, Point r)
    {
        int val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());

        if (val == 0) return 0;  // colinear
        return (val > 0)? 1: 2; // clock or counterclock wise
    };

    // A function used by library function qsort() to sort an array of
    // points with respect to the first point


    auto compare = [&orientation,&distSq,&p0](Point p1, Point p2)
    {
       //Point *p1 = (Point *)vp1;
       //Point *p2 = (Point *)vp2;

       // Find orientation
       int o = orientation(p0, p1, p2);
       if (o == 0)
         return (distSq(p0, p2) >= distSq(p0, p1))? -1 : 1;

       return (o == 2)? -1: 1;
    };


   // Find the bottommost point
   int ymin = points[0].y(), min = 0;

   for (int i = 1; i < n; i++)
   {
     int y = points[i].y();

     // Pick the bottom-most or chose the left
     // most point in case of tie
     if ((y < ymin) || (ymin == y &&
         points[i].x() < points[min].x()))
        ymin = points[i].y(), min = i;
   }

   // Place the bottom-most point at first position
   //swap(points[0], points[min]);
   points.swap(0,min);

   // Sort n-1 points with respect to the first point.
   // A point p1 comes before p2 in sorted ouput if p2
   // has larger polar angle (in counterclockwise
   // direction) than p1
   p0 = points[0];

   std::sort(&points[1], &points[n], compare);

   // If two or more points make same angle with p0,
   // Remove all but the one that is farthest from p0
   // Remember that, in above sorting, our criteria was
   // to keep the farthest point at the end when more than
   // one points have same angle.
   int m = 1; // Initialize size of modified array
   for (int i=1; i<n; i++)
   {
       // Keep removing i while angle of i and i+1 is same
       // with respect to p0
       while (i < n-1 && orientation(p0, points[i], points[i+1]) == 0)
          i++;


       points[m] = points[i];
       m++;  // Update size of modified array
   }

   // If modified array of points has less than 3 points,
   // convex hull is not possible
   if (m < 3) return;

   // Create an empty stack and push first three points
   // to it.
   std::stack<Point> S;
   S.push(points[0]);
   S.push(points[1]);
   S.push(points[2]);

   // Process remaining n-3 points
   for (int i = 3; i < m; i++)
   {
      // Keep removing top while the angle formed by
      // points next-to-top, top, and points[i] makes
      // a non-left turn
      while (orientation(nextToTop(S), S.top(), points[i]) != 2)
         S.pop();
      S.push(points[i]);
   }

   QList<Point> finalSortedList;

   // Now stack has the output points, print contents of stack
    while (!S.empty())
    {
        finalSortedList<<S.top();

        S.pop();
    }

   //Now rearrage the 3-D Vector according to its sorted 2-D Vector
   for(int i=0;i<S.size();i++)
   {
       for(int j=0;j<S.size();j++)
       {
           if(finalSortedList[i] == OrigPoints[j])
           {
              thePointsList.swap(j,i);
           }
       }
   }


}



osg::Vec3 RMatrixOperations::computePolygonNormal( const osg::Vec3Array* polygon )
{
    const int num_points = polygon->size();

    osg::Vec3 polygon_normal( 0, 0, 0 );

    for( int k = 0; k < num_points; ++k )
    {
        const osg::Vec3& vertex_current = polygon->at( k );
        const osg::Vec3& vertex_next = polygon->at( ( k + 1 ) % num_points );
        polygon_normal._v[0] += ( vertex_current.y() - vertex_next.y() )*( vertex_current.z() + vertex_next.z() );
        polygon_normal._v[1] += ( vertex_current.z() - vertex_next.z() )*( vertex_current.x() + vertex_next.x() );
        polygon_normal._v[2] += ( vertex_current.x() - vertex_next.x() )*( vertex_current.y() + vertex_next.y() );
    }

    polygon_normal.normalize();

    return polygon_normal;
}

// Gets the 2-D projection of points in 3-D space on a given plane
QList<osg::Vec2> RMatrixOperations::get2DProjection(QList<osg::Vec3> thePointsList)
{
  //Declare some variables
  osg::Vec3Array* polygon = new osg::Vec3Array;
  std::vector<osg::Vec3> pointsVector(thePointsList.size());
  QList<osg::Vec2> the2DPointsList;

  //first calculate the polygon normal
  for (int i =0; i<thePointsList.size(); i++)
  {
      polygon->push_back( thePointsList[i] );
      pointsVector.push_back(thePointsList[i] );
  }

  osg::Vec3 theNormal = computePolygonNormal(polygon);

  //then Compute two orthogonal direction vectors to form the orthogonal triplet
  auto orthogonal = [](osg::Vec3 &v)
  {
      double intpart;

      float k = modf(abs(v.x()) + 0.5,&intpart);

      return osg::Vec3(-v.y(), v.x() - k * v.z(), k * v.y());
  };

  //First orthogonal vector
  osg::Vec3 theAxis1 = orthogonal(theNormal);

  //Cross product
  osg::Vec3  theAxis2 = theAxis1^(theNormal);

  //define the origin on the plane... use centroid!
  osg::Vec3  centroid = computePolygonCentroid(pointsVector);

  for(int i=0;i<thePointsList.size(); i++ )
  {
      float p1 = theAxis1*(thePointsList[i]-centroid);
      float p2 = theAxis2*(thePointsList[i]-centroid);

      osg::Vec2 the2DPoint(p1,p2);

      the2DPointsList.append(the2DPoint);
  }

  return the2DPointsList;
}

osg::Vec3 RMatrixOperations::computePolygonCentroid( const std::vector<osg::Vec3>& polygon )
{
    osg::Vec3 polygonCentroid( osg::Vec3( 0, 0, 0 ) );
    for( std::vector<osg::Vec3>::const_iterator it = polygon.begin(); it != polygon.end(); ++it )
    {
        const osg::Vec3& vertex_current = ( *it );
        polygonCentroid += vertex_current;
    }
    polygonCentroid /= (double)( polygon.size() );

    return polygonCentroid;
}


bool RMatrixOperations::checkOpenPolygonConvexity( const std::vector<osg::Vec2>& polygon )
{
    if( polygon.size() < 3 )
    {
        return true;
    }
    const int num_points = polygon.size();
    double zcrossproduct_previous = 0;
    for( int k = 0; k < num_points - 2; ++k )
    {
        const osg::Vec2& vertex_current = polygon[k];
        const osg::Vec2& vertex_next1 = polygon[k + 1];
        const osg::Vec2& vertex_next2 = polygon[k + 2];

        double dx1 = vertex_next1.x() - vertex_current.x();
        double dy1 = vertex_next1.y() - vertex_current.y();

        double dx2 = vertex_next2.x() - vertex_next1.x();
        double dy2 = vertex_next2.y() - vertex_next1.y();

        double zcrossproduct = dx1*dy2 - dy1*dx2;

        if( k > 0 )
        {
            if( std::abs( zcrossproduct ) > 0.0001 )
            {
                if( std::abs( zcrossproduct_previous ) > 0.0001 )
                {
                    if( zcrossproduct*zcrossproduct_previous < 0 )
                    {
                        // there is a change in direction -> not convex
                        return false;
                    }
                }
            }
        }
        zcrossproduct_previous = zcrossproduct;
    }
    return true;
}



#endif



