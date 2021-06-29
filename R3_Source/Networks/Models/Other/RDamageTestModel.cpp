#include "RDamageTestModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include "RCensusInformation.h"
#include "RIntensityModel.h"
#include "RMCEERShelterNeedsEstimator.h"

#include <math.h>

RDamageTestModel::RDamageTestModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theNumberOfBuildings = 0;
    totalHabitableArea = 0.0;
    theTotalBuildingArea = 0.0;
    theTotalNumberOfDwellings = 0;
    theNumberOfPeopleTimesDaysDisplaced = 0.0;


    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}

RDamageTestModel::~RDamageTestModel()
{
}


int RDamageTestModel::resetTime()
{
    // Reset data member on each new scenario
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    timeSinceLastPlot = 1000.0;
    buildingRepairsPlot = nullptr;
    displacedPopulationPlot = nullptr;
    thePowerShortageDuration = 0.0;
    theWaterShortageDuration = 0.0;
    theApparentDamage = 0.0;
    averageDamageState = 0.0;
    theSnowballFactor = 0.0;
    theNumberOfDwellingsAtDS0 = 0;
    theNumberOfDwellingsAtDS1 = 0;
    theNumberOfDwellingsAtDS2 = 0;
    theNumberOfDwellingsAtDS3 = 0;
    theNumberOfDwellingsAtDS4 = 0;

    tickVector.clear();
    labelVector.clear();
    timeVector.clear();
    theHousingRecoveryVector.clear();
    theDisplacedPopulationVector.clear();
    theImmediateEconomicLoss.clear();
    theImmediateContentLoss.clear();
    theTimeNeededForRepairs.clear();
    theTimeNeededForReoccupation.clear();
    theTimeUnderRepairs.clear();
    theBuildingDamageState.clear();

    theImmediateEconomicLoss.resize(theNumberOfBuildings);
    theImmediateEconomicLoss.fill(0.0);

    theImmediateContentLoss.resize(theNumberOfBuildings);
    theImmediateContentLoss.fill(0.0);

    theTimeNeededForRepairs.resize(theNumberOfBuildings);
    theTimeNeededForRepairs.fill(0.0);

    theTimeNeededForReoccupation.resize(theNumberOfBuildings);
    theTimeNeededForReoccupation.fill(0.0);

    theTimeUnderRepairs.resize(theNumberOfBuildings);
    theTimeUnderRepairs.fill(0.0);

    theBuildingDamageState.resize(theNumberOfBuildings);
    theBuildingDamageState.fill(0);

    return 0;
}


QObject * RDamageTestModel::getCensusInformation() const
{
    return theCensusInformation;
}

void RDamageTestModel::setCensusInformation(QObject *value)
{
    theCensusInformation = qobject_cast<RCensusInformation*>(value);
}


QObject* RDamageTestModel::getIntensityList()
{
    return theIntensityModelResponse;
}

void RDamageTestModel::setIntensityList(QObject *value)
{
    theIntensityModelResponse =  qobject_cast<RResponse*>(value);
}


QObject* RDamageTestModel::getBuildingDamageModel()
{
    return theDamageModelResponse;
}

void RDamageTestModel::setBuildingDamageModel(QObject* value)
{
    theDamageModelResponse= qobject_cast<RResponse*>(value);
}



QVector<double> RDamageTestModel::getTimeVector()
{
    return timeVector;
}

QVector<double> RDamageTestModel::getHousingRecoveryVector()
{
    return theHousingRecoveryVector;
}

QVector<int> RDamageTestModel::getDisplacedPopulationVector()
{
    return theDisplacedPopulationVector;
}

QVector<int> RDamageTestModel::getPopulationWithFamilyVector()
{
    return thePersonsWithFamilyVector;
}

QVector<int> RDamageTestModel::getPopulationInSheltersVector()
{
    return thePersonsInShelterVector;
}

QVector<int> RDamageTestModel::getPopulationInRentedHousingVector()
{
    return thePersonsInRenterHousingVector;
}

QVector<int> RDamageTestModel::getPopulationReplacedVector()
{
    return thePersonsGoneVector;
}


int RDamageTestModel::getNumberOfDwellings()
{
    return theTotalNumberOfDwellings;
}

int RDamageTestModel::getNumberOfHabitableDwellings()
{
    return theNumberOfHabitableDwellings;
}

int RDamageTestModel::getNumberOfDisplacedPersons()
{
    return theNumberOfPeopleAtHome;
}

int RDamageTestModel::getNumberOfPermanentlyDisplaced()
{
    return theNumberOfPeopleGone;
}

int RDamageTestModel::getNumberOfPersonsInHotels()
{
    return theNumberOfPeopleInHotels;
}


int RDamageTestModel::getNumberOfPersonsLivingWithFamily()
{
    return theNumberOfPeopleLivingWithTheirFamilies;
}

int RDamageTestModel::getNumberOfPersonsInShelters()
{
    return theNumberOfPeopleSeekingShelter;
}

QVector<QString> RDamageTestModel::getHouseholdArchetype()
{
    return theHouseholdArchetype;
}

void RDamageTestModel::getRequests()
{

}


int RDamageTestModel::evaluateModel(RGradientType theGradientType)
{
    // Output
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();



    RNormalDistribution theNormalDistribution(theDomain);

    // "Functionality vs time" plot
    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
        timeAnalysisBegan = theCurrentTime->getCurrentValue();
    }


    // -------------------------------------------------------------------------------------------
    // 1 - Define the archetypes
    // --------------------------------------------------------------------------------------
    if (theNumberOfBuildings < 1){

        // Tell the Census Information to define the buildings
        theCensusInformation->instantiateBuildings();

        theNumberOfBuildings = theCensusInformation->getNumberOfBuildings();
        theBuildingType = theCensusInformation->getBuildingTypesVector();
        theCodeLevel = theCensusInformation->getBuildingCodeLevelVector();
        theOccupancyClass = theCensusInformation->getBuildingOccupancyClassVector();
        theBuildingArea = theCensusInformation->getBuildingAreaVector();
        theBuildingValue = theCensusInformation->getBuildingValueVector();
        theBuildingNumberOfStoreys = theCensusInformation->getBuildingStoreysVector();
        theNumberOfDwellingsInTheBuilding = theCensusInformation->getBuildingDwellingsVector();
        theBuildingLocation = theCensusInformation->getBuildingLocationVector();


        theTotalNumberOfDwellings = 0.0;
        for (int i = 0; i < theNumberOfBuildings; i++){
            theTotalNumberOfDwellings += theNumberOfDwellingsInTheBuilding.at(i);
            theTotalBuildingArea += theBuildingArea.at(i);
        }

        theBuildingDamageState.resize(theNumberOfBuildings);
        theBuildingDamageState.fill(0);

    } // if



    // -------------------------------------------------------------------------------------------
    // 3 - Immediate losses
    // -------------------------------------------------------------------------------------------
    // Considering only one earthquake in this analysis - think about aftershocks later
    if (timeOfEvent == 0.0){

        theNumberOfDwellingsAtDS0 = 0;
        theNumberOfDwellingsAtDS1 = 0;
        theNumberOfDwellingsAtDS2 = 0;
        theNumberOfDwellingsAtDS3 = 0;
        theNumberOfDwellingsAtDS4 = 0;


        // Get the pointer to the intensity model
        RResponse* theResponseObject = qobject_cast<RResponse *>(theIntensityModelResponse);
        RIntensityModel *theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());


        // Tell that intensity model what we want
        theIntensityModel->setLocation(theLocation);
        theIntensityModel->setIntensityMeasure(QString("Sa"));


        // Run the intensity model and get the response
        theIntensityModel->setPeriod(0.3);
        theIntensityModel->evaluateModel(RGradientType::NONE);
        double Sa0_3 = theResponseObject->getCurrentValue();

        theIntensityModel->setPeriod(1.0);
        theIntensityModel->evaluateModel(RGradientType::NONE);
        double Sa1 = theResponseObject->getCurrentValue();


        theDamageModel = qobject_cast<RHazusBuildingModel*>(theDamageModelResponse->getModel());


        // For all the buildings
        for (int i = 0; i < theNumberOfBuildings; i++){


            RLocation* myLocation = parameterToLocation(theLocation);
            myLocation->setLatitude(theBuildingLocation.at(i).first);
            myLocation->setLongitude(theBuildingLocation.at(i).second);


            // Building damage
            theDamageModel->setBuildingType(theBuildingType.at(i));
            theDamageModel->setCodeDesignLevel(theCodeLevel.at(i));
            theDamageModel->setOccupancyClass(theOccupancyClass.at(i));
            theDamageModel->setSa1(Sa1);
            theDamageModel->setSa2(Sa0_3);
            theDamageModel->evaluateModel(RGradientType::NONE);
            theBuildingDamageState[i] = theDamageModel->getStrDS();
            //int theNSDDamageState = theDamageModel->getNSDDS();
            //int theNSADamageState= theDamageModel->getNSADS();

            if (theBuildingDamageState.at(i) == 0){
                theNumberOfDwellingsAtDS0 += theNumberOfDwellingsInTheBuilding.at(i);

            } else if (theBuildingDamageState.at(i) == 1){
                theNumberOfDwellingsAtDS1 += theNumberOfDwellingsInTheBuilding.at(i);

            } else if (theBuildingDamageState.at(i) == 2){
                theNumberOfDwellingsAtDS2 += theNumberOfDwellingsInTheBuilding.at(i);

            } else if (theBuildingDamageState.at(i) == 3){
                theNumberOfDwellingsAtDS3 += theNumberOfDwellingsInTheBuilding.at(i);

            } else if (theBuildingDamageState.at(i) == 4){
                theNumberOfDwellingsAtDS4 += theNumberOfDwellingsInTheBuilding.at(i);
            }


            // Considering only one earthquake in this analysis - think about aftershocks later
//            if (theBuildingDamageState[i] > 0.0 || theNSDDamageState > 0.0 || theNSADamageState > 0.0){
//                timeOfEvent = theCurrentTime->getCurrentValue();


//                QVector<double> CDFStrVector;
//                QVector<double> CDFNonstrAccVector;
//                QVector<double> CDFNonstrDriftVector;
//                QVector<double> ContentDamageVector;
//                QVector<double> RepairTimeVector;

//                CDFStrVector << 0.0;
//                CDFNonstrAccVector << 0.0;
//                CDFNonstrDriftVector << 0.0;
//                RepairTimeVector << 0.0;
//                ContentDamageVector << 0.0;


//                if (theOccupancyClass.at(i) == QString("RES1")) {
//                    CDFStrVector << 0.5 << 2.3 << 11.7 << 23.4;
//                    CDFNonstrAccVector << 0.5 << 2.7 << 8.0 << 26.6;
//                    CDFNonstrDriftVector << 1.0 << 5.5 << 25.0 << 50.0;
//                    ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
//                    RepairTimeVector << 2.0 << 30.0 << 90.0 << 180.0;

//                } else if (theOccupancyClass.at(i) == QString("RES3")) {
//                    CDFStrVector << 0.3 << 1.4 << 6.9 << 13.8 ;
//                    CDFNonstrAccVector << 0.8 << 4.3 << 13.1 << 43.7 ;
//                    CDFNonstrDriftVector << 0.9 << 4.3 << 21.3 << 42.5 ;
//                    ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
//                    RepairTimeVector << 2.0 << 30.0 << 120.0 << 240.0;
//                }


//                double theEffectiveDamageRatio = (CDFStrVector.at(theBuildingDamageState[i]) + CDFNonstrDriftVector.at(theNSDDamageState) + CDFNonstrAccVector.at(theNSADamageState)) / 100.0;
//                double theStructuralLossRatio = (CDFStrVector.at(theBuildingDamageState[i])) / 100.0;
//                double theDriftLossRatio = (CDFNonstrDriftVector.at(theNSDDamageState)) / 100.0;
//                double theAccelerationLossRatio = (CDFNonstrAccVector.at(theNSADamageState)) / 100.0;
//                double theTotalLoss = (theStructuralLossRatio + theDriftLossRatio + theAccelerationLossRatio) * theBuildingValue.at(i);
//                double theContentReplacementCost = ContentDamageVector.at(theBuildingDamageState[i]) * theBuildingValue.at(i);
//                double theReplacementTime = RepairTimeVector.at(theBuildingDamageState[i]);


//                // If too much loss
//                if (theEffectiveDamageRatio > 1.0){
//                    theNeedForRedesign[i] = true;
//                }
//                // If too much loss
//                else if (theTotalLoss > 0.50 * theBuildingValue.at(i)){
//                    theNeedForRedesign[i] = true;
//                }
//                // If old building
//                else if (theTotalLoss > 0.40 * theBuildingValue.at(i) && (theCodeLevel.at(i) == QString("Pre") || theCodeLevel.at(i) == QString("Low"))){
//                    theNeedForRedesign[i] = true;
//                }



//                // If replacement is not needed, i.e.m, rho <> 1.25
//                if (theNeedForRedesign[i] != true){
//                    theImmediateEconomicLoss[i] = theTotalLoss;
//                    theTimeNeededForRepairs[i] = theReplacementTime;
//                    theImmediateContentLoss[i] = theContentReplacementCost * theBuildingValue.at(i);

//                    // If replacement is needed
//                } else {
//                    //qCritical() << "needs redesign" << theImmediateEconomicLoss[i] << theTimeNeededForRepairs[i] << theImmediateContentLoss[i] << theBuildingValue.at(i) << theReplacementTime << theContentReplacementCost;
//                    theImmediateEconomicLoss[i] = 1.25 * theBuildingValue.at(i);
//                    theTimeNeededForRepairs[i] = 1.25 * theReplacementTime;
//                    theImmediateContentLoss[i] = 1.0 * theContentReplacementCost * theBuildingValue.at(i);
//                }

//                // Reoccupancy time
//                double theServiceInterruptionMultiplier = 0.0;
//                if (theBuildingDamageState[i] >= 3){
//                    theServiceInterruptionMultiplier = 1.0;

//                } else if (theBuildingDamageState[i] < 3) {
//                    theServiceInterruptionMultiplier = 0.5;
//                }


//                avgTime += theServiceInterruptionMultiplier * theReplacementTime;
//                theTimeNeededForReoccupation[i] = theServiceInterruptionMultiplier * theReplacementTime;


//                if (theBuildingTenure.at(i) == QString("Renter") && theOccupancyClass.at(i) == QString("RES1")){
//                    theDelayInTakingAction[i] = 30.0;
//                }


//                // qCritical() << this->objectName() << "building" << i << theOccupancyClass.at(i) << "has a repair time of" << theTimeNeededForRepairs.at(i) << "days. The damage ratio is" << theBuildingDamageState.at(i) << theNSDDamageState << theNSADamageState;
//            }
        }


    } // i


    return 1;
}



void RDamageTestModel::plotUsableBuildings(double habitableArea)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (buildingRepairsPlot == nullptr) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            buildingRepairsPlot = new QCustomPlot();
            QString theString = QString("Total habitable dwellings %1").arg(this->objectName());
            mainWindow->addSubWindow(buildingRepairsPlot, theString, true);
            buildingRepairsPlot->addGraph();
            buildingRepairsPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            buildingRepairsPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            buildingRepairsPlot->graph(0)->setName("Total inhabitable dwellings");
            buildingRepairsPlot->xAxis->setLabel("Time (days)");
            buildingRepairsPlot->yAxis->setLabel(theString);
            buildingRepairsPlot->xAxis->setAutoTicks(false);
            buildingRepairsPlot->xAxis->setAutoTickLabels(false);
            buildingRepairsPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            buildingRepairsPlot->legend->setFont(legendFont);
            buildingRepairsPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            buildingRepairsPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            buildingRepairsPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        buildingRepairsPlot->graph(0)->addData(timeElapsed, habitableArea);
        buildingRepairsPlot->yAxis->setRange(0, theTotalNumberOfDwellings);
        buildingRepairsPlot->xAxis->setRange(0, timeElapsed + 1);
        buildingRepairsPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        tickVector << timeElapsed;
        labelVector << QString("%1").arg(static_cast<int>(timeElapsed));
        buildingRepairsPlot->xAxis->setTickVector(tickVector);
        buildingRepairsPlot->xAxis->setTickVectorLabels(labelVector);
        buildingRepairsPlot->replot();
    }
}



