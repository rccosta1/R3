#include "RSimCenterBuildingPortfolio.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RNormalDistribution.h"
#include "RPolygonalAreaSourceModel.h"
#include "RLocation.h"
#include <math.h>


RSimCenterBuildingPortfolio::RSimCenterBuildingPortfolio(QObject *parent, QString name)
    : RBuildingPortfolio(parent, name)
{
    theNumberOfBuildings = 0;
    theRetrofitLevel = 0.0;
    this->setMunicipality(QString("SanFrancisco"));
}


RSimCenterBuildingPortfolio::~RSimCenterBuildingPortfolio()
{
    
}


int RSimCenterBuildingPortfolio::resetTime()
{
    return 0;
}

void RSimCenterBuildingPortfolio::setMunicipality(QString value)
{
    inputMunicipality = value;
}

QString RSimCenterBuildingPortfolio::getMunicipality()
{
    return inputMunicipality;
}


QString RSimCenterBuildingPortfolio::getLatitudes()
{
    return doubleVectorToString(inputLatitudes);
}

void RSimCenterBuildingPortfolio::setLatitudes(QString value)
{
    inputLatitudes = stringToDoubleVector(value);
}


QString RSimCenterBuildingPortfolio::getLongitudes()
{
    return doubleVectorToString(inputLongitudes);
}

void RSimCenterBuildingPortfolio::setLongitudes(QString value)
{
    inputLongitudes = stringToDoubleVector(value);
}


QString RSimCenterBuildingPortfolio::getNumberOfStories()
{
    return intVectorToString(inputNumberOfStories);
}

void RSimCenterBuildingPortfolio::setNumberOfStories(QString value)
{
    inputNumberOfStories = stringToIntVector(value);
}


QString RSimCenterBuildingPortfolio::getYearBuilt()
{
    return intVectorToString(inputYearBuilt);
}

void RSimCenterBuildingPortfolio::setYearBuilt(QString value)
{
    inputYearBuilt = stringToIntVector(value);
}


QString RSimCenterBuildingPortfolio::getBuildingType()
{
    return inputBuildingTypes;
}

void RSimCenterBuildingPortfolio::setBuildingType(QString value)
{
    inputBuildingTypes = value;
}


QString RSimCenterBuildingPortfolio::getBuildingOccupancy()
{
    return inputBuildingOccupancyClasses;
}

void RSimCenterBuildingPortfolio::setBuildingOccupancy(QString value)
{
    inputBuildingOccupancyClasses = value;
}


QString RSimCenterBuildingPortfolio::getBuildingArea()
{
    return doubleVectorToString(inputBuildingAreas);
}

void RSimCenterBuildingPortfolio::setBuildingArea(QString value)
{
    inputBuildingAreas = stringToDoubleVector(value);
}


double RSimCenterBuildingPortfolio::getRetrofitLevel()
{
    return theRetrofitLevel;
}

void RSimCenterBuildingPortfolio::setRetrofitLevel(double value)
{
    theRetrofitLevel = value;
}


int RSimCenterBuildingPortfolio::instantiateBuildings()
{
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now isntantiating buildings" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    // -------------------------------------------------------------------------------------------
    // 0 - Instantiate the owner based on census information
    // -------------------------------------------------------------------------------------------
    // End this quickly if the household households were already defined
    if (theNumberOfBuildings > 0){
        return 0;
    }
    
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Instantiating buildings at" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // Get the list of buildings from the input file
    QStringList theListOfBuildingTypes = inputBuildingTypes.split(QRegularExpression("\\W+"), QString::SkipEmptyParts);
    QStringList theListOfOccupancyClasses = inputBuildingOccupancyClasses.split(QRegularExpression("\\W+"), QString::SkipEmptyParts);
    QVector<int> theNumberOfInstancesPerBuildingType;

    // Check if the vectors containing building properties have the same lengths
    // If not, there was a problem with the input file generation and it needs to be fix before running
    int stdSize = theListOfBuildingTypes.size();
    if (inputYearBuilt.size() != stdSize || inputBuildingAreas.size() != stdSize || inputNumberOfStories.size() != stdSize || theListOfOccupancyClasses.size() != stdSize){
        qCritical() << "There was a problem in" << this->objectName() << ". The size of the vectors defining the building properties do not match. Check your input file!";
        qCritical() << "theListOfBuildingTypes.size()" << theListOfBuildingTypes.size();
        qCritical() << "inputYearBuilt.size()" << inputYearBuilt.size();
        qCritical() << "inputBuildingAreas.size()" << inputBuildingAreas.size();
        qCritical() << "inputNumberOfStories.size()" << inputNumberOfStories.size();
        qCritical() << "theListOfOccupancyClasses.size()" << theListOfOccupancyClasses.size();
        qCritical() << "____________________________________";
    }


    // Get the number of buildings
    theNumberOfBuildings = theListOfBuildingTypes.size();

    // For each building type, generate the buildings
    for (int i = 0; i < theNumberOfBuildings; i++){


        if (!theListOfBuildingTypes.at(i).contains("W")){

            if (theListOfBuildingTypes.at(i) == QString("S3") || theListOfBuildingTypes.at(i) == QString("PC1") || theListOfBuildingTypes.at(i) == QString("MH")){
                // do nothing because these buildings do not get L/M/H denominations

                // if RM1
            } else if (theListOfBuildingTypes.at(i) == QString("RM1")){
                // If low rising
                if (inputNumberOfStories.at(i) < 3){
                    theListOfBuildingTypes[i] += QString("L");

                    // If medium rise
                } else {
                    theListOfBuildingTypes[i] += QString("M");
                }

                // if URM
            } else if (theListOfBuildingTypes.at(i) == QString("URM")){
                // If low rising
                if (inputNumberOfStories.at(i) < 2){
                    theListOfBuildingTypes[i] += QString("L");

                    // If medium rise
                } else {
                    theListOfBuildingTypes[i] += QString("M");
                }

                // All other building types
            } else {

                // If low rising
                if (inputNumberOfStories.at(i) < 3){
                    theListOfBuildingTypes[i] += QString("L");

                    // If high rise
                } else if (inputNumberOfStories.at(i) > 7){
                    theListOfBuildingTypes[i] += QString("H");

                    // If medium rise
                } else {
                    theListOfBuildingTypes[i] += QString("M");
                }
            }
        }

        // Location
        theDomain->createObject(objectName() + "BuildingLocation" + QVariant(i).toString(), "RLocation");
        RLocation *myLocation = qobject_cast<RLocation *>(theDomain->getLastAddedObject());
        myLocation->setLatitude(inputLatitudes.at(i));
        myLocation->setLongitude(inputLongitudes.at(i));

        // Stories
        int myNumberOfStories = inputNumberOfStories.at(i);


        // Area
        double myBuildingArea = inputBuildingAreas.at(i);


        // Value
        double myBuildingValue = myBuildingArea * 1000.0; // Placeholder value

        // Number of dwellings
        int myNumberOfDwellings = 1;
        if (theListOfOccupancyClasses.at(i) == QString("RES3")){
            myNumberOfDwellings = inputNumberOfStories.at(i) * 4; // considering four apartments per story on average
        }

        if (theListOfOccupancyClasses.at(i) == QString("RES3")){
            if (myNumberOfDwellings < 3){
                theListOfOccupancyClasses[i] += QString("A");

            }else if(myNumberOfDwellings == 3 || myNumberOfDwellings == 4){
                theListOfOccupancyClasses[i] += QString("B");

            } else if(myNumberOfDwellings > 4 && myNumberOfDwellings <= 9){
                theListOfOccupancyClasses[i] += QString("C");

            } else if(myNumberOfDwellings > 9 && myNumberOfDwellings <= 19){
                theListOfOccupancyClasses[i] += QString("D");

            } else if(myNumberOfDwellings > 20 && myNumberOfDwellings <= 49){
                theListOfOccupancyClasses[i] += QString("E");

            } else {
                theListOfOccupancyClasses[i] += QString("F");
            }
        }


        // Building Code Design Levels
        QString CodeLevel;

        if (inputMunicipality == QString("SanFrancisco")){
            // San Francisco is on UBC's Seismic Zone 4/FEMA zone 9 as per Kiremidjian

            // For wood-frame buildings
            if (theListOfBuildingTypes.at(i).contains("W")){
                if (inputYearBuilt.at(i) <= 1940){
                    CodeLevel = QString("Moderate");
                }
                else if (inputYearBuilt.at(i) > 1940 && inputYearBuilt.at(i) <= 1975){
                    CodeLevel = QString("Moderate");
                }
                else if (inputYearBuilt.at(i) > 1975){
                    CodeLevel = QString("High");
                }
                else {
                    qCritical() << this->objectName() << "could not identify the code level for building type" << theListOfBuildingTypes.at(i);
                }

                // For S5, C3, and URM buildings
            } else if (theListOfBuildingTypes.at(i).contains("S5") || theListOfBuildingTypes.at(i).contains("C3") || theListOfBuildingTypes.at(i).contains("URM")){
                if (inputYearBuilt.at(i) <= 1940){
                    CodeLevel = QString("Pre");
                } else {
                    CodeLevel = QString("Low");
                }

                // For other buildings
            } else {
                if (inputYearBuilt.at(i) <= 1940){
                    CodeLevel = QString("Pre");
                }
                else if (inputYearBuilt.at(i) > 1940 && inputYearBuilt.at(i) <= 1975){
                    CodeLevel = QString("Moderate");
                }
                else if (inputYearBuilt.at(i) > 1975){
                    CodeLevel = QString("High");
                }
                else {
                    qCritical() << this->objectName() << "could not identify the code level for building type" << theListOfBuildingTypes.at(i);
                }
            }
        }

        theBuildingStoreysVector        << myNumberOfStories;
        theNumberOfDwellingsPerBuilding << myNumberOfDwellings;
        theBuildingYearBuiltVector      << inputYearBuilt.at(i);
        theBuildingAreasVector          << myBuildingArea;
        theBuildingValueVector          << myBuildingValue;
        theBuildingTypesVector          << theListOfBuildingTypes.at(i);
        theBuildingOccupancyClassVector << theListOfOccupancyClasses.at(i);
        theBuildingCodeLevelVector      << CodeLevel;
        theBuildingLocationVector       << myLocation;
    } // i

    //qCritical() << this->objectName() << "generated" << theNumberOfBuildings << "buildings";

    return 1;
}

