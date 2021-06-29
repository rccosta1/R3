#include "RNetworkManagerModel.h"
#include "RParameter.h"
#include "RTime.h"
#include <QMap>
#include <QCoreApplication>
#include <QDateTime>
#include "RMainWindow.h"
#include "RDomain.h"
#include "RContinuousRandomVariable.h"
#include "RResponse.h"
#include "RModel.h"
#include "RLocation.h"
#include "math.h"
#include <QMessageBox>
#include <QEventLoop>
#include <math.h>
#include <QDateTime>
#include "RTime.h"
#include "ROccurrenceModel.h"
#include "RBoundedExponentialMagnitudeModel.h"
/*
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <QNetworkReply>
*/
#include "RHouseholdPortfolio.h"
#include "RNetworkModel.h"
#include "RPacket.h"
#include "RPacketResponse.h"
#include "RResidentialMobilityNeighborhoodModel.h"
#include "RWorkforceStudyNeighborhoodModel.h"
#include "RRecoveryStrategyStudyNeighborhoodModel.h"
#include "RNormalDistribution.h"
#include "RBooreAtkinson2008IntensityModel.h"
#include "RContractorNegotiationModel.h"

#ifdef GIS
#include "RGISWidget.h"
#include "RPowerLineModel.h"
#include "RPowerSubstationModel.h"
#include "RWaterPipelineModel.h"
#include "RWaterTreatmentFacilityModel.h"
#include "RWaterPumpStationModel.h"
#include "RWaterStorageTankModel.h"
#include "RRoadwayModel.h"
#endif


RNetworkManagerModel::RNetworkManagerModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    myCounter = 0;
    timeOfLastPlot = 0;
    customPlot = nullptr;
    customPlotDisplaced = nullptr;
    customPlotResidentialMobility = nullptr;
    theMinimumRecoveryValue = 9999999.0;
    theMaximumRecoveryValue = 0.0;
    theMaxTimeGap = 365.0;

    theEventOccurrence = false;
    aModelfinishedRepairs = false;
    isOrchestratingModel = true;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}


RNetworkManagerModel::~RNetworkManagerModel()
{

}


int RNetworkManagerModel::resetTime()
{
    RModelManager theModelManager(theDomain);
    theModelManager.setPathMatrix(originalPathMatrix);

    theEventOccurrence = false;
    aModelfinishedRepairs = false;

    totalNumberOfPersonsSlightlyInjured = 0;
    totalNumberOfPersonsModeratelyInjured = 0;
    totalNumberOfPersonsSeverelyInjured = 0;

    lastEvaluationTime = 0.0;
    timeSinceLastPlot = 1000.0;
    tickVector.clear();
    labelVector.clear();
    tickVector << 0;
    labelVector << QString("%1").arg(0);

    return 0;
}


QStringList RNetworkManagerModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator"  << "&Display GIS Map" << "Separator" << "&Display Census Tracts On Map" << "Separator" << "&Display Buildings On Map" << "Separator" << "&Display Old Buildings On Map" << "Separator" << "&Display Single Family Dwellings On Map" << "Separator" << "&Display Multi Family Dwellings On Map" << "Separator" << "&Display Renters On Map"  << "Separator" << "&Display Recent Immigrants On Map" << "Separator"  << "&Display Locations On Map" << "Separator" << "& Display Network Links On Map" << "Separator" <<"&Help";
    return actionList;
}



QString RNetworkManagerModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}


void RNetworkManagerModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}

double RNetworkManagerModel::getMaxTimeGap()
{
    return theMaxTimeGap;
}

void RNetworkManagerModel::setMaxTimeGap(double value)
{
    theMaxTimeGap = value;
}


// ******** Maps and Diagrams ********
void RNetworkManagerModel::displayGISMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());

#ifdef GIS
    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

#else
    qCritical() << "The GIS map is not included in the Rts version you are using";
#endif
}


#ifdef GIS
void RNetworkManagerModel::displayLocationsOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {
        // Get a list of all the models
        QList<RNetworkModel *> modelList = theDomain->findChildren<RNetworkModel *>();
        RLocation *theLocationsAsPointer;

        for (int i = 0; i < modelList.size(); i++){

            // Get the location of the model
            theLocationsAsPointer = qobject_cast<RLocation *>(modelList.at(i)->getLocation());
            QVector<double> theCoordinates;

            // If the model has a location, pass it to the map widget
            if (theLocationsAsPointer){

                theCoordinates << theLocationsAsPointer->getLatitude() << theLocationsAsPointer->getLongitude();

                if(!(qobject_cast<RWaterPipelineModel*>(modelList.at(i)))){
                    theGISMapWidget->createPoint(theCoordinates, modelList.at(i));
                }
            }
        }
    }
}

void RNetworkManagerModel::displayBuildingsOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {

        // Get a list of all the models
        QList<RNeighborhoodModel *> modelList = theDomain->findChildren<RNeighborhoodModel *>();
        QVector<RLocation *> theLocations;
        for (int i = 0; i < modelList.size(); i++){

            // Get the location of the model
            theLocations = modelList.at(i)->getBuildingLocations();

            // If the model has a location, pass it to the map widget
            for (int j = 0; j < theLocations.size(); j++){
                QVector<double> theCoordinates;
                qCritical() << "Ploting a building at coordinates" << theLocations.at(j)->getLatitude() << theLocations.at(j)->getLongitude();
                theCoordinates << theLocations.at(j)->getLatitude() << theLocations.at(j)->getLongitude();
                theGISMapWidget->createPoint(theCoordinates, modelList.at(i));
            }
        }
    }
}


void RNetworkManagerModel::displayNetworkOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {
        // Get a list of all models
        QList<RNetworkModel *> listOfModels = theDomain->findChildren<RNetworkModel *>();

        for (int i = 0; i < listOfModels.size(); i++){

            // Creating the vector to receive the coordinates
            QVector<QVector<double>> coordinates;

            // Getting the location of the first model
            RLocation *theLocationOftheFirstModel = qobject_cast<RLocation *>(listOfModels.at(i)->getLocation());

            if (theLocationOftheFirstModel){

                // Passing the coordinates of the first model to the vector that will create the polyline
                QVector<double> theCoordinatesOfTheFirstModel;
                theCoordinatesOfTheFirstModel << theLocationOftheFirstModel->getLatitude() << theLocationOftheFirstModel->getLongitude();
                coordinates << theCoordinatesOfTheFirstModel;

                // Getting hold of all the responses coming in to this model
                QList<QPointer<RParameter> > theResponseList;// = stringToParameterList(listOfModels.at(i)->getInputParameterList());

                for (int j = 0; j < listOfModels.at(i)->metaObject()->propertyCount(); j++) {
                    // Getting the meta property
                    QMetaProperty metaProperty = listOfModels.at(i)->metaObject()->property(j);

                    // Getting the tpe of the meta property
                    QVariant::Type type = metaProperty.type();

                    // Getting the name of the property
                    const char * charName = metaProperty.name();

                    if (type == QMetaType::QString){
                        if(QString(charName).contains("List")){

                            QString theString = charName;


                            theResponseList << stringToParameterList(theString);

                            for (int k = 0; k < theResponseList.size(); k++){

                                // Cast the response parameter as a response object
                                RResponse *theResponseObject = qobject_cast<RResponse *>(theResponseList.at(k));

                                // Get the model of the response object as a network model
                                RNetworkModel *theSecondModel = qobject_cast<RNetworkModel *>(theResponseObject->getModel());

                                // Get the location of the second model
                                RLocation *theLocationOfTheSecondModel = qobject_cast<RLocation *>(theSecondModel->getLocation());

                                // Pass the coordinates of the second model to the vector that will create the polyline
                                QVector<double> theCoordinatesOfTheSecondModel;
                                theCoordinatesOfTheSecondModel << theLocationOfTheSecondModel->getLatitude() << theLocationOfTheSecondModel->getLongitude();
                                coordinates << theCoordinatesOfTheSecondModel;

                                // Tell the Main Window to create the link
                                theGISMapWidget->createPolyline(coordinates);

                                // Clear the second model coordinates from the coordinates vector
                                coordinates.removeAt(1);
                            } // for k
                        } // if contains
                    } // if string
                } // for j
            }
        }
    }
}


void RNetworkManagerModel::displayNetworkLinksOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());

    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {

        // Power lines
        QList<RPowerSubstationModel *> listOfSubstations = theDomain->findChildren<RPowerSubstationModel *>();

        for (int i = 0; i < listOfSubstations.size(); i++){

            // get the location of this model
            RLocation* fromLocation = qobject_cast<RLocation*>(listOfSubstations.at(i)->getLocation());
            QVector<double> theFromLocationCoordinates;
            theFromLocationCoordinates << fromLocation->getLatitude() << fromLocation->getLongitude();

            QList<QPointer<RParameter>> listOfInputs = stringToParameterList(listOfSubstations.at(i)->getPowerSupplierList());

            for(int j = 0; j < listOfInputs.size(); j ++){
                RNetworkModel* thisModel = qobject_cast<RNetworkModel*>(qobject_cast<RResponse*>(listOfInputs.at(j))->getModel());
                RLocation* toLocation = qobject_cast<RLocation*>(thisModel->getLocation());
                QVector<double> theToLocationCoordinates;
                theToLocationCoordinates << toLocation->getLatitude() << toLocation->getLongitude();

                // Creating the vector to receive the coordinates
                QVector<QVector<double>> coordinates;
                coordinates << theFromLocationCoordinates << theToLocationCoordinates;

                // Create the polyline
                theGISMapWidget->createPolyline(coordinates, listOfSubstations.at(i));
            }
        }

        QList<RWaterTreatmentFacilityModel*> listOfTreatmentFacilities = theDomain->findChildren<RWaterTreatmentFacilityModel*>();
        QList<RWaterPumpStationModel*> listOfPumpStations = theDomain->findChildren<RWaterPumpStationModel*>();

        for (int i = 0; i < listOfTreatmentFacilities.size(); i++){

            // get the location of this model
            RLocation* fromLocation = qobject_cast<RLocation*>(listOfTreatmentFacilities.at(i)->getLocation());
            QVector<double> theFromLocationCoordinates;
            theFromLocationCoordinates << fromLocation->getLatitude() << fromLocation->getLongitude();

            // Get the list of models that provides power to this model
            QList<QPointer<RParameter>> listOfPowerSupplierResponses = stringToParameterList(listOfTreatmentFacilities.at(i)->getPowerSupplierList());

            for (int j = 0; j < listOfPowerSupplierResponses.size(); j++){
                RNetworkModel* thePowerModel = qobject_cast<RNetworkModel*>(qobject_cast<RResponse*>(listOfPowerSupplierResponses.at(j))->getModel());

                if (thePowerModel){
                    RLocation* toLocation = qobject_cast<RLocation*>(thePowerModel->getLocation());
                    QVector<double> theToLocationCoordinates;
                    theToLocationCoordinates << toLocation->getLatitude() << toLocation->getLongitude();

                    // Creating the vector to receive the coordinates
                    QVector<QVector<double>> coordinates;
                    coordinates << theFromLocationCoordinates << theToLocationCoordinates;

                    // Create the polyline
                    theGISMapWidget->createPolyline(coordinates, listOfSubstations.at(i));
                }
            }
        }

        for (int i = 0; i < listOfPumpStations.size(); i++){

            // get the location of this model
            RLocation* fromLocation = qobject_cast<RLocation*>(listOfPumpStations.at(i)->getLocation());
            QVector<double> theFromLocationCoordinates;
            theFromLocationCoordinates << fromLocation->getLatitude() << fromLocation->getLongitude();

            // Get the list of models that provides power to this model
            QList<QPointer<RParameter>> listOfPowerSupplierResponses = stringToParameterList(listOfPumpStations.at(i)->getPowerSupplierList());

            for (int j = 0; j < listOfPowerSupplierResponses.size(); j++){
                RNetworkModel* thePowerModel = qobject_cast<RNetworkModel*>(qobject_cast<RResponse*>(listOfPowerSupplierResponses.at(j))->getModel());
                if (thePowerModel){
                    RLocation* toLocation = qobject_cast<RLocation*>(thePowerModel->getLocation());
                    QVector<double> theToLocationCoordinates;
                    theToLocationCoordinates << toLocation->getLatitude() << toLocation->getLongitude();

                    // Creating the vector to receive the coordinates
                    QVector<QVector<double>> coordinates;
                    coordinates << theFromLocationCoordinates << theToLocationCoordinates;

                    // Create the polyline
                    theGISMapWidget->createPolyline(coordinates, listOfSubstations.at(i));
                }
            }
        }

        // Water pipelines
        // Get a list of all models
        QList<RWaterPipelineModel *> listOfPipelines = theDomain->findChildren<RWaterPipelineModel *>();

        for (int i = 0; i < listOfPipelines.size(); i++){

            // Get the location 'from' location
            RLocation *theFromLocation = qobject_cast<RLocation *>(listOfPipelines.at(i)->getFromLocation());
            QVector<double> theFromLocationCoordinates;
            theFromLocationCoordinates << theFromLocation->getLatitude() << theFromLocation->getLongitude();

            // Get the location 'to' location
            RLocation *theToLocation = qobject_cast<RLocation *>(listOfPipelines.at(i)->getToLocation());
            QVector<double> theToLocationCoordinates;
            theToLocationCoordinates << theToLocation->getLatitude() << theToLocation->getLongitude();


            // Creating the vector to receive the coordinates
            QVector<QVector<double>> coordinates;

            coordinates << theFromLocationCoordinates << theToLocationCoordinates;

            // Create the polyline
            theGISMapWidget->createPolyline(coordinates, listOfPipelines.at(i));
        } // Water



        // Get a list of all models
        QList<RRoadwayModel *> listOfRoads = theDomain->findChildren<RRoadwayModel *>();

        for (int i = 0; i < listOfRoads.size(); i++){

            //            // Get the location 'from' location
            //            RLocation *theFromLocation = qobject_cast<RLocation *>(listOfRoads.at(i)->getFromLocation());
            //            QVector<double> theFromLocationCoordinates;
            //            theFromLocationCoordinates << theFromLocation->getLatitude() << theFromLocation->getLongitude();

            //            // Get the location 'to' location
            //            RLocation *theToLocation = qobject_cast<RLocation *>(listOfRoads.at(i)->getToLocation());
            //            QVector<double> theToLocationCoordinates;
            //            theToLocationCoordinates << theToLocation->getLatitude() << theToLocation->getLongitude();


            //            // Creating the vector to receive the coordinates
            //            QVector<QVector<double>> coordinates;

            //            coordinates << theFromLocationCoordinates << theToLocationCoordinates;

            //            // Create the polyline
            //            theGISMapWidget->createPolyline(coordinates, listOfRoads.at(i));


        } // Roads



        // **************
        // Models in general - check the response list!!!
        // **************
        // Get a list of all models
        QList<RNetworkModel *> listOfWaterNetworkModels = theDomain->findChildren<RNetworkModel *>();

        for (int i = 0; i < listOfWaterNetworkModels.size(); i++){

            //            // Creating the vector to receive the coordinates
            //            QVector<QVector<double>> coordinates;

            //            // Getting the location of the first model
            //            RLocation *theLocationOftheFirstModel = qobject_cast<RLocation *>(listOfWaterNetworkModels.at(i)->getLocation());

            //            if (theLocationOftheFirstModel){
            //                // Passing the coordinates of the first model to the vector that will create the polyline
            //                QVector<double> theCoordinatesOfTheFirstModel;
            //                theCoordinatesOfTheFirstModel << theLocationOftheFirstModel->getLatitude() << theLocationOftheFirstModel->getLongitude();
            //                coordinates << theCoordinatesOfTheFirstModel;


            //                // Getting hold of all the responses coming in to this model
            //                QList<QPointer<RParameter> > theSupplierList = stringToParameterList(listOfWaterNetworkModels.at(i)->getWaterSupplierList());

            //                if (theSupplierList.size() > 0){
            //                    for (int j = 0; j < theSupplierList.size(); j++){

            //                        // Cast the response parameter as a response object
            //                        RResponse *theResponseObject = qobject_cast<RResponse *>(theSupplierList.at(j));

            //                        // Get the model of the response object as a network model
            //                        RNetworkModel *theSecondModel = qobject_cast<RNetworkModel *>(theResponseObject->getModel());

            //                        // Get the location of the second model
            //                        RLocation *theLocationOfTheSecondModel = qobject_cast<RLocation *>(theSecondModel->getLocation());

            //                        // Pass the coordinates of the second model to the vector that will create the polyline
            //                        QVector<double> theCoordinatesOfTheSecondModel;
            //                        theCoordinatesOfTheSecondModel << theLocationOfTheSecondModel->getLatitude() << theLocationOfTheSecondModel->getLongitude();
            //                        coordinates << theCoordinatesOfTheSecondModel;

            //                        // Create the polyline
            //                        theGISMapWidget->createPolyline(coordinates, theSecondModel);

            //                        // Clear the second model coordinates from the coordinates vector
            //                        coordinates.removeAt(1);
            //                    }
            //                }
            //            }
        } //


    }
}


void RNetworkManagerModel::displayCensusTractsOnMap()
{
    RNormalDistribution theNormalDistribution(theDomain);
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QList<RHouseholdPortfolio *> theListOfCensusTracts = theDomain->findChildren<RHouseholdPortfolio *>();

    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {
        for (int i = 0; i < theListOfCensusTracts.size(); i++){

            QString subString = theListOfCensusTracts.at(i)->objectName();
            subString = subString.remove(8,18);

            // qCritical() << theNormalDistribution.getInverseNormalCDF((1/0.25) * qLn(theListOfCensusTracts.at(i)->getMedianHouseholdIncome() / 56908.0),1.0);
            QVector<double> theCoordinates;
            theCoordinates << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLatitude() << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLongitude();
            theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), 0.0);
            theGISMapWidget->addLabel(theCoordinates, subString, QString("Black"));
        }
    }
}




void RNetworkManagerModel::displayRentersOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QList<RHouseholdPortfolio *> theListOfCensusTracts = theDomain->findChildren<RHouseholdPortfolio *>();

    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {

        double minValue = 9999999.0;
        double maxValue = 0.0;
        for (int i = 0; i < theListOfCensusTracts.size(); i++){
            maxValue = qMax(maxValue,1.0*theListOfCensusTracts.at(i)->getPercentageOfRentedBuildings());
            minValue = qMin(minValue,1.0*theListOfCensusTracts.at(i)->getPercentageOfRentedBuildings());
        }

        double deltaValue = maxValue - minValue;
        qCritical() << "Renters - " << "MinValue:" << minValue << "MaxValue:" << maxValue;


        for (int i = 0; i < theListOfCensusTracts.size(); i++){

            QString subString = theListOfCensusTracts.at(i)->objectName();
            subString = subString.remove(10,17);
            double plotValue = theListOfCensusTracts.at(i)->getPercentageOfRentedBuildings();

            QVector<double> theCoordinates;
            theCoordinates << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLatitude() << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLongitude();
            theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), (plotValue - minValue)/deltaValue);
        }
    }
}



void RNetworkManagerModel::displayRecentImmigrantsOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QList<RHouseholdPortfolio *> theListOfCensusTracts = theDomain->findChildren<RHouseholdPortfolio *>();

    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {

        double minValue = 9999999.0;
        double maxValue = 0.0;
        for (int i = 0; i < theListOfCensusTracts.size(); i++){
            maxValue = qMax(maxValue,1.0*theListOfCensusTracts.at(i)->getPercentageOfRecentImmigrants());
            minValue = qMin(minValue,1.0*theListOfCensusTracts.at(i)->getPercentageOfRecentImmigrants());
        }

        double deltaValue = maxValue - minValue;
        qCritical() << "Recent immigrants - " << "MinValue:" << minValue << "MaxValue:" << maxValue;


        for (int i = 0; i < theListOfCensusTracts.size(); i++){

            QString subString = theListOfCensusTracts.at(i)->objectName();
            subString = subString.remove(10,17);
            double plotValue = theListOfCensusTracts.at(i)->getPercentageOfRecentImmigrants();

            QVector<double> theCoordinates;
            theCoordinates << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLatitude() << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLongitude();
            theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), (plotValue - minValue)/deltaValue);
        }
    }
}


void RNetworkManagerModel::displayOldBuildingsOnMap()
{
    //    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    //    QList<RHouseholdPortfolio *> theListOfCensusTracts = theDomain->findChildren<RHouseholdPortfolio *>();

    //    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    //    if (!theGISMapWidget){
    //        theMainWindow->createGISWidget();
    //    }

    //    else {

    //        double minValue = 9999999.0;
    //        double maxValue = 0.0;
    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){
    //            maxValue = qMax(maxValue,theListOfCensusTracts.at(i)->getPercentagePre1940Buildings());
    //            minValue = qMin(minValue,theListOfCensusTracts.at(i)->getPercentagePre1940Buildings());
    //        }

    //        double deltaValue = maxValue - minValue;
    //        qCritical() << "Old buildings - " << "MinValue:" << minValue << "MaxValue:" << maxValue;

    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){

    //            QString subString = theListOfCensusTracts.at(i)->objectName();
    //            subString = subString.remove(10,17);

    //            double plotValue = theListOfCensusTracts.at(i)->getPercentagePre1940Buildings();

    //            QVector<double> theCoordinates;
    //            theCoordinates << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLatitude() << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLongitude();
    //            theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), (plotValue - minValue)/deltaValue);
    //        }
    //    }
}


void RNetworkManagerModel::displaySingleFamilyDwellingsOnMap()
{
    //    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    //    QList<RHouseholdPortfolio *> theListOfCensusTracts = theDomain->findChildren<RHouseholdPortfolio *>();

    //    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    //    if (!theGISMapWidget){
    //        theMainWindow->createGISWidget();
    //    }

    //    else {

    //        double minValue = 9999999.0;
    //        double maxValue = 0.0;
    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){
    //            theListOfCensusTracts.at(i)->instantiateBuildings();
    //            maxValue = qMax(maxValue,1.0*theListOfCensusTracts.at(i)->getNumberOfSingleFamilyBuildings());
    //            minValue = qMin(minValue,1.0*theListOfCensusTracts.at(i)->getNumberOfSingleFamilyBuildings());
    //        }

    //        double deltaValue = maxValue - minValue;
    //        qCritical() << "Single family dwellings - " << "MinValue:" << minValue << "MaxValue:" << maxValue;

    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){

    //            QString subString = theListOfCensusTracts.at(i)->objectName();
    //            subString = subString.remove(10,17);


    //            double plotValue = theListOfCensusTracts.at(i)->getNumberOfSingleFamilyBuildings();


    //            QVector<double> theCoordinates;
    //            theCoordinates << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLatitude() << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLongitude();
    //            theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), (plotValue - minValue)/deltaValue);
    //            //theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), plotValue);
    //        }
    //    }
}


void RNetworkManagerModel::displayMultiFamilyDwellingsOnMap()
{
    //    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    //    QList<RHouseholdPortfolio *> theListOfCensusTracts = theDomain->findChildren<RHouseholdPortfolio *>();

    //    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    //    if (!theGISMapWidget){
    //        theMainWindow->createGISWidget();
    //    }

    //    else {

    //        double minValue = 9999999.0;
    //        double maxValue = 0.0;
    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){
    //            theListOfCensusTracts.at(i)->instantiateBuildings();
    //            maxValue = qMax(maxValue,1.0*theListOfCensusTracts.at(i)->getNumberOfMultiFamilyBuildings());
    //            minValue = qMin(minValue,1.0*theListOfCensusTracts.at(i)->getNumberOfMultiFamilyBuildings());
    //        }

    //        double deltaValue = maxValue - minValue;
    //        qCritical() << "Multi family dwellings - " << "MinValue:" << minValue << "MaxValue:" << maxValue;

    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){

    //            QString subString = theListOfCensusTracts.at(i)->objectName();
    //            subString = subString.remove(10,17);


    //            double plotValue = theListOfCensusTracts.at(i)->getNumberOfMultiFamilyBuildings();

    //            QVector<double> theCoordinates;
    //            theCoordinates << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLatitude() << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLongitude();
    //            theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), (plotValue - minValue)/deltaValue);
    //        }
    //    }
}



void RNetworkManagerModel::displayHazardAreaSources()
{

}


void RNetworkManagerModel::displayDirectEconomicLossOnMap()
{
    //    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());

    //    QVector<double> theVariableOfInterest;
    //    double theMax = 0.0;

    //    for (int i = 0; i < theListOfCensusTracts.size(); i++){
    //        theVariableOfInterest << (theListOfCensusTracts.at(i)->getStructuralLossResponse())->getCurrentValue();
    //        theMax = qMax(theMax,(theListOfCensusTracts.at(i)->getStructuralLossResponse())->getCurrentValue());
    //    }


    //    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    //    if (!theGISMapWidget){
    //        theMainWindow->createGISWidget();
    //    }

    //    else {
    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){

    //            theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), theVariableOfInterest.at(i)/theMax);
    //        }
    //    }
}


void RNetworkManagerModel::displayInjuredPopulationOnMap()
{
    //    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());

    //    QVector<double> theVariableOfInterest;
    //    double theMax = 0.0;

    //    for (int i = 0; i < theListOfCensusTracts.size(); i++){
    //        theVariableOfInterest << (theListOfCensusTracts.at(i)->getInjuredPopulationResponse())->getCurrentValue();
    //        theMax = qMax(theMax,(theListOfCensusTracts.at(i)->getInjuredPopulationResponse())->getCurrentValue());
    //    }


    //    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    //    if (!theGISMapWidget){
    //        theMainWindow->createGISWidget();
    //    }

    //    else {
    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){

    //            theGISMapWidget->createPolygon(theListOfCensusTracts.at(i)->getBoundariesVector(), theVariableOfInterest.at(i)/theMax);
    //        }
    //    }
}


void RNetworkManagerModel::displayDisplacedPopulationOnMap()
{
    //    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    //    QList<RNeighborhoodModel *> theListOfCensusTracts = theDomain->findChildren<RNeighborhoodModel *>();

    //    QVector<double> theVariableOfInterest;
    //    double theMax = 0.0;

    //    for (int i = 0; i < theListOfCensusTracts.size(); i++){
    //        //theVariableOfInterest << theListOfCensusTracts.at(i)->getNumberOfDisplacedPersons();
    //        theMax = qMax(theMax,(theListOfCensusTracts.at(i)->getNumberOfDisplacedPersons()));
    //    }


    //    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();


    //    if (!theGISMapWidget){
    //        theMainWindow->createGISWidget();
    //    }

    //    else {
    //        for (int i = 0; i < theListOfCensusTracts.size(); i++){
    //            QVector<double> theCoordinates;
    //            theCoordinates << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLatitude() << qobject_cast<RLocation*>(theListOfCensusTracts.at(i)->getLocation())->getLongitude();
    //            theGISMapWidget->createPolygon((qobject_cast<RHouseholdPortfolio*>(theListOfCensusTracts.at(i)->getCensusInformation()))->getBoundariesVector(), theVariableOfInterest.at(i)/theMax);
    //            theGISMapWidget->addLabel(theCoordinates, QString("%1").arg(theVariableOfInterest.at(i)),QString("Black"));
    //        }
    //    }
}



void RNetworkManagerModel::displayRecoveryOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QList<RNeighborhoodModel *> theRecoveryModelList = theDomain->findChildren<RNeighborhoodModel *>();

    QVector<double> theValueToPlot;
    QVector<RHouseholdPortfolio *> theHouseholdPortfolioForThisCT;

    for (int i = 0; i < theRecoveryModelList.size(); i++){
        // Collect the value to plot from each CT model
        theValueToPlot << static_cast<double>(theRecoveryModelList.at(i)->getNumberOfHabitableDwellings());

        // Get a pointer to the Information Model for the CT
        theHouseholdPortfolioForThisCT << qobject_cast<RHouseholdPortfolio *>(theRecoveryModelList.at(i)->getHouseholdPortfolio());
    }

    // Identify the differential in recovery between CTs

    for (int i = 0; i < theValueToPlot.size(); i++){
        theMaximumRecoveryValue = qMax(theMaximumRecoveryValue,theValueToPlot.at(i));
        theMinimumRecoveryValue = qMin(theMinimumRecoveryValue,theValueToPlot.at(i));
    }

    double deltaValue = theMaximumRecoveryValue - theMinimumRecoveryValue;


    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        // Create the GIS map if it is not showing yet
        theMainWindow->createGISWidget();
    }

    else {
        // Update and existing map
        for (int i = 0; i < theRecoveryModelList.size(); i++){
            theGISMapWidget->createPolygon(theHouseholdPortfolioForThisCT.at(i)->getBoundariesVector(), (theValueToPlot.at(i) - theMinimumRecoveryValue)/deltaValue);
        }
    }
}

void RNetworkManagerModel::displayInfrastructureRecoveryOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {

        // Get a list of all the models
        QList<RNetworkModel *> modelList = theDomain->findChildren<RNetworkModel *>();
        RLocation *theLocationsAsPointer;

        for (int i = 0; i < modelList.size(); i++){

            bool theFunctionality = modelList.at(i)->getFunctionality();
            double theRepairTime = modelList.at(i)->getExpectedRepairTime();

            // Get the location of the model
            theLocationsAsPointer = qobject_cast<RLocation *>(modelList.at(i)->getLocation());
            QVector<double> theCoordinates;

            // If the model has a location, pass it to the map widget
            if (theLocationsAsPointer){

                theCoordinates << theLocationsAsPointer->getLatitude() << theLocationsAsPointer->getLongitude();

                if(!(qobject_cast<RWaterPipelineModel*>(modelList.at(i))) && !(qobject_cast<RResidentialMobilityNeighborhoodModel*>(modelList.at(i)))){
                    theGISMapWidget->createPoint(theCoordinates, modelList.at(i),theFunctionality, theRepairTime);
                }
            }
        }
    }
}

void RNetworkManagerModel::displayEpicenterOnMap()
{
    RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
    QPointer<RGISWidget> theGISMapWidget = theMainWindow->getGISWidget();

    if (!theGISMapWidget){
        theMainWindow->createGISWidget();
    }

    else {

        // Get hold of the epicenter location
        RBooreAtkinson2008IntensityModel * theIntensityModel = theDomain->findChild<RBooreAtkinson2008IntensityModel*>();
        if (theIntensityModel){
            QVector<double> epicenterCoordinates;

            epicenterCoordinates << theIntensityModel->getEpicenterLocationLatitude() << theIntensityModel->getEpicenterLocationLongitude();

            theGISMapWidget->createPoint(epicenterCoordinates);
        }
    }
}




#endif


//  ********************************


int RNetworkManagerModel::evaluateModel()
{
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
        timeAnalysisBegan = theCurrentTime->getCurrentValue();
    }


    RModelManager theModelManager(theDomain);

    // Creating the network
    if (myCounter == 0){

        // Get hold of all the models in the network
        QList<RModel *> modelList = theDomain->findChildren<RModel *>();
        QCoreApplication::processEvents();

        // Create a model map that is the basis for the path matrix
        theModelManager.createModelMap(theInputParameterList, modelList);

        // Define the paths from model to model
        theModelManager.establishPaths();

        QCoreApplication::processEvents();

        // Get hold of the path matrix
        QVector<QVector<RModel *>>  thePathMatrix = theModelManager.retrievePathMatrix();
        pathMatrixSize = thePathMatrix.size();

        // ********
        // ** THIS MIGHT HAVE NOT BEEN A GOOD IDEA, MAYBE REMOVE IF
        // ********
        if (thePathMatrix.size() > 0){
            // Pass the paths to their respective models
            theModelManager.setPaths();
        }

        // Save a copy of the original path matrix to be used later to "reset" the scenario
        for (int j = 0; j < pathMatrixSize; j++){
            originalPathMatrix << thePathMatrix.at(j);
        }
    }


    // Get the models that serve input to this network model
    QList<RModel *> inputModelList;
    for (int i = 0; i < theInputParameterList.size(); i++) {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameterList.at(i));
        inputModelList << qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    }


    // Run analysis in all models
    for (int i = 0; i < inputModelList.size(); i++) {
        // qCritical() << "Starting analysis at" << inputModelList.at(i)->objectName();
        inputModelList.at(i)->runAnalysis(RGradientType::NONE);
    }


    for (int i = 0; i < theInputParameterList.size(); i++) {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameterList.at(i));
        // The response from this model
        theResponse->setCurrentValue(theResponse->getCurrentValue() + theInputCastAsResponse->getCurrentValue());
    }


    // -------------------------------------------------------------------------------------------
    // Create plots
    // -------------------------------------------------------------------------------------------
    // If it has been more than certain days since last plot
    timeSinceLastPlot += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    lastEvaluationTime = theCurrentTime->getCurrentValue();

    if (timeSinceLastPlot >= theMaxTimeGap){
        int theNumberOfHabitableDwellings = 0;
        int theTotalNumberOfDwellings = 0;
        int theNumberOfDisplacedPersons = 0;
        int theTotalNumberOfPeople = 0;
        int theNumberOfPermanentlyDisplacedPersons = 0;
        int theNumberOfPersonLivingWithFamily = 0;
        int theNumberOfPersonsInHotels = 0;
        int theNumberOfPersonsInShelters = 0;
        int theNumberOfPersonsEvicted = 0;
        int theNumberOfPersonsLeavingVoluntarily = 0;
        int theNumberOfPersonsTerminated = 0;
        int theNumberOfPersonsWithNoFundsToRepair = 0;


        for (int i = 0; i < theInputParameterList.size(); i++) {
            RResponse *thisResponse = qobject_cast<RResponse *>(theInputParameterList.at(i));

            RResidentialMobilityNeighborhoodModel *theRodrigosNeighborhoodModel = qobject_cast<RResidentialMobilityNeighborhoodModel *>(thisResponse->getModel());
            RWorkforceStudyNeighborhoodModel *theChenbosNeighborhoodModel = qobject_cast<RWorkforceStudyNeighborhoodModel *>(thisResponse->getModel());

            if (theRodrigosNeighborhoodModel) {
                theTotalNumberOfDwellings += theRodrigosNeighborhoodModel->getNumberOfDwellings();
                theNumberOfHabitableDwellings += theRodrigosNeighborhoodModel->getNumberOfHabitableDwellings();
            }

            else if(theChenbosNeighborhoodModel){
                theTotalNumberOfDwellings += theChenbosNeighborhoodModel->getNumberOfDwellings();
                theNumberOfHabitableDwellings += theChenbosNeighborhoodModel->getNumberOfHabitableDwellings();
                theTotalNumberOfPeople += theChenbosNeighborhoodModel->getNumberOfPeople();
                theNumberOfDisplacedPersons += theChenbosNeighborhoodModel->getNumberOfDisplacedPersons();
                theNumberOfPermanentlyDisplacedPersons += theChenbosNeighborhoodModel->getNumberOfPermanentlyDisplaced();
                theNumberOfPersonLivingWithFamily += theChenbosNeighborhoodModel->getNumberOfPersonsLivingWithFamily();
                theNumberOfPersonsInHotels += theChenbosNeighborhoodModel->getNumberOfPersonsInHotels();
                theNumberOfPersonsInShelters += theChenbosNeighborhoodModel->getNumberOfPersonsInShelters();
            }

        } // for i

        if (theTotalNumberOfDwellings > 0){
            this->plotResults(theNumberOfHabitableDwellings, theTotalNumberOfDwellings);
        }

        if (theNumberOfPersonsInShelters > 0){
            this->plotResults(theNumberOfDisplacedPersons, theNumberOfPermanentlyDisplacedPersons, theNumberOfPersonLivingWithFamily, theNumberOfPersonsInHotels, theNumberOfPersonsInShelters,QString("Displaced persons"), QString("People gone"), QString("People living with family"), QString("People in rented housing"), QString("People in public shelters"),theTotalNumberOfPeople);
        }

        if (theNumberOfPersonsEvicted){
            this->plotResults(theNumberOfPersonsLeavingVoluntarily,theNumberOfPersonsTerminated, theNumberOfPersonsEvicted, theNumberOfPersonsWithNoFundsToRepair, QString("Voluntary mobility"),QString("Terminated"), QString("Evictions"), QString("No funds"),theTotalNumberOfPeople);
        }



        timeSinceLastPlot = 0.0;

#ifdef GIS
        if (theOutputLevel == RDomain::Maximum) {
            //this->displayRecoveryOnMap();
            //this->displayInfrastructureRecoveryOnMap();
            //this->displayEpicenterOnMap();
        }
#endif

    }

    myCounter += 1;
    return 1;
}



void RNetworkManagerModel::plotResults(double habitableDwellings, double totalDwellings)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (customPlot == nullptr) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            customPlot = new QCustomPlot();
            QString theString = QString("Total inhabitable residential dwellings %1"); //.arg(this->objectName());
            mainWindow->addSubWindow(customPlot, theString, true);
            customPlot->addGraph();
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            customPlot->graph(0)->setName("Total inhabitable residential dwellings");
            customPlot->xAxis->setLabel("Time (days)");
            customPlot->yAxis->setLabel(theString);
           // customPlot->xAxis->setAutoTicks(false);
           //customPlot->xAxis->setAutoTickLabels(false);
            customPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            customPlot->legend->setFont(legendFont);
            customPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

            double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

            customPlot->graph(0)->addData(0.0, totalDwellings);
            customPlot->yAxis->setRange(0, totalDwellings);
            customPlot->xAxis->setRange(0, timeElapsed + 1);
            customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

            tickVector << timeElapsed;
            labelVector << QString("%1").arg((int) timeElapsed);
            //customPlot->xAxis->setTickVector(tickVector);
            //customPlot->xAxis->setTickVectorLabels(labelVector);
            customPlot->replot();
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan + 0.01;

        customPlot->graph(0)->addData(timeElapsed, habitableDwellings);
        customPlot->yAxis->setRange(0, totalDwellings);
        customPlot->xAxis->setRange(0, timeElapsed + 1.0);
        customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        tickVector << timeElapsed;
        labelVector << QString("%1").arg((int) timeElapsed);
        //customPlot->xAxis->setTickVector(tickVector);
        //customPlot->xAxis->setTickVectorLabels(labelVector);
        customPlot->replot();
    }
}


void RNetworkManagerModel::plotResults(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name, double maxValue)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (customPlotResidentialMobility == nullptr) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            customPlotResidentialMobility = new QCustomPlot();
            QString theString = QString("Displaced Persons at %1").arg(this->objectName());
            mainWindow->addSubWindow(customPlotResidentialMobility, theString, true);
            customPlotResidentialMobility->addGraph();
            customPlotResidentialMobility->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            customPlotResidentialMobility->graph(0)->setPen(QColor(0, 0, 0, 255));
            customPlotResidentialMobility->graph(0)->setName(var1Name);
            customPlotResidentialMobility->addGraph();
            customPlotResidentialMobility->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
            customPlotResidentialMobility->graph(1)->setPen(QColor(0, 0, 255, 255));
            customPlotResidentialMobility->graph(1)->setName(var2Name);
            customPlotResidentialMobility->addGraph();
            customPlotResidentialMobility->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 7));
            customPlotResidentialMobility->graph(2)->setPen(QColor(255, 0, 0, 255));
            customPlotResidentialMobility->graph(2)->setName(var3Name);
            customPlotResidentialMobility->addGraph();
            customPlotResidentialMobility->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, 7));
            customPlotResidentialMobility->graph(3)->setPen(QColor(255, 0, 255, 255));
            customPlotResidentialMobility->graph(3)->setName(var4Name);
            customPlotResidentialMobility->xAxis->setLabel("Time (days)");
            customPlotResidentialMobility->yAxis->setLabel(theString);
            //customPlotResidentialMobility->xAxis->setAutoTicks(false);
            //customPlotResidentialMobility->xAxis->setAutoTickLabels(false);
            customPlotResidentialMobility->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            customPlotResidentialMobility->legend->setFont(legendFont);
            customPlotResidentialMobility->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            customPlotResidentialMobility->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            customPlotResidentialMobility->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        customPlotResidentialMobility->graph(0)->addData(timeElapsed, var1);
        customPlotResidentialMobility->graph(1)->addData(timeElapsed, var2);
        customPlotResidentialMobility->graph(2)->addData(timeElapsed, var3);
        customPlotResidentialMobility->graph(3)->addData(timeElapsed, var4);
        customPlotResidentialMobility->yAxis->setRange(0, maxValue);
        customPlotResidentialMobility->xAxis->setRange(0, timeElapsed + 1);
        customPlotResidentialMobility->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        tickVector << timeElapsed;
        labelVector << QString("%1").arg((int) timeElapsed);
        //customPlotResidentialMobility->xAxis->setTickVector(tickVector);
        //customPlotResidentialMobility->xAxis->setTickVectorLabels(labelVector);
        customPlotResidentialMobility->replot();
    }
}



void RNetworkManagerModel::plotResults(double var1, double var2, double var3, double var4, double var5, QString var1Name, QString var2Name, QString var3Name, QString var4Name, QString var5Name, double maxValue)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (customPlotDisplaced == nullptr) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            customPlotDisplaced = new QCustomPlot();
            QString theString = QString("Displaced Persons at %1").arg(this->objectName());
            mainWindow->addSubWindow(customPlotDisplaced, theString, true);
            customPlotDisplaced->addGraph();
            customPlotDisplaced->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            customPlotDisplaced->graph(0)->setPen(QColor(0, 0, 0, 255));
            customPlotDisplaced->graph(0)->setName(var1Name);
            customPlotDisplaced->addGraph();
            customPlotDisplaced->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
            customPlotDisplaced->graph(1)->setPen(QColor(0, 0, 255, 255));
            customPlotDisplaced->graph(1)->setName(var2Name);
            customPlotDisplaced->addGraph();
            customPlotDisplaced->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 7));
            customPlotDisplaced->graph(2)->setPen(QColor(255, 0, 0, 255));
            customPlotDisplaced->graph(2)->setName(var3Name);
            customPlotDisplaced->addGraph();
            customPlotDisplaced->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar, 7));
            customPlotDisplaced->graph(3)->setPen(QColor(255, 0, 255, 255));
            customPlotDisplaced->graph(3)->setName(var4Name);
            customPlotDisplaced->addGraph();
            customPlotDisplaced->graph(4)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangleInverted, 7));
            customPlotDisplaced->graph(4)->setPen(QColor(0, 255, 0, 255));
            customPlotDisplaced->graph(4)->setName(var5Name);
            customPlotDisplaced->xAxis->setLabel("Time (days)");
            customPlotDisplaced->yAxis->setLabel(theString);
            //customPlotDisplaced->xAxis->setAutoTicks(false);
            //customPlotDisplaced->xAxis->setAutoTickLabels(false);
            customPlotDisplaced->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            customPlotDisplaced->legend->setFont(legendFont);
            customPlotDisplaced->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            customPlotDisplaced->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            customPlotDisplaced->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        customPlotDisplaced->graph(0)->addData(timeElapsed, var1);
        customPlotDisplaced->graph(1)->addData(timeElapsed, var2);
        customPlotDisplaced->graph(2)->addData(timeElapsed, var3);
        customPlotDisplaced->graph(3)->addData(timeElapsed, var4);
        customPlotDisplaced->graph(4)->addData(timeElapsed, var5);
        customPlotDisplaced->yAxis->setRange(0, maxValue);
        customPlotDisplaced->xAxis->setRange(0, timeElapsed + 1);
        customPlotDisplaced->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        tickVector << timeElapsed;
        labelVector << QString("%1").arg((int) timeElapsed);
        //customPlotDisplaced->xAxis->setTickVector(tickVector);
        //customPlotDisplaced->xAxis->setTickVectorLabels(labelVector);
        customPlotDisplaced->replot();
    }
}


