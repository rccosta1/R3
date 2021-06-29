#include "RBuildingPortfolio.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RNormalDistribution.h"
#include "RLocation.h"
#include <math.h>


RBuildingPortfolio::RBuildingPortfolio(QObject *parent, QString name)
    : RObject(parent, name)
{
    theNumberOfBuildings = 0;
}


RBuildingPortfolio::~RBuildingPortfolio()
{
    
}


int RBuildingPortfolio::resetTime()
{
    return 0;
}


void RBuildingPortfolio::setMunicipality(QString value)
{
    inputMunicipality = value;

    if (inputMunicipality == QString("Vancouver")){
        medianMunicipalHouseholdIncome = 56908.0;
        insuranceTakeUp = 0.45;

    } else if (inputMunicipality == QString("SanFrancisco")){
        medianMunicipalHouseholdIncome = 96000.0;
        insuranceTakeUp = 0.15;

    } else {
        qCritical() << "The" << this->objectName() << "could not identify the municipality. Using default values for insurace take-up rate and building values!!";
        medianMunicipalHouseholdIncome = 50000.0;
        insuranceTakeUp = 0.25;
    }
}

double RBuildingPortfolio::getMedianHouseholdIncome() const
{
    return theMedianHouseholdIncome;
}

void RBuildingPortfolio::setMedianHouseholdIncome(double value)
{
    theMedianHouseholdIncome = value;
}



QString RBuildingPortfolio::getMunicipality() const
{
    return inputMunicipality;
}


QString RBuildingPortfolio::getBuildingTypes()
{
    return inputBuildingTypes;
}

void RBuildingPortfolio::setBuildingTypes(QString value)
{
    inputBuildingTypes = value;
}


QString RBuildingPortfolio::getBuildingOccupancyClasses()
{
    return inputBuildingOccupancyClasses;
}

void RBuildingPortfolio::setBuildingOccupancyClasses(QString value)
{
    inputBuildingOccupancyClasses = value;
}


QString RBuildingPortfolio::getBuildingCodeLevel()
{
    return inputBuildingCodeLevels;
}

void RBuildingPortfolio::setBuildingCodeLevel(QString value)
{
    inputBuildingCodeLevels = value;
}

QString RBuildingPortfolio::getBuildingNumberPerType()
{
    return inputNumberPerBuildingType;
}

void RBuildingPortfolio::setBuildingNumberPerType(QString value)
{
    inputNumberPerBuildingType = value;
}


QString RBuildingPortfolio::getBuildingAreas()
{
    return doubleVectorToString(inputBuildingAreas);
}

void RBuildingPortfolio::setBuildingAreas(QString value)
{
    inputBuildingAreas = stringToDoubleVector(value);
}

double RBuildingPortfolio::getBuildingValues()
{
    return inputBuildingValues;
}

void RBuildingPortfolio::setBuildingValues(double value)
{
    inputBuildingValues = value;
}

QString RBuildingPortfolio::getBuildingDensity()
{
    return inputBuildingDensity;
}

void RBuildingPortfolio::setBuildingDensity(QString value)
{
    inputBuildingDensity = value;
}

double RBuildingPortfolio::getRetrofitLevel()
{
    return theRetrofitLevel;
}

void RBuildingPortfolio::setRetrofitLevel(double value)
{
    theRetrofitLevel = value;
}

QString RBuildingPortfolio::getRoomsDistribution()
{
    return doubleVectorToString(inputRoomsDistributionVector);
}

void RBuildingPortfolio::setRoomsDistribution(QString value)
{
    inputRoomsDistributionVector = stringToDoubleVector(value);
}


// Extractors
QVector<QString> RBuildingPortfolio::getBuildingTypesVector() const
{
    return theBuildingTypesVector;
}


QVector<int> RBuildingPortfolio::getYearBuiltVector() const
{
    return theBuildingYearBuiltVector;
}


QVector<QString> RBuildingPortfolio::getBuildingCodeLevelVector() const
{
    return theBuildingCodeLevelVector;
}


QVector<QString> RBuildingPortfolio::getBuildingOccupancyClassVector() const
{
    return theBuildingOccupancyClassVector;
}


QVector<int> RBuildingPortfolio::getBuildingStoreysVector() const
{
    return theBuildingStoreysVector;
}


QVector<int> RBuildingPortfolio::getBuildingDwellingsVector() const
{
    return theNumberOfDwellingsPerBuilding;
}


QVector<int> RBuildingPortfolio::getBuildingRoomsVector() const
{
    return theBuildingNumberOfRoomsVector;
}



QVector<double> RBuildingPortfolio::getBuildingAreaVector() const
{
    return theBuildingAreasVector;
}

QVector<double> RBuildingPortfolio::getLandValueVector() const
{
    return theBuildingLandValueVector;
}

QVector<double> RBuildingPortfolio::getBuildingValueVector() const
{
    return theBuildingValueVector;
}


QVector<RLocation*> RBuildingPortfolio::getBuildingLocationVector() const
{
    return theBuildingLocationVector;
}


int RBuildingPortfolio::getNumberOfBuildings() const
{
    return theNumberOfBuildings;
}


int RBuildingPortfolio::instantiateBuildings()
{
    return 0;
}



