#ifndef RNormalDistribution_H
#define RNormalDistribution_H

#include "RMethod.h"
#include "RDomain.h"

class RNormalDistribution : public RMethod
{

public:
    RNormalDistribution(RDomain *theDomain);
    ~RNormalDistribution();

    double getNormalPDF(double x, double sigma);
    double getNormalCDF(double x, double sigma);
    double getInverseNormalCDF(double x, double sigma);

private:

    double incompleteGammaFunction(double a, double x);
    double standardNormalPhi(double uValue);
    double standardNormalInversePhi(double probValue);
    double errorFunction(double x);
    double inverseErrorFunction(double y);
};

#endif
