#include "RUserDefinedMagnitudeModel.h"

#include "RParameter.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include <math.h>

RUserDefinedMagnitudeModel::RUserDefinedMagnitudeModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    theOccurrence = 0;

    // Instantiating the response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theMagnitude = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theMagnitude->setModel(this);

    // Create the standard normal random variable
    theDomain->createObject(objectName() + "_theta", "RContinuousRandomVariable");
    theStandardNormalRandomVariable = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theStandardNormalRandomVariable->setDistributionType(QString("Normal (mean, stdv)"));
    theStandardNormalRandomVariable->setMean("0.0");
    theStandardNormalRandomVariable->setStandardDeviation("1.0");
    theAutoParametersList << theStandardNormalRandomVariable;
}

RUserDefinedMagnitudeModel::~RUserDefinedMagnitudeModel()
{
}





QString RUserDefinedMagnitudeModel::getXPoints()
{
    return theXPoints;
}

void RUserDefinedMagnitudeModel::setXPoints(QString value)
{
    theXPoints = value;
}



QString RUserDefinedMagnitudeModel::getPDFPoints()
{
    return thePDFPoints;
}

void RUserDefinedMagnitudeModel::setPDFPoints(QString value)
{
    thePDFPoints = value;
}



QObject * RUserDefinedMagnitudeModel::getOccurrence() const
{
    return theOccurrence;
}

void RUserDefinedMagnitudeModel::setOccurrence(QObject *value)
{
    theOccurrence = qobject_cast<RParameter *>(value);
}





int RUserDefinedMagnitudeModel::evaluateModel()
{	
    if (theOccurrence->getCurrentValue() == 0.0) {

        theMagnitude->setCurrentValue(0.0);

        return 0;
    }



    // Create an auxiliary tool to get the normal distribution
    RNormalDistribution theNormalDistribution(theDomain);


    // Calculate the probability of the model StandardNormalRandomVariable
    double realization = theStandardNormalRandomVariable->getCurrentValue();
    double probability = theNormalDistribution.getNormalCDF(realization, 1.0);


    // Create a local random variable
    RContinuousRandomVariable randomVariable(0, QString("RV"));
    randomVariable.setDistributionType("User Defined");
    randomVariable.setProperty("XPoints", theXPoints);
    randomVariable.setProperty("PDFPoints", thePDFPoints);
    double response = randomVariable.getInverseCDFValue(probability);


    // Updating the response current value
    theMagnitude->setCurrentValue(response);

    return 0;
}
