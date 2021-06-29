#include "RNeighborhoodModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include "RIntensityModel.h"
#include "RAHSPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"
#include "RNetworkSamplingModel.h"


#include <math.h>

RNeighborhoodModel::RNeighborhoodModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{

}

RNeighborhoodModel::~RNeighborhoodModel()
{
}

QObject * RNeighborhoodModel::getHouseholdPortfolio() const
{
    return theHouseholdPortfolio;
}

void RNeighborhoodModel::setHouseholdPortfolio(QObject *value)
{
    theHouseholdPortfolio = qobject_cast<RHouseholdPortfolio*>(value);
}


QObject* RNeighborhoodModel::getSa()
{
    return theIntensityModelResponse;
}

void RNeighborhoodModel::setSa(QObject *value)
{
    theIntensityModelResponse =  qobject_cast<RResponse*>(value);
}


QObject* RNeighborhoodModel::getBuildingDamageModel()
{
    return theBuildingDamageModelResponse;
}

void RNeighborhoodModel::setBuildingDamageModel(QObject* value)
{
    theBuildingDamageModelResponse = qobject_cast<RResponse*>(value);
}

int RNeighborhoodModel::getSamplingStartingPoint()
{
    return theSamplingStartingPoint;
}

void RNeighborhoodModel::setSamplingStartingPoint(int value)
{
    theSamplingStartingPoint = value;
}

QVector<double> RNeighborhoodModel::getTimeVector()
{
    return timeVector;
}

int RNeighborhoodModel::getNumberOfRepairedDwellings()
{
    return theNumberOfRepairedDwellings;
}


QObject* RNeighborhoodModel::getDisplacementEstimator()
{
    return theDisplacementEstimator;
}

void RNeighborhoodModel::setDisplacementEstimator(QObject *value)
{
    theDisplacementEstimator = qobject_cast<RPopulationDisplacementEstimator*>(value);
}

QObject* RNeighborhoodModel::getFinancingTimeEstimator()
{
    return theFinancingTimeEstimator;
}

void RNeighborhoodModel::setFinancingTimeEstimator(QObject *value)
{
    theFinancingTimeEstimator = qobject_cast<RAlisjahbana2021FinancingTimeEstimator*>(value);
}

QString RNeighborhoodModel::getContractorList()
{
    return parameterListToString(theContractorList);
}

void RNeighborhoodModel::setContractorList(QString value)
{
    theContractorList = stringToParameterList(value);
}

//QString RNeighborhoodModel::getFinancialResourcesSupplierList()
//{
//    return parameterListToString(theFinancialResourcesSupplierList);
//}

//void RNeighborhoodModel::setFinancialResourcesSupplierList(QString value)
//{
//    theFinancialResourcesSupplierList = stringToParameterList(value);
//}
