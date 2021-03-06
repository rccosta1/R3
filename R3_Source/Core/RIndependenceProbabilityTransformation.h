#ifndef RIndependenceProbabilityTransformation_H
#define RIndependenceProbabilityTransformation_H

#include "RProbabilityTransformation.h"



class RIndependenceProbabilityTransformation : public RProbabilityTransformation
{
	Q_OBJECT

    Q_PROPERTY(QObject *ProbabilityDistributions READ getProbabilityDistributions WRITE setProbabilityDistributions)

		
public:
	RIndependenceProbabilityTransformation(QObject *parent, QString name);
	~RIndependenceProbabilityTransformation();

    QObject *getProbabilityDistributions() const;
    void setProbabilityDistributions(QObject *value);

	// Establishes the correlation matrix and the computes the Cholesky decomposition at the start of the analysis
    int initializeAtStartOfAnalysis();

    // Transforms random variable realizations from the standard normal to the original space
    int transformPointToOriginalSpace(const QVector<double> *standardNormalVector, QVector<double> *originalVector);

	// Transforms random variable realizations from the original to the standard normal space
    int transformPointToStandardNormalSpace(const QVector<double> *originalVector, QVector<double> *standardNormalVector);

    // Transforms a gradient vector from the standard normal to the original space
    int transformGradientToOriginalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInStandardNormalSpace, QVector<double> *vectorInOriginalSpace);

	// Transforms a gradient vector from the original to the standard normal space
    int transformGradientToStandardNormalSpace(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, const QVector<double> *vectorInOriginalSpace, QVector<double> *vectorInStandardNormalSpace);

	// Computes and returns the Jacobiam matrix
    int computeJacobian(const QVector<double> *pointInOriginalSpace, const QVector<double> *pointInStandardNormalSpace, QVector< QVector<double> > *jacobian, RProbabilityTransformation::RJacobianType jacobianType);

	// Computes and returns the Jacobian matrix of the vector of standard normal random variables wrt. the means and stdvs of original random variables
    int computeSensitivityToDistributionParameters(const QVector<double> *pointInOriginalSpace, QVector< QVector<double> > *dy_dMean, QVector< QVector<double> > *dy_dStdv);


private:

    QPointer<RProbabilityDistributions> theProbabilityDistributions;

	// Auxiliary method to compute the diagonal Jacobian matrix of the standard normal variables (u) wrt. the original random variables (x)
    int computeDiagonalJacobian_u_x_AsVector(const QVector<double> *originalVector, const QVector<double> *standardNormalVector, QVector<double> *jacobianV_u_x);
	
};

#endif // RIndependenceProbabilityTransformation_H
