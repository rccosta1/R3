#include "RHAZUSBuildingPortfolio.h"
#include "RParameter.h"
#include "RNormalDistribution.h"
#include "RLocation.h"
#include <math.h>
#include <QtMath>
#include <QRegularExpression>
#include <QString>

RHAZUSBuildingPortfolio::RHAZUSBuildingPortfolio(QObject *parent, QString name)
    : RBuildingPortfolio(parent, name)
{
    theLatitudeRandomVariable = nullptr;
    theNumberOfBuildings = 0;
    theRetrofitLevel = 0.0;
    
    this->setMunicipality(QString("SanFrancisco"));
}


RHAZUSBuildingPortfolio::~RHAZUSBuildingPortfolio()
{
    
}


int RHAZUSBuildingPortfolio::resetTime()
{
    if (theLatitudeRandomVariable) {
        delete theLatitudeRandomVariable;
    }
    theLatitudeRandomVariable = nullptr;
    
    return 0;
}



QPair<double,double> RHAZUSBuildingPortfolio::instantiateBuildingLocation()
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





int RHAZUSBuildingPortfolio::instantiateBuildings()
{
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
    QStringList theListOfCodeLevels = inputBuildingCodeLevels.split(QRegularExpression("\\W+"), QString::SkipEmptyParts);
    QVector<int> theListOfNumberPerType = stringToIntVector(inputNumberPerBuildingType);


    // ************************************
    // Ad-hoc
    //qCritical() << this->objectName() << "is removing all non W1-RES1 buildings from the simulation for the ICOSSAR paper.";

    QStringList auxListOfBuildingTypes = theListOfBuildingTypes;
    for (int i = 0; i < auxListOfBuildingTypes.size(); i++){
        int ii = auxListOfBuildingTypes.size() - 1 - i;

        if (!(theListOfBuildingTypes.at(ii).contains("W"))){
            theListOfBuildingTypes.removeAt(ii);
            theListOfOccupancyClasses.removeAt(ii);
            theListOfCodeLevels.removeAt(ii);
            theListOfNumberPerType.removeAt(ii);
        }
    }

    for (int i = 0; i < theListOfBuildingTypes.size(); i++){
        theListOfOccupancyClasses[i] = QString("RES1");
    }

    //    qCritical() << "Changing the maximum number of buildings in this census tract to 100!";
    //    for (int i = 0; i < theListOfBuildingTypes.size(); i++){
    //        theListOfNumberPerType[i] *= 3.0;
    //    }

    // ************************************


    // For each building type, generate the buildings
    for (int i = 0; i < theListOfBuildingTypes.size(); i++){
        for (int j = 0; j < theListOfNumberPerType.at(i); j++){

            // Number of stories
            double rngNumberOfStories = rand()/static_cast<double>(RAND_MAX);

            int myNumberOfStories = 0;

            // For low-rise buildings, L
            if (theListOfBuildingTypes.at(i).contains("L")){
                myNumberOfStories = qRound(rngNumberOfStories * 2) + 2;

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


            int myNumberOfDwellings = 1;
            if (theListOfOccupancyClasses.at(i) == QString("RES3A")){
                myNumberOfDwellings = 2;

            } else if (theListOfOccupancyClasses.at(i) == QString("RES3B")){
                myNumberOfDwellings = 3 + qRound(rand()/static_cast<double>(RAND_MAX));

            } else if (theListOfOccupancyClasses.at(i) == QString("RES3C")){
                myNumberOfDwellings = 5 + qRound(4 * rand()/static_cast<double>(RAND_MAX));

            } else if (theListOfOccupancyClasses.at(i) == QString("RES3D")){
                myNumberOfDwellings = 10 + qRound(9 * rand()/static_cast<double>(RAND_MAX));

            } else if (theListOfOccupancyClasses.at(i) == QString("RES3E")){
                myNumberOfDwellings = 20 + qRound(29 * rand()/static_cast<double>(RAND_MAX));

            } else if (theListOfOccupancyClasses.at(i) == QString("RES3F")){
                myNumberOfDwellings = 50 + qRound(20 * rand()/static_cast<double>(RAND_MAX));
            }


            // Number of rooms
            double rngRooms = rand() / static_cast<double>(RAND_MAX);
            double lowerLimit = 0.00001;
            double higherLimit = 0.00002;
            QVector<double> theVectorOfNumberOfRooms;
            theVectorOfNumberOfRooms << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
            double myNumberOfRooms = 0.0;


            for (int k = 0; k < inputRoomsDistributionVector.size(); k++){
                higherLimit += inputRoomsDistributionVector.at(k);

                if ((rngRooms - lowerLimit) * (rngRooms - higherLimit) < 0.0){
                    myNumberOfRooms = theVectorOfNumberOfRooms.at(k);
                }
                lowerLimit += inputRoomsDistributionVector.at(k);
            }


            // Is there any way to attach the building area to the occupancy?
            // Building area
            double myBuildingArea = 0.0;
            for (int k = 0; k < myNumberOfDwellings; k++){

                double rngForArea = rand()/static_cast<double>(RAND_MAX);

                if (theListOfOccupancyClasses.at(i) == QString("RES1")){
                    myBuildingArea += (3200.0 - 500.0 + rngForArea * 1000.0);//inputBuildingAreas.at(i) - 500.0 + rngForArea * 1000.0;
                } else {
                    myBuildingArea += 1.0; //https://www.propertyshark.com/Real-Estate-Reports/size-of-new-built-homes-in-the-us?fbclid=IwAR3g3R6GNBsEWn7puPsr2ah6kPnoB7FNfK5CeOPYx4aEqM3thsFUxzbOR9o
                }
            }


            // Are there any data sources to attach building cost per area to occupancy?
            // Building value
            double myPropertyValue = 0.0;
            double myLandValue = 0.0;
            double myBuildingValue = 0.0;

            for (int k = 0; k < myNumberOfDwellings; k++){
                double rngForValue = rand()/static_cast<double>(RAND_MAX) * qSqrt(myNumberOfRooms);

                if (theListOfOccupancyClasses.at(i) == QString("RES1")){
                    myPropertyValue = myNumberOfDwellings * ((101.0 - qPow(myNumberOfDwellings,0.5)) / 100.0) * 1.0 * (0.75 + 0.5 * rngForValue) * inputBuildingValues ;

                } else if (theListOfOccupancyClasses.at(i) == QString("RES3A")){
                    myPropertyValue = myNumberOfDwellings * ((101.0 - qPow(myNumberOfDwellings,0.5)) / 100.0) * 1.0 *  (0.75 + 0.5 * rngForValue) * inputBuildingValues ;

                } else if (theListOfOccupancyClasses.at(i) == QString("RES3B")){
                    myPropertyValue = myNumberOfDwellings * ((101.0 - qPow(myNumberOfDwellings,0.5)) / 100.0) * 1.0 *  (0.75 + 0.5 * rngForValue) * inputBuildingValues ;

                } else if (theListOfOccupancyClasses.at(i) == QString("RES3C")){
                    myPropertyValue = myNumberOfDwellings * ((101.0 - qPow(myNumberOfDwellings,0.5)) / 100.0) * 1.0 *  (0.75 + 0.5 * rngForValue) * inputBuildingValues ;

                } else if (theListOfOccupancyClasses.at(i) == QString("RES3D")){
                    myPropertyValue = myNumberOfDwellings * ((101.0 - qPow(myNumberOfDwellings,0.5)) / 100.0) * 1.0 *  (0.75 + 0.5 * rngForValue) * inputBuildingValues ;

                } else if (theListOfOccupancyClasses.at(i) == QString("RES3E")){
                    myPropertyValue = myNumberOfDwellings * ((101.0 - qPow(myNumberOfDwellings,0.5)) / 100.0) * 1.0 *  (0.75 + 0.5 * rngForValue) * inputBuildingValues ;

                } else if (theListOfOccupancyClasses.at(i) == QString("RES3F")){
                    myPropertyValue = myNumberOfDwellings * ((101.0 - qPow(myNumberOfDwellings,0.5)) / 100.0) * 1.0 *  (0.75 + 0.5 * rngForValue) * inputBuildingValues ;
                }

                // Consider that the building value is half of the property value, which is what we have from census
                myBuildingValue = 0.5 * myPropertyValue;
                myLandValue = 0.5 * myPropertyValue;

                //qCritical() << "The value of this building is" << myBuildingValue << "while the average value for this CT is" << inputBuildingValues;

            }// k


            // Building Code Design Levels
            QString myCodeLevel;
            int myYearBuilt;

            double rngCodeLevel = rand()/static_cast<double>(RAND_MAX);

            if (theListOfCodeLevels.size() > 0){
                myCodeLevel = theListOfCodeLevels.at(i);

                if (theListOfBuildingTypes.at(i) == "C3L"){
                    myCodeLevel = QString("Low"); // C3L moderate exists in the database I got from Maryia but it does not appear in HAZUS
                }
            }


            else if (inputMunicipality == QString("SanFrancisco")){
                // San Francisco is on UBC's Seismic Zone 4/FEMA zone 9 as per Kiremidjian



                // For wood-frame buildings
                if (theListOfBuildingTypes.at(i).contains("W")){
                    if (rngCodeLevel <= inputPercentageOfPre1940Buildings){
                        myCodeLevel = QString("Moderate");
                        myYearBuilt = 1940;
                    }
                    else if (rngCodeLevel > inputPercentageOfPre1940Buildings && rngCodeLevel <= inputPercentageOfPre1940Buildings + inputPercentageOfPre1975Buildings){
                        myCodeLevel = QString("Moderate");
                        myYearBuilt = 1980;
                    }
                    else if (rngCodeLevel > inputPercentageOfPre1940Buildings + inputPercentageOfPre1975Buildings){
                        myCodeLevel = QString("High");
                        myYearBuilt = 1990;
                    }
                    else {
                        qCritical() << this->objectName() << "could not identify the code level for building type" << theListOfBuildingTypes.at(i);
                    }

                    // For other buildings
                } else {
                    if (rngCodeLevel <= inputPercentageOfPre1940Buildings){
                        myCodeLevel = QString("Pre");
                        myYearBuilt = 1940;
                    }
                    else if (rngCodeLevel > inputPercentageOfPre1940Buildings && rngCodeLevel <= inputPercentageOfPre1940Buildings + inputPercentageOfPre1975Buildings){
                        myCodeLevel = QString("Moderate");
                        myYearBuilt = 1980;
                    }
                    else if (rngCodeLevel > inputPercentageOfPre1940Buildings + inputPercentageOfPre1975Buildings){
                        myCodeLevel = QString("High");
                        myYearBuilt = 1990;
                    }
                    else {
                        qCritical() << this->objectName() << "could not identify the code level for building type" << theListOfBuildingTypes.at(i);
                    }
                }
            }
            if (theRetrofitLevel == 1.0){
                myCodeLevel = QString("High");
                myYearBuilt = 2020;
            }

            theNumberOfBuildings += 1;
            theBuildingStoreysVector        << myNumberOfStories;
            theNumberOfDwellingsPerBuilding << myNumberOfDwellings;
            theBuildingAreasVector          << myBuildingArea;
            theBuildingLandValueVector      << myLandValue;
            theBuildingValueVector          << myBuildingValue;
            theBuildingTypesVector          << theListOfBuildingTypes.at(i);
            theBuildingOccupancyClassVector << theListOfOccupancyClasses.at(i);
            theBuildingCodeLevelVector      << myCodeLevel;
            theBuildingYearBuiltVector      << myYearBuilt;
            theBuildingNumberOfRoomsVector  << myNumberOfRooms;
            //theBuildingLocationVector       << this->instantiateBuildingLocation();
        } // j
    } // i

    return 1;
}

