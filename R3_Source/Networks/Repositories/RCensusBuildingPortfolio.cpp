#include "RCensusBuildingPortfolio.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RNormalDistribution.h"
#include "RPolygonalAreaSourceModel.h"
#include "RLocation.h"
#include <math.h>


RCensusBuildingPortfolio::RCensusBuildingPortfolio(QObject *parent, QString name)
    : RObject(parent, name)
{
    theLatitudeRandomVariable = nullptr;
    theNumberOfBuildings = 0;
    inputPercentageInsured = 0.0;
    theRetrofitLevel = 0.0;
    
    this->setMunicipality(QString("SanFrancisco"));
}


RCensusBuildingPortfolio::~RCensusBuildingPortfolio()
{
    
}


int RCensusBuildingPortfolio::resetTime()
{
    if (theLatitudeRandomVariable) {
        delete theLatitudeRandomVariable;
    }
    theLatitudeRandomVariable = nullptr;
    
    return 0;
}

void RCensusBuildingPortfolio::setMunicipality(QString value)
{
    inputMunicipality = value;
    
    if (inputMunicipality == QString("Vancouver")){
        medianMunicipalHouseholdIncome = 56908.0;
        insuranceTakeUp = 0.45;
        
    } else if (inputMunicipality == QString("SanFrancisco")){
        medianMunicipalHouseholdIncome = 93448.0;
        insuranceTakeUp = 0.15;
        
    } else {
        qCritical() << "The" << this->objectName() << "could not identify the municipality. Using default values for insurace take-up rate and building values!!";
        medianMunicipalHouseholdIncome = 50000.0;
        insuranceTakeUp = 0.25;
    }
}


QObject * RCensusBuildingPortfolio::getLocation() const
{
    return theLocation;
}

void RCensusBuildingPortfolio::setLocation(QObject *value)
{
    theLocation = qobject_cast<RParameter *>(value);
    
    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theLocation);
}


QString RCensusBuildingPortfolio::getHouseholdSizeDistribution()
{
    return doubleVectorToString(inputHouseholdSizeVector);
}

void RCensusBuildingPortfolio::setHouseholdSizeDistribution(QString value)
{
    inputHouseholdSizeVector = stringToDoubleVector(value);
}

double RCensusBuildingPortfolio::getMedianHouseholdIncome() const
{
    return theMedianHouseholdIncome;
}

void RCensusBuildingPortfolio::setMedianHouseholdIncome(double value)
{
    theMedianHouseholdIncome = value;
}

double RCensusBuildingPortfolio::getPercentageLowIncome() const
{
    return inputPercentageLowIncome;
}

void RCensusBuildingPortfolio::setPercentageLowIncome(double value)
{
    inputPercentageLowIncome = value;
}


double RCensusBuildingPortfolio::getPercentageModerateIncome() const
{
    return inputPercentageModerateIncome;
}

void RCensusBuildingPortfolio::setPercentageModerateIncome(double value)
{
    inputPercentageModerateIncome = value;
}


double RCensusBuildingPortfolio::getPercentageHighIncome() const
{
    return inputPercentageHighIncome;
}

void RCensusBuildingPortfolio::setPercentageHighIncome(double value)
{
    inputPercentageHighIncome = value;
}



double RCensusBuildingPortfolio::getPercentageOfChildren() const
{
    return inputPercentageOfChildren;
}

void RCensusBuildingPortfolio::setPercentageOfChildren(double value)
{
    inputPercentageOfChildren = value;
}

double RCensusBuildingPortfolio::getPercentageOfSeniors() const
{
    return  inputPercentageOfSeniors;
}

void RCensusBuildingPortfolio::setPercentageOfSeniors(double value)
{
    inputPercentageOfSeniors = value;
}

QString RCensusBuildingPortfolio::getAgeDistribution()
{
    return doubleVectorToString(inputAgeDistributionVector);
}

void RCensusBuildingPortfolio::setAgeDistribution(QString value)
{
    inputAgeDistributionVector = stringToDoubleVector(value);
}


double RCensusBuildingPortfolio::getPercentageOfRentedBuildings() const
{
    return inputPercentageOfRenters;
}

void RCensusBuildingPortfolio::setPercentageOfRentedBuildings(double value)
{
    inputPercentageOfRenters = value;
}


double RCensusBuildingPortfolio::getPercentageWithMortgage() const
{
    return inputPercentageWithMortgage;
}

void RCensusBuildingPortfolio::setPercentageWithMortgage(double value)
{
    inputPercentageWithMortgage = value;
}

double RCensusBuildingPortfolio::getPercentageOfRecentImmigrants() const
{
    return inputPercentageOfRecentImmigrants;
}

void RCensusBuildingPortfolio::setPercentageOfRecentImmigrants(double value)
{
    inputPercentageOfRecentImmigrants = value;
}


double RCensusBuildingPortfolio::getPercentageOfEstablishedImmigrants() const
{
    return inputPercentageOfEstablishedImmigrants;
}

void RCensusBuildingPortfolio::setPercentageOfEstablishedImmigrants(double value)
{
    inputPercentageOfEstablishedImmigrants = value;
}


double RCensusBuildingPortfolio::getPercentageUnaffordableDwellings() const
{
    return inputPercentageOfUnaffordableDwellings;
}

void RCensusBuildingPortfolio::setPercentageUnaffordableDwellings(double value)
{
    inputPercentageOfUnaffordableDwellings = value;
}

double RCensusBuildingPortfolio::getPercentageUnsafeDwellings() const
{
    return inputPercentageOfUnsafeDwellings;
}

void RCensusBuildingPortfolio::setPercentageUnsafeDwellings(double value)
{
    inputPercentageOfUnsafeDwellings = value;
}


double RCensusBuildingPortfolio::getPercentageNotSuitableDwellings() const
{
    return inputPercentageOfNotSuitableDwellings;
}

void RCensusBuildingPortfolio::setPercentageNotSuitableDwellings(double value)
{
    inputPercentageOfNotSuitableDwellings = value;
}


QString RCensusBuildingPortfolio::getMunicipality() const
{
    return inputMunicipality;
}

QString RCensusBuildingPortfolio::getBoundaryPointsLatitude() const
{
    return inputBoundaryPointsLatitudes;
}

void RCensusBuildingPortfolio::setBoundaryPointsLatitude(QString value)
{
    inputBoundaryPointsLatitudes = value;
}


QString RCensusBuildingPortfolio::getBoundaryPointsLongitude() const
{
    return inputBoundaryPointsLongitudes;
}

void RCensusBuildingPortfolio::setBoundaryPointsLongitude(QString value)
{
    inputBoundaryPointsLongitudes = value;
}


QVector<QVector<double>> RCensusBuildingPortfolio::getBoundariesVector()
{
    QVector<QVector<double>> returnVector;
    QVector<double> latitudeVector = stringToDoubleVector(inputBoundaryPointsLatitudes);
    QVector<double> longitudeVector = stringToDoubleVector(inputBoundaryPointsLongitudes);
    
    for (int i = 0; i < latitudeVector.size(); i++){
        QVector<double> theCoordinates;
        
        theCoordinates << latitudeVector.at(i) << longitudeVector.at(i);
        returnVector << theCoordinates;
    }
    
    return returnVector;
}

double RCensusBuildingPortfolio::getPercentagePre1940Buildings() const
{
    return inputPercentageOfPre1940Buildings;
}

void RCensusBuildingPortfolio::setPercentagePre1940Buildings(double value)
{
    inputPercentageOfPre1940Buildings = value;
}


double RCensusBuildingPortfolio::getPercentagePre1975Buildings() const
{
    return inputPercentageOfPre1975Buildings;
}

void RCensusBuildingPortfolio::setPercentagePre1975Buildings(double value)
{
    inputPercentageOfPre1975Buildings = value;
}


double RCensusBuildingPortfolio::getPercentagePost1975Buildings() const
{
    return inputPercentageOfPost1975Buildings;
}

void RCensusBuildingPortfolio::setPercentagePost1975Buildings(double value)
{
    inputPercentageOfPost1975Buildings = value;
}

QString RCensusBuildingPortfolio::getBuildingTypes()
{
    return inputBuildingTypes;
}

void RCensusBuildingPortfolio::setBuildingTypes(QString value)
{
    inputBuildingTypes = value;
}


QString RCensusBuildingPortfolio::getBuildingOccupancyClasses()
{
    return inputBuildingOccupancyClasses;
}

void RCensusBuildingPortfolio::setBuildingOccupancyClasses(QString value)
{
    inputBuildingOccupancyClasses = value;
}


QString RCensusBuildingPortfolio::getBuildingNumberPerType()
{
    return intVectorToString(inputDwellingsPerBuildingType);
}

void RCensusBuildingPortfolio::setBuildingNumberPerType(QString value)
{
    inputDwellingsPerBuildingType = stringToIntVector(value);
}


QString RCensusBuildingPortfolio::getBuildingAreas()
{
    return doubleVectorToString(inputBuildingAreas);
}

void RCensusBuildingPortfolio::setBuildingAreas(QString value)
{
    inputBuildingAreas = stringToDoubleVector(value);
}

QString RCensusBuildingPortfolio::getBuildingValues()
{
    return doubleVectorToString(inputBuildingValues);
}

void RCensusBuildingPortfolio::setBuildingValues(QString value)
{
    inputBuildingValues = stringToDoubleVector(value);
}

QString RCensusBuildingPortfolio::getBuildingDensity()
{
    return inputBuildingDensity;
}

void RCensusBuildingPortfolio::setBuildingDensity(QString value)
{
    inputBuildingDensity = value;
}

double RCensusBuildingPortfolio::getRetrofitLevel()
{
    return theRetrofitLevel;
}

void RCensusBuildingPortfolio::setRetrofitLevel(double value)
{
    theRetrofitLevel = value;
}

// Extractors
int RCensusBuildingPortfolio::getNumberOfBuildings() const
{
    return theNumberOfBuildings;
}


int RCensusBuildingPortfolio::getNumberOfSingleFamilyBuildings() const
{
    if (inputDwellingsPerBuildingType.size() > 0){
        return inputDwellingsPerBuildingType.at(0);

    } else {
        qCritical() << "There are no dwellings in this Census Tract!!";
        return 1;
    }
}


int RCensusBuildingPortfolio::getNumberOfMultiFamilyBuildings() const
{
    int sum = 0;
    for(int i = 0; i < inputDwellingsPerBuildingType.size() - 1; i++){
        sum += inputDwellingsPerBuildingType.at(i);
    }

    if (sum > 0){
        return sum;
    } else {
        qCritical() << "There are no dwellings in this Census Tract!!";
        return 1;
    }
}


QVector<QVector<bool>> RCensusBuildingPortfolio::getHouseholdInsuranceStatus() const
{
    return theBuildingInsuranceVector;
}


QVector<QString> RCensusBuildingPortfolio::getBuildingTenureVector() const
{
    return theBuildingTenureVector;
}


QVector<QVector<bool>> RCensusBuildingPortfolio::getMortgageVector() const
{
    return householdMortgageVector;
}


double RCensusBuildingPortfolio::getPercentageOfHouseholdsWithCars() const
{
    return inputPercentageWithCars;
}


void RCensusBuildingPortfolio::setPercentageOfHouseholdsWithCars(double value)
{
    inputPercentageWithCars = value;
}


QVector<QString> RCensusBuildingPortfolio::getBuildingArchetypeVector() const
{
    return theBuildingArchetypeVector;
}


QVector<QString> RCensusBuildingPortfolio::getBuildingTypesVector() const
{
    return theBuildingTypesVector;
}


QVector<QString> RCensusBuildingPortfolio::getBuildingCodeLevelVector() const
{
    return theBuildingCodeLevelVector;
}


QVector<QString> RCensusBuildingPortfolio::getBuildingOccupancyClassVector() const
{
    return theBuildingOccupancyClassVector;
}


QVector<int> RCensusBuildingPortfolio::getBuildingStoreysVector() const
{
    return theBuildingStoreysVector;
}


QVector<int> RCensusBuildingPortfolio::getBuildingDwellingsVector() const
{
    return theNumberOfDwellingsPerBuilding;
}


QVector<double> RCensusBuildingPortfolio::getBuildingAreaVector() const
{
    return theBuildingAreasVector;
}


QVector<double> RCensusBuildingPortfolio::getBuildingValueVector() const
{
    return theBuildingValueVector;
}


QVector<QPair<double, double>> RCensusBuildingPortfolio::getBuildingLocationVector() const
{
    return theBuildingLocationVector;
}


QVector<QVector<int>> RCensusBuildingPortfolio::getHouseholdSizeVector() const
{
    return householdSizeVector;
}


QVector<QVector<QString>> RCensusBuildingPortfolio::getHouseholdAgeCategoryVector() const
{
    return householdAgeCategory;
}

QVector<QVector<QString>> RCensusBuildingPortfolio::getHouseholdImmigrantStatusVector() const
{
    return householdImmigrantStatusVector;
}


QVector<QVector<bool>> RCensusBuildingPortfolio::getHouseholdCarOwnershipVector() const
{
    return householdCarOwnershipVector;
}


QVector<QVector<QString>> RCensusBuildingPortfolio::getHouseholdHouseholdIncomeVector() const
{
    return householdHouseholdIncomeVector;
}


QVector<QVector<QString>> RCensusBuildingPortfolio::getHouseholdTenureVector() const
{
    return householdTenureVector;
}


QVector<QVector<QString>> RCensusBuildingPortfolio::getHouseholdArchetypeVector() const
{
    return householdArchetypeVector;
}


QVector<QVector<bool>> RCensusBuildingPortfolio::getHouseholdPredispositionToLeave() const
{
    return householdPredispositionToLeave;
}

QPair<double,double> RCensusBuildingPortfolio::instantiateBuildingLocation()
{
    //        if (theLatitudeRandomVariable) {
    //            delete theLatitudeRandomVariable;
    //        }
    //        theLatitudeRandomVariable = 0;
    
    
    //        QVector<double> latitudeVector = stringToDoubleVector(inputBoundaryPointsLatitudes);
    //        QVector<double> longitudeVector = stringToDoubleVector(inputBoundaryPointsLongitudes);
    
    
    //        if (theLocationList.size() < latitudeVector.size()){
    //            for (int i = 0; i < latitudeVector.size(); i++){
    //                theDomain->createObject(objectName() + "BoundaryPoint" + QVariant(i).toString(), "RLocation");
    //                RLocation *myLocation = qobject_cast<RLocation *>(theDomain->getLastAddedObject());
    
    //                myLocation->setLatitude(latitudeVector.at(i));
    //                myLocation->setLongitude(longitudeVector.at(i));
    //                theLocationList << myLocation;
    //            }
    //        }
    
    
    //        if (!theLatitudeRandomVariable) {
    //            QMap<double, double> latLngMap;
    //            for (int i = 0; i < theLocationList.count(); i++) {
    //                latLngMap.insertMulti(theLocationList[i]->getLatitude(), theLocationList[i]->getLongitude());
    //            }
    
    //            QVector<double> latVector = latLngMap.uniqueKeys().toVector();
    //            QVector<double> PDFVector;
    //            for (int i = 0; i < latVector.count(); i++) {
    //                double lat = latVector[i];
    
    //                QVector<double> lngVector;
    
    //                for (int j = 0; j < theLocationList.count(); j++) {
    //                    // k is the next point
    //                    int k = j + 1;
    //                    if (k == theLocationList.count()) {
    //                        k = 0;
    //                    }
    //                    if ((theLocationList[j]->getLatitude() - lat) * (theLocationList[k]->getLatitude() - lat) < 0) {
    //                        // The line between j and k intersects with this latitude
    //                        double intersectionLng;
    //                        /*// TEMP....................................................................................................................................................................
    //                            double lng1 = theLocationList[j]->getLongitude();
    //                            double lng2 = theLocationList[k]->getLongitude();
    //                            double lat1 = theLocationList[j]->getLatitude();
    //                            double lat2 = theLocationList[k]->getLatitude();
    //                            intersectionLng = lng1 + (lng2 - lng1)/(lat2 - lat1) * (lat - lat1);*/
    
    
    
    //                        int result = theLocationList[j]->computeIntersectionWithLatitude(lat, theLocationList[k], &intersectionLng);
    //                        if (result < 0) {
    //                            qCritical() << "Error 2: Cannot compute the intersection in model" << objectName() << " Please check your coordinates...";
    //                        }
    //                        lngVector << intersectionLng;
    
    //                        // OR CALCULATE THE INTERSECTION USING A LINEAR RELATIONSHIP BETWEEN LAT AND LNG. THIS IS HOW WE HAVE EVALAUTE THE PDF OF LATITUDE AFTER ALL.
    
    //                    } else if (theLocationList[j]->getLatitude() == lat) {
    //                        // The point j iteself is on the latitude
    
    //                        // l is the previous point
    //                        int l = j - 1;
    //                        if (l == -1) {
    //                            l = theLocationList.count() - 1;
    //                        }
    
    //                        // Add the point j's longitude to the lngVector only if the the lines that go out of point j are not at the same side of the latitude
    //                        if ((theLocationList[l]->getLatitude() - lat) * (theLocationList[k]->getLatitude() - lat) <= 0) {
    //                            lngVector << theLocationList[j]->getLongitude();
    //                        }
    //                    }
    //                }
    
    //                // Terje
    //                if (remainder(lngVector.size(), 2.0) != 0.0) {
    //                    qCritical() << "Something is wrong in" << this->objectName() << " Please check your coordinates...";
    //                }
    
    
    //                qSort(lngVector.begin(), lngVector.end());
    
    //                double length = 0;
    //                for (int j = 0; j < lngVector.count(); j += 2) {
    //                    length += qAbs(lngVector[j+1] - lngVector[j]);
    //                }
    //                length *= cos(lat / 180 * 3.14159265359);
    
    //                PDFVector << length;
    //            }
    
    //            theLatitudeRandomVariable = new RContinuousRandomVariable(0, objectName() + "LatRV");
    //            theLatitudeRandomVariable->setDistributionType("User Defined");
    //            theLatitudeRandomVariable->setUserDefinedPDF(latVector, PDFVector);
    //        }
    
    
    //        double theta1 = rand()/static_cast<double>(RAND_MAX);
    //        double theta2 = rand()/static_cast<double>(RAND_MAX);
    //        double latitude = theLatitudeRandomVariable->getInverseCDFValue(theta1);
    //        bool isDifferentiable = true;
    
    //        QVector<double> lngVector;
    //        QVector<double> dLngVector;
    //        double topOrBottomMostLongitude;
    //        for (int i = 0; i < theLocationList.count(); i++) {
    //            int j = i + 1;
    //            if (j == theLocationList.count()) {
    //                j = 0;
    //            }
    //            if ((theLocationList[i]->getLatitude() - latitude) * (theLocationList[j]->getLatitude() - latitude) < 0) {
    //                double intersectionLng;
    //                int result = theLocationList[i]->computeIntersectionWithLatitude(latitude, theLocationList[j], &intersectionLng);
    //                if (result < 0) {
    //                    qCritical() << "Error 1: Cannot compute the intersection in model" << objectName() << " Please check your coordinates...";
    //                }
    //                lngVector << intersectionLng;
    
    //            } else if (theLocationList[i]->getLatitude() == latitude) {
    //                // The point i iteself is on the latitude
    
    //                // k is the previous point
    //                int k = i - 1;
    //                if (k == -1) {
    //                    k = theLocationList.count() - 1;
    //                }
    
    //                // Add the point i's longitude to the lngVector only if the the lines that go out of point j are not at the same side of the latitude
    //                if ((theLocationList[k]->getLatitude() - latitude) * (theLocationList[j]->getLatitude() - latitude) <= 0) {
    //                    lngVector << theLocationList[i]->getLongitude();
    //                } else {
    //                    topOrBottomMostLongitude = theLocationList[i]->getLongitude();
    //                }
    //            }
    //        }
    
    
    //        QVector<double> lngVectorBackup = lngVector;
    //        qSort(lngVector.begin(), lngVector.end());
    
    //        double deltaLng = 0;
    //        for (int i = 0; i < lngVector.count(); i += 2) {
    //            deltaLng += qAbs(lngVector[i+1] - lngVector[i]);
    //        }
    
    
    //        double dLng_dLat = 0;
    //        if (dLngVector.count() != 2) {
    //            isDifferentiable = false;
    //        } else {
    
    //        }
    
    
    //        double lngPortion = deltaLng * theta2;
    //        double lngSoFar = 0;
    //        double longitude;
    //        for (int i = 0; i < lngVector.count(); i += 2) {
    //            lngSoFar += qAbs(lngVector[i+1] - lngVector[i]);
    //            if (lngSoFar >= lngPortion) {
    //                longitude = lngVector[i + 1] - (lngSoFar - lngPortion);
    //                break;
    //            }
    //        }
    
    //        // If the point is topmost ot bottommost point
    //        if (lngVector.isEmpty()) {
    //            longitude = topOrBottomMostLongitude;
    //        }
    
    
    //        return qMakePair(latitude, longitude);
    return qMakePair(parameterToLocation(theLocation)->getLatitude(), parameterToLocation(theLocation)->getLongitude());
}





int RCensusBuildingPortfolio::instantiateBuildings()
{
    // owner
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
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
        qDebug() << "Instantiating owners and owners at" << this->objectName() << "at time" << theCurrentTime->getTime();
    }
    
    // Get the list of buildings from the input file
    QStringList theListOfBuildingTypes = inputBuildingTypes.split(QRegularExpression("\\W+"), QString::SkipEmptyParts);
    QStringList theListOfOccupancyClasses = inputBuildingOccupancyClasses.split(QRegularExpression("\\W+"), QString::SkipEmptyParts);
    QVector<int> theNumberOfInstancesPerBuildingType;
    
    // For each building type, generate the buildings
    for (int i = 0; i < theListOfBuildingTypes.size(); i++){
        
        theNumberOfInstancesPerBuildingType << 0;
        int theNumberOfDwellingsToBeAllocated = inputDwellingsPerBuildingType.at(i);
        
        while (theNumberOfDwellingsToBeAllocated > 0){
            
            // Number of stories
            double rngNumberOfStories = rand()/static_cast<double>(RAND_MAX);
            
            int myNumberOfStories = 0;
            
            // For low-rise buildings, L
            if (theListOfBuildingTypes.at(i).contains("L")){
                myNumberOfStories = 4;
                
                // For medium-rise buildings, M
            } else if (theListOfBuildingTypes.at(i).contains("M")){
                myNumberOfStories = qRound(rngNumberOfStories * 5) + 5;
                
                // For high-rise buildings, H
            } else if (theListOfBuildingTypes.at(i).contains("H")){
                myNumberOfStories = qRound(rngNumberOfStories * 10) + 10;
                
                // For wood-frame buildings, W
            } else if (theListOfBuildingTypes.at(i).contains("W")){
                myNumberOfStories = 2;
                
            } else {
                qCritical() << this->objectName() << "could not identify the building type for this building and cannot instantiate the number of floors";
            }



            // Number of dwellings per floor
            double rngUnitsPerFloor = rand()/static_cast<double>(RAND_MAX);
            int myNumberOfUnitsPerFloor;
            
            if (theListOfOccupancyClasses.at(i) == QString("RES3")){
                if (myNumberOfStories < 7){
                    myNumberOfUnitsPerFloor = qRound(rngUnitsPerFloor * 6) + 4;
                    
                } else {
                    myNumberOfUnitsPerFloor = qRound(rngUnitsPerFloor * 6) + 4;
                }
                
            } else {
                myNumberOfUnitsPerFloor = 1;
            }
            
            int myNumberOfDwellings = 1;
            if (theListOfOccupancyClasses.at(i) == QString("RES3")){
                myNumberOfDwellings = qMin(myNumberOfStories * myNumberOfUnitsPerFloor,theNumberOfDwellingsToBeAllocated);
            }
            
            
            // Building area
            double myBuildingArea = 0.0;
            for (int j = 0; j < myNumberOfDwellings; j++){
                double rngForArea = rand()/static_cast<double>(RAND_MAX);
                myBuildingArea += inputBuildingAreas.at(i) - 500.0 + rngForArea * 1000.0;
            }
            
            
            // Building value
            double myBuildingValue = 0.0;
            
            for (int j = 0; j < myNumberOfDwellings; j++){
                double rngForValue = rand()/static_cast<double>(RAND_MAX);
                
                if (theListOfBuildingTypes.at(i) == "W1"){
                    myBuildingValue = (0.75 + 0.5 * rngForValue) * inputBuildingValues.at(i) * theMedianHouseholdIncome / medianMunicipalHouseholdIncome;
                    
                } else if (theListOfBuildingTypes.at(i) == "C2L"){
                    myBuildingValue = (0.75 + 0.5 * rngForValue) * inputBuildingValues.at(i) * theMedianHouseholdIncome / medianMunicipalHouseholdIncome;
                    
                } else if (theListOfBuildingTypes.at(i) == "C1M"){
                    myBuildingValue = (0.75 + 0.5 * rngForValue) * inputBuildingValues.at(i) * theMedianHouseholdIncome / medianMunicipalHouseholdIncome;
                    
                } else if (theListOfBuildingTypes.at(i) == "C2H"){
                    myBuildingValue = (0.75 + 0.5 * rngForValue) * inputBuildingValues.at(i) * theMedianHouseholdIncome / medianMunicipalHouseholdIncome;
                    
                } else if (theListOfBuildingTypes.at(i) == "MH"){
                    myBuildingValue = (0.75 + 0.5 * rngForValue) * inputBuildingValues.at(i) * theMedianHouseholdIncome / medianMunicipalHouseholdIncome;
                    
                } else if (theListOfBuildingTypes.at(i) == "URM"){
                    myBuildingValue += 0.0;
                    qCritical() << "Something went wrong when defining the building value in" << this->objectName() << theListOfBuildingTypes.at(i);
                    
                } else {
                    qCritical() << "Something went wrong when defining the building value in" << this->objectName() << theListOfBuildingTypes.at(i);
                }
            }
            
            theNumberOfBuildings += 1;
            theNumberOfInstancesPerBuildingType[i] += 1;
            theBuildingStoreysVector   << myNumberOfStories;
            theNumberOfDwellingsPerBuilding << myNumberOfDwellings;
            theBuildingAreasVector     << myBuildingArea;
            theBuildingValueVector     << myBuildingValue;
            theNumberOfDwellingsToBeAllocated -= myNumberOfDwellings;
        } // while
    } // for i
    
    
    // For building types
    for (int i = 0; i < theListOfBuildingTypes.size(); i++){
        for (int j = 0; j < theNumberOfInstancesPerBuildingType.at(i); j++){

            // Building Code Design Levels
            QString CodeLevel;

            double rngCodeLevel = rand()/static_cast<double>(RAND_MAX);

            if (inputMunicipality == QString("SanFrancisco")){
                // San Francisco is on UBC's Seismic Zone 4/FEMA zone 9 as per Kiremidjian

                // For wood-frame buildings
                if (theListOfBuildingTypes.at(i).contains("W")){
                    if (rngCodeLevel <= inputPercentageOfPre1940Buildings){
                        CodeLevel = QString("Moderate");
                    }
                    else if (rngCodeLevel > inputPercentageOfPre1940Buildings && rngCodeLevel <= inputPercentageOfPre1940Buildings + inputPercentageOfPre1975Buildings){
                        CodeLevel = QString("Moderate");
                    }
                    else if (rngCodeLevel > inputPercentageOfPre1940Buildings + inputPercentageOfPre1975Buildings){
                        CodeLevel = QString("High");
                    }
                    else {
                        qCritical() << this->objectName() << "could not identify the code level for building type" << theListOfBuildingTypes.at(i);
                    }

                    // For other buildings
                } else {
                    if (rngCodeLevel <= inputPercentageOfPre1940Buildings){
                        CodeLevel = QString("Pre");
                    }
                    else if (rngCodeLevel > inputPercentageOfPre1940Buildings && rngCodeLevel <= inputPercentageOfPre1940Buildings + inputPercentageOfPre1975Buildings){
                        CodeLevel = QString("Moderate");
                    }
                    else if (rngCodeLevel > inputPercentageOfPre1940Buildings + inputPercentageOfPre1975Buildings){
                        CodeLevel = QString("High");
                    }
                    else {
                        qCritical() << this->objectName() << "could not identify the code level for building type" << theListOfBuildingTypes.at(i);
                    }
                }
            }

            theBuildingTypesVector << theListOfBuildingTypes.at(i);
            theBuildingOccupancyClassVector << theListOfOccupancyClasses.at(i);
            theBuildingCodeLevelVector << CodeLevel;
            theBuildingLocationVector   << this->instantiateBuildingLocation();
        }
    }
    return 1;
}



