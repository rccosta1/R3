#ifndef RInHouseProbabilityDistributions_H
#define RInHouseProbabilityDistributions_H

#include "RProbabilityDistributions.h"


class RInHouseProbabilityDistributions : public RProbabilityDistributions
{
    Q_OBJECT

public:
    RInHouseProbabilityDistributions(QObject *parent, QString name);
    ~RInHouseProbabilityDistributions();

    // Methods for the Normal distribution
    double getNormalPDF(double x, double sigma);
    double getNormalCDF(double x, double sigma);
    double getInverseNormalCDF(double x, double sigma);

    double generateStandardNormalNumber();
    double generateBinomialNumber(double p, int n);
    double gammaFunction(double x);
    double generateExponentialNumber(double mu);

    // Methods for the Exponential distribution:
    double getExponentialPDF(double x, double mu);
    double getExponentialCDF(double x, double mu);
    double getInverseExponentialCDF(double x, double mu);

    // Methods for the Laplace distribution:
    double getLaplacePDF(double x, double a);
    double getLaplaceCDF(double x, double a);
    double getInverseLaplaceCDF(double x, double a);

    // Methods for the Rayleigh distribution:
    double getRayleighPDF(double x, double sigma);
    double getRayleighCDF(double x, double sigma);
    double getInverseRayleighCDF(double x, double sigma);

    // Methods for the Gamma distribution:
    double getGammaPDF(double x, double a, double b);
    double getGammaCDF(double x, double a, double b);
    double getInverseGammaCDF(double x, double a, double b);

    // Methods for the Uniform distribution:
    double getUniformPDF(double x, double a, double b);
    double getUniformCDF(double x, double a, double b);
    double getInverseUniformCDF(double x, double a, double b);

    // Methods for the Lognormal distribution:
    double getLognormalPDF(double x, double zeta, double sigma);
    double getLognormalCDF(double x, double zeta, double sigma);
    double getInverseLognormalCDF(double x, double zeta, double sigma);

    // Methods for the Chi-squared distribution:
    double getChiSquaredPDF(double x, double nu);
    double getChiSquaredCDF(double x, double nu);
    double getInverseChiSquaredCDF(double x, double nu);

    // Methods for the t-distribution distribution:
    double getTDistributionPDF(double x, double nu);
    double getTDistributionCDF(double x, double nu);
    double getInverseTDistributionCDF(double x, double nu);

    // Methods for the Beta distribution:
    double getBetaPDF(double x, double a, double b);
    double getBetaCDF(double x, double a, double b);
    double getInverseBetaCDF(double x, double a, double b);

    // Methods for the Logistic distribution:
    double getLogisticPDF(double x, double a);
    double getLogisticCDF(double x, double a);
    double getInverseLogisticCDF(double x, double a);

    // Methods for the Weibull distribution:
    double getWeibullPDF(double x, double a, double b);
    double getWeibullCDF(double x, double a, double b);
    double getInverseWeibullCDF(double x, double a, double b);

    // Methods for the Gumbel distribution:
    double getGumbelPDF(double x, double a, double b);
    double getGumbelCDF(double x, double a, double b);
    double getInverseGumbelCDF(double x, double a, double b);

private:

    double incompleteGammaFunction(double a, double x);
    double standardNormalPhi(double uValue);
    double standardNormalInversePhi(double probValue);
    double errorFunction(double x);
    double inverseErrorFunction(double y);
};

#endif // RInHouseProbabilityDistributions_H
