#include "RSimpleWindVelocityModel.h"

#include "RParameter.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"

#include <math.h>

RSimpleWindVelocityModel::RSimpleWindVelocityModel(QObject *parent, QString name)
	: RModel(parent, name)
{
	// Initializing the data members
	theTheta = 0;
	theV10 = 0;
	theV50 = 0;
    theProbabilityDistributions = 0;

	
	// Instantiating the response object
	theDomain->createObject(objectName() + "Response", "RGenericResponse");
	theWindSpeed = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
	theWindSpeed->setModel(this);
}

RSimpleWindVelocityModel::~RSimpleWindVelocityModel()
{

}


QObject * RSimpleWindVelocityModel::getProbabilityDistributions() const
{
    return theProbabilityDistributions;
}

void RSimpleWindVelocityModel::setProbabilityDistributions(QObject *value)
{
    theProbabilityDistributions = qobject_cast<RProbabilityDistributions *>(value);
}

	
QObject * RSimpleWindVelocityModel::getTheta() const	
{	
	return theTheta;
}	
	
void RSimpleWindVelocityModel::setTheta(QObject *value)	
{	
	theTheta = qobject_cast<RParameter *>(value);	
}	

QObject * RSimpleWindVelocityModel::getV10() const	
{	
	return theV10;
}	
	
void RSimpleWindVelocityModel::setV10(QObject *value)	
{	
	theV10 = qobject_cast<RParameter *>(value);
}	
	
QObject * RSimpleWindVelocityModel::getV50() const	
{	
	return theV50;
}	
	
void RSimpleWindVelocityModel::setV50(QObject *value)	
{	
	theV50 = qobject_cast<RParameter *>(value);
}

QList<RParameter *> RSimpleWindVelocityModel::getPhysicalParameterList()
{
	QList<RParameter *> paramList;
	paramList << theTheta;
	return paramList;
}

int RSimpleWindVelocityModel::evaluateModel(bool evaluateDDM, RObject::RParameterType parameterType)
{	
	//Equation from: Pandey M D, An Y, "The analysis of design wind speed estimates specified in the National Building Code of Canada", , Vol. Iss: , pp.513 - 524
	// ATTENTION: THE FORMULA IN THE PAPER IS ERRATIC
	// CORRECT FORMULA: V(T) = 1/1.4565*(V50+0.4565*V10+(V50-V10)/(1.1339)*ln(-0.0339/ln(1-1/T)))

	// Calculating the Gumbel distribution parameters for the velocity: CDF(V) = exp(-exp((V - A) / B))
	double v10 = theV10->getCurrentValue();
	double v50 = theV50->getCurrentValue();
	double B = (v50 - v10) / (1.1339 * 1.4565);
	double A = (v50 + 0.4565 * v10) / 1.4565 + B * log(0.0339);

	// Converting the Gumbel distibution parameters to GSL a and b
	double a = 1 / B;
	double b = exp(A / B);
	
	// Calculating the velocity
	double theta = theTheta->getCurrentValue();
    double thetaProb = theProbabilityDistributions->getNormalCDF(theta, 1.0);

	// This is a COMPLEMENTARY GUMBEL MINIMUM distribution
    double velocity = theProbabilityDistributions->getInverseGumbelCDF(thetaProb, a, b);

	theWindSpeed->setCurrentValue(velocity);

	return 0;
}
