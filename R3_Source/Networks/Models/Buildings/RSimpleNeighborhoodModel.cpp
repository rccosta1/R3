#include "RSimpleNeighborhoodModel.h"
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

RSimpleNeighborhoodModel::RSimpleNeighborhoodModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theNumberOfBuildings = 0;
    totalHabitableArea = 0.0;
    theTotalBuildingArea = 0.0;
    theTotalNumberOfDwellings = 0;
    theSa03Counter = -1;
    theSa1Counter = -1;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "PowerResponse", "RGenericResponse");
    thePowerShortageDurationResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePowerShortageDurationResponse->setModel(this);
    thePowerShortageDurationResponse->setCurrentValue(0.0);

    theDomain->createObject(objectName() + "WaterResponse", "RGenericResponse");
    theWaterShortageDurationResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theWaterShortageDurationResponse->setModel(this);
    theWaterShortageDurationResponse->setCurrentValue(0.0);
}

RSimpleNeighborhoodModel::~RSimpleNeighborhoodModel()
{
}


int RSimpleNeighborhoodModel::resetTime()
{
    // Reset data member on each new scenario
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    timeSinceLastPlot = 1000.0;
    buildingRepairsPlot = nullptr;
    displacedPopulationPlot = nullptr;
    thePowerShortageDuration = 0.0;
    theWaterShortageDuration = 0.0;
    theAverageDamageState = 0.0;
    theRealTimeDisplaced = 0.0;

    // Reset some vectors
    theNumberOfPeopleSeekingShelter = 0;
    theNumberOfPeopleInHotels = 0;
    theNumberOfPeopleGone = 0;
    theNumberOfPeopleLivingWithTheirFamilies = 0;
    theTotalTimeDisplaced = 0.000001;

    // -----------
    // Not the most ellegant solution
    // PGA [0 , 1 , 2 ..., n]
    QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());

    if (thePGACounter > (theSa0_3Vector.size() - 1)){
        qCritical() << this->objectName() << "Hey, I do not have enough Sa values!";
        thePGACounter = 0;
        theSa03Counter = 0;
        theSa1Counter = 0;
    }

    theSa03Counter++;
    theSa1Counter++;
    // -----------

    // Weather conditions
    theWeatherConditions = QString("Good");

    double rngWeather = rand()/static_cast<double>(RAND_MAX);
    if (rngWeather < 0.5){
        theWeatherConditions = QString("Bad");
    }

    thePercentageOfBuildingsAtDS0 = 0.0;
    thePercentageOfBuildingsAtDS1 = 0.0;
    thePercentageOfBuildingsAtDS2 = 0.0;
    thePercentageOfBuildingsAtDS3 = 0.0;
    thePercentageOfBuildingsAtDS4 = 0.0;
    thePercentageOfDwellingsAtDS0 = 0.0;
    thePercentageOfDwellingsAtDS1 = 0.0;
    thePercentageOfDwellingsAtDS2 = 0.0;
    thePercentageOfDwellingsAtDS3 = 0.0;
    thePercentageOfDwellingsAtDS4 = 0.0;

    theTickVector.clear();
    theLabelVector.clear();
    timeVector.clear();
    theHousingRecoveryVector.clear();
    theDisplacedPopulationVector.clear();
    thePersonsWithFamilyVector.clear();
    thePersonsInShelterVector.clear();
    thePersonsInRenterHousingVector.clear();
    thePersonsGoneVector.clear();
    theStructuralDamageState.clear();
    theNSDDamageState.clear();
    theNSADamageState.clear();
    theHouseholdFatalityIncidence.clear();
    thePoorVector.clear();
    theModerateVector.clear();
    theRichVector.clear();
    thePreCodeVector.clear();
    theLowCodeVector.clear();
    theModCodeVector.clear();
    theHighCodeVector.clear();
    theOwnerVector.clear();
    theRenterVector.clear();
    theSFVector.clear();
    theMFVector.clear();
    theInsuredVector.clear();
    theNotInsuredVector.clear();
    theRecentVector.clear();
    theEstablishedVector.clear();
    theResidentVector.clear();
    theWhiteVector.clear();
    theBlackVector.clear();
    theHispanicVector.clear();
    theAsianVector.clear();
    theNativeAmericanVector.clear();
    theVulnerableMemberVector.clear();
    theNoVulnerableMemberVector.clear();
    theFunctionalHabitabilityVector.clear();
    thePerceivedHabitabilityVector.clear();

    theStructuralDamageState.resize(theNumberOfBuildings);
    theStructuralDamageState.fill(0);

    theNSDDamageState.resize(theNumberOfBuildings);
    theNSDDamageState.fill(0);

    theNSADamageState.resize(theNumberOfBuildings);
    theNSADamageState.fill(0);

    theTickVector << 0;
    theLabelVector << QString("%1").arg(0);

    return 0;
}


QObject * RSimpleNeighborhoodModel::getHouseholdPortfolio() const
{
    return theHouseholdPortfolio;
}

void RSimpleNeighborhoodModel::setHouseholdPortfolio(QObject *value)
{
    theHouseholdPortfolio = qobject_cast<RHouseholdPortfolio*>(value);
}


QObject* RSimpleNeighborhoodModel::getSa()
{
    return theIntensityModelResponse;
}

void RSimpleNeighborhoodModel::setSa(QObject *value)
{
    theIntensityModelResponse =  qobject_cast<RResponse*>(value);
}


QObject* RSimpleNeighborhoodModel::getBuildingDamageModel()
{
    return theBuildingModelResponse;
}

void RSimpleNeighborhoodModel::setBuildingDamageModel(QObject* value)
{
    theBuildingModelResponse = qobject_cast<RResponse*>(value);
}

QObject* RSimpleNeighborhoodModel::getPreparednessEstimator()
{
    return thePreparednessEstimator;
}

void RSimpleNeighborhoodModel::setPreparednessEstimator(QObject *value)
{
    thePreparednessEstimator = qobject_cast<RDisasterPreparednessEstimator*>(value);
}

QObject* RSimpleNeighborhoodModel::getDisplacementEstimator()
{
    return theDisplacementEstimator;
}

void RSimpleNeighborhoodModel::setDisplacementEstimator(QObject *value)
{
    theDisplacementEstimator = qobject_cast<RPopulationDisplacementEstimator*>(value);
}

QObject* RSimpleNeighborhoodModel::getMorbidityEstimator()
{
    return theMorbidityEstimator;
}

void RSimpleNeighborhoodModel::setMorbidityEstimator(QObject *value)
{
    theMorbidityEstimator = qobject_cast<RSutley2016MorbidityEstimator*>(value);
}


QString RSimpleNeighborhoodModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RSimpleNeighborhoodModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}


QString RSimpleNeighborhoodModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RSimpleNeighborhoodModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}


QVector<double> RSimpleNeighborhoodModel::getTimeVector()
{
    return timeVector;
}

QVector<int> RSimpleNeighborhoodModel::getHousingRecoveryVector()
{
    return theHousingRecoveryVector;
}

QVector<int> RSimpleNeighborhoodModel::getDisplacedPopulationVector()
{
    return theDisplacedPopulationVector;
}

QVector<int> RSimpleNeighborhoodModel::getPopulationWithFamilyVector()
{
    return thePersonsWithFamilyVector;
}

QVector<int> RSimpleNeighborhoodModel::getPopulationInSheltersVector()
{
    return thePersonsInShelterVector;
}

QVector<int> RSimpleNeighborhoodModel::getPopulationInRentedHousingVector()
{
    return thePersonsInRenterHousingVector;
}

QVector<int> RSimpleNeighborhoodModel::getPopulationReplacedVector()
{
    return thePersonsGoneVector;
}

QVector<double> RSimpleNeighborhoodModel::getPoorVector()
{
    return thePoorVector;
}

QVector<double> RSimpleNeighborhoodModel::getModIncomeVector()
{
    return theModerateVector;
}

QVector<double> RSimpleNeighborhoodModel::getRichVector()
{
    return theRichVector;
}

QVector<double> RSimpleNeighborhoodModel::getPreCodeVector()
{
    return thePreCodeVector;
}

QVector<double> RSimpleNeighborhoodModel::getLowCodeVector()
{
    return theLowCodeVector;
}

QVector<double> RSimpleNeighborhoodModel::getModCodeVector()
{
    return theModCodeVector;
}

QVector<double> RSimpleNeighborhoodModel::getHighCodeVector()
{
    return theHighCodeVector;
}

QVector<double> RSimpleNeighborhoodModel::getOwnerVector()
{
    return theOwnerVector;
}

QVector<double> RSimpleNeighborhoodModel::getRenterVector()
{
    return theRenterVector;
}

QVector<double> RSimpleNeighborhoodModel::getSFVector()
{
    return theSFVector;
}

QVector<double> RSimpleNeighborhoodModel::getMFVector()
{
    return theMFVector;
}

QVector<double> RSimpleNeighborhoodModel::getFunctionalHabitability()
{
    return theFunctionalHabitabilityVector;
}

QVector<double> RSimpleNeighborhoodModel::getPerceivedHabitability()
{
    return thePerceivedHabitabilityVector;
}

QVector<double> RSimpleNeighborhoodModel::getInsured()
{
    return theInsuredVector;
}

QVector<double> RSimpleNeighborhoodModel::getNoInsured()
{
    return theNotInsuredVector;
}

QVector<double> RSimpleNeighborhoodModel::getRecentVector()
{
    return theRecentVector;
}

QVector<double> RSimpleNeighborhoodModel::getEstablishedVector()
{
    return theEstablishedVector;
}

QVector<double> RSimpleNeighborhoodModel::getResidentVector()
{
    return theResidentVector;
}

QVector<double> RSimpleNeighborhoodModel::getWhiteVector()
{
    return theWhiteVector;
}

QVector<double> RSimpleNeighborhoodModel::getBlackVector()
{
    return theBlackVector;
}

QVector<double> RSimpleNeighborhoodModel::getHispanicVector()
{
    return theHispanicVector;
}

QVector<double> RSimpleNeighborhoodModel::getAsianVector()
{
    return theAsianVector;
}

QVector<double> RSimpleNeighborhoodModel::getNativeAmericanVector()
{
    return theNativeAmericanVector;
}

QVector<double> RSimpleNeighborhoodModel::getVulnerableMemberVector()
{
    return theVulnerableMemberVector;
}

QVector<double> RSimpleNeighborhoodModel::getNoVulnerableMemberVector()
{
    return theNoVulnerableMemberVector;
}

int RSimpleNeighborhoodModel::getNumberOfDwellings()
{
    return theTotalNumberOfDwellings;
}

int RSimpleNeighborhoodModel::getNumberOfHabitableDwellings()
{
    return theNumberOfHabitableDwellings;
}


int RSimpleNeighborhoodModel::getNumberOfPeople()
{
    return theTotalNumberOfPeople;
}

int RSimpleNeighborhoodModel::getNumberOfDisplacedPersons()
{
    return theTotalNumberOfPeople - theNumberOfPeopleNotDisplaced;
}

int RSimpleNeighborhoodModel::getNumberOfPermanentlyDisplaced()
{
    return theNumberOfPeopleGone;
}

int RSimpleNeighborhoodModel::getNumberOfPersonsInHotels()
{
    return theNumberOfPeopleInHotels;
}

int RSimpleNeighborhoodModel::getNumberOfPersonsLivingWithFamily()
{
    return theNumberOfPeopleLivingWithTheirFamilies;
}

int RSimpleNeighborhoodModel::getNumberOfPersonsInShelters()
{
    return theNumberOfPeopleSeekingShelter;
}

double RSimpleNeighborhoodModel::getTotalTimeDisplaced()
{
    return theTotalTimeDisplaced;
}

void RSimpleNeighborhoodModel::getRequests()
{

}

QVector<RLocation*> RSimpleNeighborhoodModel::getBuildingLocations()
{
    if (theBuildingLocation.size() == 0){
        theBuildingLocation = theHouseholdPortfolio->getBuildingLocationVector();
    }
    return theBuildingLocation;
}


int RSimpleNeighborhoodModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    RNormalDistribution theNormalDistribution(theDomain);

    // "Functionality vs time" plot
    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
        timeAnalysisBegan = theCurrentTime->getCurrentValue();
    }

    // --------------------------------------------------------------------------------------
    // 1 - Define the building and household archetypes
    //     At this step all characteristics of buildings and households are defined
    // --------------------------------------------------------------------------------------
    if (theNumberOfBuildings < 1){

        theMedianHouseholdIncome          = theHouseholdPortfolio->getMedianHouseholdIncome();

        // Tell the Census Information to define the buildings
        theHouseholdPortfolio->instantiateHouseholds();
        theNumberOfBuildings              = theHouseholdPortfolio->getNumberOfBuildings();

        theBuildingType                   = theHouseholdPortfolio->getBuildingTypesVector();
        theBuildingCodeLevel              = theHouseholdPortfolio->getBuildingCodeLevelVector();
        theBuildingOccupancyClass         = theHouseholdPortfolio->getBuildingOccupancyClassVector();
        theBuildingArea                   = theHouseholdPortfolio->getBuildingAreaVector();
        theBuildingValue                  = theHouseholdPortfolio->getBuildingValueVector();
        theBuildingNumberOfStoreys        = theHouseholdPortfolio->getBuildingStoreysVector();
        theNumberOfDwellingsInTheBuilding = theHouseholdPortfolio->getBuildingDwellingsVector();
        theBuildingLocation               = theHouseholdPortfolio->getBuildingLocationVector();
        theBuildingInsuranceStatus        = theHouseholdPortfolio->getBuildingInsuranceStatus();

        // Tell the Census Information to define the households
        theHouseholdIncome                = theHouseholdPortfolio->getHouseholdIncomeVector();
        theHouseholdMortgage              = theHouseholdPortfolio->getMortgageVector();
        theHouseholdSize                  = theHouseholdPortfolio->getHouseholdSizeVector();
        theHousingTenure                  = theHouseholdPortfolio->getHouseholdTenureVector();
        theCarOwnership                   = theHouseholdPortfolio->getHouseholdCarOwnershipVector();
        theHouseholdAgeCategory           = theHouseholdPortfolio->getHouseholdAgeCategoryVector();
        theHouseholdImmigrantStatus       = theHouseholdPortfolio->getHouseholdImmigrantStatusVector();
        theHouseholdEthnicity             = theHouseholdPortfolio->getHouseholdRaceVector();
        theHouseholdDisabilityStatus      = theHouseholdPortfolio->getHouseholdDisabilityStatus();
        theHouseholdHigherEducationStatus = theHouseholdPortfolio->getHouseholdEducationStatus();

        // initalize all these vectors
        theStructuralDamageState.resize(theNumberOfBuildings);
        theStructuralDamageState.fill(0);

        theNSDDamageState.resize(theNumberOfBuildings);
        theNSDDamageState.fill(0);

        theNSADamageState.resize(theNumberOfBuildings);
        theNSADamageState.fill(0);

        theTotalNumberOfDwellings = 0.0;
        theTotalNumberOfPeople = 0;
        QVector<double> auxTimeDisplaced;
        QVector<QString> auxDestination;

        for (int i = 0; i < theNumberOfBuildings; i++){
            theTotalNumberOfDwellings += theNumberOfDwellingsInTheBuilding.at(i);
            theTotalBuildingArea += theBuildingArea.at(i);

            for (int j = 0; j < theNumberOfDwellingsInTheBuilding.at(i); j++){
                theTotalNumberOfPeople += theHouseholdSize.at(i).at(j);
                auxTimeDisplaced << 0.0;
                auxDestination << QString("NotDisplaced");
            }

            theTimeDisplaced << auxTimeDisplaced;
            theHouseholdDestination << auxDestination;
        }

        // Instantiate the Household Preparedness Estimator
        for (int i = 0; i < theNumberOfBuildings; i++){

            QVector<bool> auxAvailabilityOfGenerator;
            QVector<bool> auxAvailabilityOfWaterSource;
            QVector<QString> disasterDestination;

            for (int j = 0; j < theNumberOfDwellingsInTheBuilding.at(i); j++){

                thePreparednessEstimator->setTenure(theHousingTenure.at(i).at(j));
                thePreparednessEstimator->setIncome(theHouseholdIncome.at(i).at(j));
                thePreparednessEstimator->setImmigrationStatus(theHouseholdImmigrantStatus.at(i).at(j));
                thePreparednessEstimator->setSize(theHouseholdSize.at(i).at(j));
                thePreparednessEstimator->setChildren(false);
                thePreparednessEstimator->setElders(false);

                if (theHouseholdAgeCategory.at(i).at(j) == QString("Children")){
                    thePreparednessEstimator->setChildren(true);
                }
                if (theHouseholdAgeCategory.at(i).at(j) == QString("Elderly")){
                    thePreparednessEstimator->setElders(true);
                }

                // Alternative power source
                auxAvailabilityOfGenerator << thePreparednessEstimator->getGeneratorAvailability();

                // Alternative water source
                auxAvailabilityOfWaterSource << thePreparednessEstimator->getWaterAvailability();

                // Hotel
                disasterDestination << thePreparednessEstimator->getDisasterDestination();

            } //j

            theAvailabilityOfGenerator << auxAvailabilityOfGenerator;
            theAvailabilityOfAlternativeWaterSource << auxAvailabilityOfWaterSource;
            theDisasterDestination << disasterDestination;
        } // i
    } // if


    // --------------------------------------------------------------------------------------
    // 2 - Evaluate the availability of water and power to the neighborhood
    // --------------------------------------------------------------------------------------
    // ** Power needed
    double powerConsumedByResidentialUnitsInSanFranciscoIn2018 = 1462000.0 / 365.0 / 24.0; // [MWh] https://ecdms.energy.ca.gov/elecbycounty.aspx
    int totalDwellingsInSanFrancisco = 360725; // According to SimplyAnalitics
    double powerNeededInThisCT = 0.0;// powerConsumedByResidentialUnitsInSanFranciscoIn2018 * theTotalNumberOfDwellings / static_cast<double>(totalDwellingsInSanFrancisco);

    double timeUntilWaterIsRestored = 0.0;
    double timeUntilPowerIsRestored = 0.0;

    // Go over all the responses getting to this object and take actions accordingly
    // Check if power is available in the residential the (i.e., not at the residential building)
    for (int i = 0; i < thePowerSupplierList.size(); i++){

        // Cast the input as a response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(thePowerSupplierList.at(i));

        if (theResponseComingIntoThisModel){

            // Check for availability of power
            // Power is false if the response > 0 because the response means 'time until repairs are finished'
            if (theResponseComingIntoThisModel->getCurrentValue() <= 0){
                theAvailabilityOfPower = true;

                // qCritical() << "Power received:" << theResponseComingIntoThisModel->getCurrentValue() << "Power needs:" << powerNeededInThisCT;
            } else {
                timeUntilPowerIsRestored = theResponseComingIntoThisModel->getCurrentValue();
                if (theOutputLevel == RDomain::Maximum){
                    qDebug() << this->objectName() << "does not have electrical power available from" << theResponseComingIntoThisModel->getModel()->objectName()  << "days. Expected restoration in"  << theResponseComingIntoThisModel->getCurrentValue() << "";
                }
            }
        }
    }

    // Check if water is available in the residential the (i.e., not at the residential building)
    for (int i = 0; i < theWaterSupplierList.size(); i++){

        // Cast the input as a response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(theWaterSupplierList.at(i));

        if (theResponseComingIntoThisModel){

            // Check for availability of water
            // Water is false if the response > 0 because the response means 'time until repairs are finished'
            if (theResponseComingIntoThisModel->getCurrentValue() <= 0.0){
                theAvailabilityOfWater = true;

            } else {
                timeUntilWaterIsRestored = theResponseComingIntoThisModel->getCurrentValue();

                if (theOutputLevel == RDomain::Maximum){
                    qDebug() << this->objectName() << "does not have water available from" << theResponseComingIntoThisModel->getModel()->objectName()  << "days. Expected restoration in"  << theResponseComingIntoThisModel->getCurrentValue() << "";
                }
            }
        }
    }


    // Uncomment this to eliminate the need for power and water
    //theWaterShortageDuration = 0.0;
    //thePowerShortageDuration = 0.0;


    int totalSF = 0;
    int totalMF = 0;

    // --------------------------------------------------------------------------------------
    // 3 - Calculate everything that happens at the time of the earthquake (t = t_0)
    // --------------------------------------------------------------------------------------
    // Immediate losses
    // Considering only one earthquake in this analysis - think about aftershocks later
    if (timeOfEvent == 0.0){

        // Morbidity Estimator
        theBuildingModel = qobject_cast<RHazusBuildingModel*>(theBuildingModelResponse->getModel());

        // ******
        RNetworkSamplingModel * theSamplingModel = theDomain->findChild<RNetworkSamplingModel*>();
        double theMultiplier = theSamplingModel->getSamplingMultiplier();
        // ******

        // For all the buildings
        for (int i = 0; i < theNumberOfBuildings; i++){

            QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());
            double Sa0_3 = theSa0_3Vector.at(theSa03Counter);

            QVector<double> theSa1Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa1Intensity());
            double Sa1 =  theSa1Vector.at(theSa1Counter);

            // ***********
            //QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());
            //double Sa0_3 = theMultiplier * theSa0_3Vector.at(0);//theSa0_3Vector.at(theSa03Counter);

            //QVector<double> theSa1Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa1Intensity());
            //double Sa1 =  theMultiplier * theSa1Vector.at(0);//theSa1Vector.at(theSa1Counter);
            // ***********


            // Give the building model the properties of this building
            theBuildingModel->setBuildingType(theBuildingType.at(i));
            theBuildingModel->setCodeDesignLevel(theBuildingCodeLevel.at(i));
            theBuildingModel->setOccupancyClass(theBuildingOccupancyClass.at(i));
            theBuildingModel->setSa1(Sa1);
            theBuildingModel->setSa2(Sa0_3);
            theBuildingModel->evaluateModel();

            // Get the damage responses
            theStructuralDamageState[i] = theBuildingModel->getStrDS();
            theNSDDamageState[i] = theBuildingModel->getNSDDS();
            theNSADamageState[i] = theBuildingModel->getNSADS();
            theAverageDamageState += theStructuralDamageState[i];

            // Considering only one earthquake in this analysis - think about aftershocks later
            if (theStructuralDamageState[i] > 0.0 || theNSDDamageState.at(i) > 0.0 || theNSADamageState.at(i) > 0.0){
                timeOfEvent = theCurrentTime->getCurrentValue();
            } // if DS > 0

            QVector<bool> auxHouseholdFatality;
            // For all dwellings in this building
            for (int j = 0; j < theNumberOfDwellingsInTheBuilding.at(i); j++){
                theMorbidityEstimator->setTime(remainder(theCurrentTime->getCurrentValue(), qFloor(theCurrentTime->getCurrentValue())));
                theMorbidityEstimator->setBuildingType(theBuildingType.at(i));
                theMorbidityEstimator->setDamageState(theStructuralDamageState.at(i));
                theMorbidityEstimator->setIncome(theHouseholdIncome.at(i).at(j));
                auxHouseholdFatality << theMorbidityEstimator->estimateFatalities();
            } // j

            if (theBuildingOccupancyClass.at(i) == QString("RES1")){
                totalSF+= theNumberOfDwellingsInTheBuilding.at(i);
            } else {
                totalMF+= theNumberOfDwellingsInTheBuilding.at(i);
            }

            theHouseholdFatalityIncidence << auxHouseholdFatality;

        } // i

        // Calculate the Percentages of households being in each Damage States (0 to 4)
        for (int i = 0; i < theNumberOfBuildings; i++){

            if (theStructuralDamageState.at(i) == 0){
                thePercentageOfBuildingsAtDS0 += theNumberOfDwellingsInTheBuilding.at(i);
                thePercentageOfDwellingsAtDS0 += 1;
            }

            if (theStructuralDamageState.at(i) == 1){
                thePercentageOfBuildingsAtDS1 += theNumberOfDwellingsInTheBuilding.at(i);
                thePercentageOfDwellingsAtDS1 += 1;
            }

            if (theStructuralDamageState.at(i) == 2){
                thePercentageOfBuildingsAtDS2 += theNumberOfDwellingsInTheBuilding.at(i);
                thePercentageOfDwellingsAtDS2 += 1;
            }

            if (theStructuralDamageState.at(i) == 3){
                thePercentageOfBuildingsAtDS3 += theNumberOfDwellingsInTheBuilding.at(i);
                thePercentageOfDwellingsAtDS3 += 1;
            }

            if (theStructuralDamageState.at(i) == 4){
                thePercentageOfBuildingsAtDS4 += theNumberOfDwellingsInTheBuilding.at(i);
                thePercentageOfDwellingsAtDS4 += 1;
            }
        }// all buildings

        theAverageDamageState /= theNumberOfBuildings;
        thePercentageOfDwellingsAtDS0 /= theNumberOfBuildings;
        thePercentageOfDwellingsAtDS1 /= theNumberOfBuildings;
        thePercentageOfDwellingsAtDS2 /= theNumberOfBuildings;
        thePercentageOfDwellingsAtDS3 /= theNumberOfBuildings;
        thePercentageOfDwellingsAtDS4 /= theNumberOfBuildings;
        thePercentageOfBuildingsAtDS0 /= theTotalNumberOfDwellings;
        thePercentageOfBuildingsAtDS1 /= theTotalNumberOfDwellings;
        thePercentageOfBuildingsAtDS2 /= theTotalNumberOfDwellings;
        thePercentageOfBuildingsAtDS3 /= theTotalNumberOfDwellings;
        thePercentageOfBuildingsAtDS4 /= theTotalNumberOfDwellings;

    } // if time


    // Rebuilding processes
    totalHabitableArea = theTotalBuildingArea;
    theNumberOfHabitableDwellings = theTotalNumberOfDwellings;
    double theImpactToPreCodeBuildings = 0.0;
    double theImpactToLowCodeBuildings = 0.0;
    double theImpactToModerateCodeBuildings = 0.0;
    double theImpactToHighCodeBuildings = 0.0;
    double theImpactToSingleFamilyBuildings = 0;
    double theImpactToMultiFamilyBuildings = 0;
    double theImpactToOwners = 0.0;
    double theImpactToRenters = 0.0;
    double theImpactToLowIncome = 0.0;
    double theImpactToModerateIncome = 0.0;
    double theImpactToHighIncome = 0.0;
    double theImpactToRecentImmigrants = 0.0;
    double theImpactToEstablishedImmigrants = 0.0;
    double theImpactToNonImmigrants = 0.0;
    double theImpactToWhiteHouseholds = 0.0;
    double theImpactToBlackHouseholds = 0.0;
    double theImpactToHispanicHouseholds = 0.0;
    double theImpactToAsianHouseholds = 0.0;
    double theImpactToNativeAmericanHouseholds = 0.0;
    double theImpactToHouseholdsWithVulnerableMembers = 0.0;
    double theImpactToHouseholdsWithoutVulnerableMembers = 0.0;
    double theImpactOfFunctionalHabitability = 0.0;
    double theImpactOfPerceivedHabitability = 0.0;

    int theCount = 0;
    // --------------------------------------------------------------------------------------
    // 4 - Calculate everything that happens after the time of the earthquake (t > t_0)
    // --------------------------------------------------------------------------------------
    // For all the buildings
    if (timeOfEvent == theCurrentTime->getCurrentValue()){

        theNumberOfPeopleNotDisplaced = theTotalNumberOfPeople;

        for (int i = 0; i < theNumberOfBuildings; i++){
            for (int j = 0; j <theNumberOfDwellingsInTheBuilding.at(i); j++){

                // Forcing a reset of these vectors
                theHouseholdDestination[i][j] = QString("NotDisplaced");

                // Provide the displacement estimator all the information needed to evaluate the status of the household
                theDisplacementEstimator->setDS(theStructuralDamageState.at(i));
                theDisplacementEstimator->setBuildingOccupancy(theBuildingOccupancyClass.at(i));
                theDisplacementEstimator->setAvailabilityOfGenerator(theAvailabilityOfGenerator.at(i).at(j));
                theDisplacementEstimator->setAvailabilityOfBottledWater(theAvailabilityOfAlternativeWaterSource.at(i).at(j));
                theDisplacementEstimator->setDaysWithoutPower(timeUntilPowerIsRestored); // **** Fix me
                theDisplacementEstimator->setDaysWithoutWater(timeUntilWaterIsRestored); // **** Fix me
                theDisplacementEstimator->setAge(theHouseholdAgeCategory.at(i).at(j));
                theDisplacementEstimator->setRace(theHouseholdEthnicity.at(i).at(j));
                theDisplacementEstimator->setDisabilityStatus(theHouseholdDisabilityStatus.at(i).at(j));
                theDisplacementEstimator->setHigherEdStatus(theHouseholdHigherEducationStatus.at(i).at(j));
                theDisplacementEstimator->setAverageDamageState(theAverageDamageState);
                theDisplacementEstimator->setWeatherConditions(theWeatherConditions);
                theDisplacementEstimator->setCarOwnership(theCarOwnership.at(i).at(j));
                theDisplacementEstimator->setInsuranceStatus(theBuildingInsuranceStatus.at(i));
                theDisplacementEstimator->setDisasterDestination(theDisasterDestination.at(i).at(j));
                theDisplacementEstimator->setTenure(theHousingTenure.at(i).at(j));
                theDisplacementEstimator->setIncome(theHouseholdIncome.at(i).at(j));
                theDisplacementEstimator->setSize(theHouseholdSize.at(i).at(j));

                // Evaluate the displacement status of the household
                bool theDisplacementStatus = false; //theDisplacementEstimator->evaluateDisplacementStatus();

                double myDisplacementTime = theDisplacementEstimator->evaluateDisplacementDuration();

                if (myDisplacementTime > 0){
                    theDisplacementStatus = true;
                }

                // Calculate the expected displacement time using data at t=0
                if (theDisplacementStatus == true){
                   theTotalTimeDisplaced += myDisplacementTime;
                }

                // Calculate the cumulative displacement time - i.e., it increases day by day
                if (theDisplacementStatus == true){
                    theRealTimeDisplaced += theHouseholdSize.at(i).at(j) * (theCurrentTime->getCurrentValue() - lastEvaluationTime);
                }

                // If the household is displaced, figure out its immediate destination
                if (theDisplacementStatus == true){
                    theHouseholdDestination[i][j] = theDisplacementEstimator->evaluateShortTermDestination();
                }

                // Shelter needs
                if (theHouseholdDestination.at(i).at(j) == QString("PublicShelter")){
                    theNumberOfPeopleSeekingShelter += theHouseholdSize.at(i).at(j);
                }

                // Hotels or rental units
                if (theHouseholdDestination.at(i).at(j) == QString("RentalUnit")){
                    theNumberOfPeopleInHotels += theHouseholdSize.at(i).at(j);
                }

                // Living with families
                if (theHouseholdDestination.at(i).at(j) == QString("Relatives")){
                    theNumberOfPeopleLivingWithTheirFamilies += theHouseholdSize.at(i).at(j);
                }


                // Displaced
                if (theHouseholdDestination.at(i).at(j) != QString("NotDisplaced")){
                //if (theHouseholdDestination.at(i).at(j) == QString("PublicShelter")){

                    // BuildingType
                    if (theBuildingOccupancyClass.at(i) == QString("RES1") || theBuildingOccupancyClass.at(i) == QString("MH")){
                        theImpactToSingleFamilyBuildings += myDisplacementTime;
                    }
                    else {
                        theImpactToMultiFamilyBuildings += myDisplacementTime;
                    }

                    // Building design code
                    if (theBuildingCodeLevel.at(i) == QString("Pre")){
                        theImpactToPreCodeBuildings += myDisplacementTime;
                    }
                    else if (theBuildingCodeLevel.at(i) == QString("Low")){
                        theImpactToLowCodeBuildings += myDisplacementTime;
                    }
                    else if (theBuildingCodeLevel.at(i) == QString("Moderate")){
                        theImpactToModerateCodeBuildings += myDisplacementTime;
                    }
                    else if (theBuildingCodeLevel.at(i) == QString("High")){
                        theImpactToHighCodeBuildings += myDisplacementTime;
                    }

                    // Tenure
                    if (theHousingTenure.at(i).at(j) == QString("RenterOccupied")){
                        theImpactToRenters += myDisplacementTime;

                    } else if (theHousingTenure.at(i).at(j) == QString("OwnerOccupied")){
                        theImpactToOwners += myDisplacementTime;
                    }

                    // Income
                    if (theHouseholdIncome.at(i).at(j) == QString("Low")){
                        theImpactToLowIncome += myDisplacementTime;
                        theCount++;
                    }
                    else if (theHouseholdIncome.at(i).at(j) == QString("Moderate")){
                        theImpactToModerateIncome += myDisplacementTime;
                    }
                    else if (theHouseholdIncome.at(i).at(j) == QString("High")){
                        theImpactToHighIncome += myDisplacementTime;
                    }


                    // Immigrant status
                    if (theHouseholdImmigrantStatus.at(i).at(j) == QString("RecentImmigrant")){
                        theImpactToRecentImmigrants += myDisplacementTime;
                    }
                    else if (theHouseholdImmigrantStatus.at(i).at(j) == QString("EstablishedImmigrant")){
                        theImpactToEstablishedImmigrants += myDisplacementTime;
                    }
                    else if (theHouseholdImmigrantStatus.at(i).at(j) == QString("NonImmigrant")){
                        theImpactToNonImmigrants += myDisplacementTime;
                    }


                    // Ethnicity
                    if (theHouseholdEthnicity.at(i).at(j) == QString("White")){
                        theImpactToWhiteHouseholds += myDisplacementTime;
                    }
                    else if (theHouseholdEthnicity.at(i).at(j) == QString("Black")){
                        theImpactToBlackHouseholds += myDisplacementTime;
                    }
                    else if (theHouseholdEthnicity.at(i).at(j) == QString("Hispanic")){
                        theImpactToHispanicHouseholds += myDisplacementTime;

                    } else if (theHouseholdEthnicity.at(i).at(j) == QString("Asian")){
                        theImpactToAsianHouseholds += myDisplacementTime;

                    } else if (theHouseholdEthnicity.at(i).at(j) == QString("NativeAmerican")){
                        theImpactToNativeAmericanHouseholds += myDisplacementTime;
                    }


                    // Age
                    if (theHouseholdAgeCategory.at(i).at(j) != QString("None")){
                        theImpactToHouseholdsWithVulnerableMembers += myDisplacementTime;
                    }
                    else {
                        theImpactToHouseholdsWithoutVulnerableMembers += myDisplacementTime;
                    }


                    // Reason for displacement
                    QString theReason = theDisplacementEstimator->getReasonForDisplacement();
                    if (theReason == QString("FunctionalHabitability")){
                        theImpactOfFunctionalHabitability += myDisplacementTime;
                    } else if (theReason == QString("PerceivedHabitability")){
                        theImpactOfPerceivedHabitability += myDisplacementTime;
                    } else {
                        //qCritical() << this->objectName() << "cannot identify the reason" << theReason << "why this household is displaced!";
                    }
                }
            } // j
        } // i

        theNumberOfPeopleNotDisplaced -= (theNumberOfPeopleInHotels + theNumberOfPeopleGone + theNumberOfPeopleLivingWithTheirFamilies + theNumberOfPeopleSeekingShelter);
       // qCritical() << this->objectName() << "Days x displaced persons = " << theTotalTimeDisplaced << "";

    } // if something happened


    theResponse->setCurrentValue(theTotalTimeDisplaced);

    // -------------------------------------------------------------------------------------------
    // 5 - Create plots
    // -------------------------------------------------------------------------------------------
    // If it has been more than one day since last plot
    timeSinceLastPlot += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    lastEvaluationTime = theCurrentTime->getCurrentValue();
    double maximumTimeBetweenPlots;

    if(theCurrentTime->getCurrentValue() - timeOfEvent < 30.0){
        maximumTimeBetweenPlots = 2.0;
    }
    else {
        maximumTimeBetweenPlots = 7.0;
    }


    if (timeSinceLastPlot >= maximumTimeBetweenPlots || qAbs(timeOfEvent - theCurrentTime->getCurrentValue()) < 0.01){

        if (theOutputLevel == RDomain::Maximum){
            qDebug() << "";
            qDebug() << "-----------------------------------------------------";
            qDebug() << "             " << this->objectName();
            qDebug() << " | " << "Number of dwellings in need of repairs:" << theNumberOfHabitableDwellings << "|  Time since event: " << theCurrentTime->getCurrentValue() - timeOfEvent;
            qDebug() << " | " << "Number of persons in shelters:" << theNumberOfPeopleSeekingShelter;
            qDebug() << " | " << "Number of persons in hotels:" << theNumberOfPeopleInHotels;
            qDebug() << " | " << "Number of persons living with their families:" << theNumberOfPeopleLivingWithTheirFamilies;
            qDebug() << " | " << "Number of persons permanently displaced:" << theNumberOfPeopleGone;
            qDebug() << "-----------------------------------------------------";
            qDebug() << "";
        }

        this->plotUsableBuildings(theNumberOfHabitableDwellings);
        //this->plotUsableBuildings(theNumberOfPeopleSeekingShelter, theNumberOfPeopleLivingWithTheirFamilies, theNumberOfPeopleInHotels, theNumberOfPeopleTakingShelterOnSite, QString("People on Public Shelters"), QString("People Living With Family"), QString("People in Hotels"), QString("People taking on-site shelter"));
        //this->plotUsableBuildings(theNumberOfPeopleInHotels, theNumberOfPeopleGone, theNumberOfPeopleTakingShelterOnSite, QString("People on Hotels"), QString("People Gone"), QString("People taking on-site shelter"));

        timeSinceLastPlot = 0.0;
        timeVector << lastEvaluationTime - timeAnalysisBegan;
        theHousingRecoveryVector << theNumberOfHabitableDwellings;
        theDisplacedPopulationVector << theTotalNumberOfPeople - theNumberOfPeopleNotDisplaced;
        thePersonsWithFamilyVector << theNumberOfPeopleLivingWithTheirFamilies;
        thePersonsInShelterVector << theNumberOfPeopleSeekingShelter;
        thePersonsInRenterHousingVector << theNumberOfPeopleInHotels;
        thePersonsGoneVector << theNumberOfPeopleGone;

        theSFVector << theImpactToSingleFamilyBuildings;
        theMFVector << theImpactToMultiFamilyBuildings;
        thePreCodeVector << theImpactToPreCodeBuildings;
        theLowCodeVector << theImpactToLowCodeBuildings;
        theModCodeVector << theImpactToModerateCodeBuildings;
        theHighCodeVector << theImpactToHighCodeBuildings;
        theRenterVector << theImpactToRenters;
        theOwnerVector << theImpactToOwners;
        thePoorVector << theImpactToLowIncome;
        theModerateVector << theImpactToModerateIncome;
        theRichVector << theImpactToHighIncome;
        theRecentVector << theImpactToRecentImmigrants;
        theEstablishedVector << theImpactToEstablishedImmigrants;
        theResidentVector << theImpactToNonImmigrants;
        theWhiteVector << theImpactToWhiteHouseholds;
        theBlackVector << theImpactToBlackHouseholds;
        theHispanicVector << theImpactToHispanicHouseholds;
        theAsianVector << theImpactToAsianHouseholds;
        theNativeAmericanVector << theImpactToNativeAmericanHouseholds;
        theVulnerableMemberVector << theImpactToHouseholdsWithVulnerableMembers;
        theNoVulnerableMemberVector << theImpactToHouseholdsWithoutVulnerableMembers;
        theFunctionalHabitabilityVector << theImpactOfFunctionalHabitability;
        thePerceivedHabitabilityVector << theImpactOfPerceivedHabitability;

    }

    return 1;
}


void RSimpleNeighborhoodModel::plotUsableBuildings(double habitableArea)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (buildingRepairsPlot == nullptr){
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

        theTickVector << timeElapsed;
        theLabelVector << QString("%1").arg(static_cast<int>(timeElapsed));
        buildingRepairsPlot->xAxis->setTickVector(theTickVector);
        buildingRepairsPlot->xAxis->setTickVectorLabels(theLabelVector);
        buildingRepairsPlot->replot();
    }
}



void RSimpleNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, QString var1Name, QString var2Name, QString var3Name)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (displacedPopulationPlot == nullptr) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            displacedPopulationPlot = new QCustomPlot();
            QString theString = QString("Total inhabitable residential building area %1").arg(this->objectName());
            mainWindow->addSubWindow(displacedPopulationPlot, theString, true);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            displacedPopulationPlot->graph(0)->setPen(QColor(0, 255, 0, 255));
            displacedPopulationPlot->graph(0)->setName(var1Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
            displacedPopulationPlot->graph(1)->setPen(QColor(0, 0, 255, 255));
            displacedPopulationPlot->graph(1)->setName(var2Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 7));
            displacedPopulationPlot->graph(2)->setPen(QColor(255, 0, 0, 255));
            displacedPopulationPlot->graph(2)->setName(var3Name);
            displacedPopulationPlot->xAxis->setLabel("Time (days)");
            displacedPopulationPlot->yAxis->setLabel(theString);
            displacedPopulationPlot->xAxis->setAutoTicks(false);
            displacedPopulationPlot->xAxis->setAutoTickLabels(false);
            displacedPopulationPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            displacedPopulationPlot->legend->setFont(legendFont);
            displacedPopulationPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            displacedPopulationPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            displacedPopulationPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        displacedPopulationPlot->graph(0)->addData(timeElapsed, var1);
        displacedPopulationPlot->graph(1)->addData(timeElapsed, var2);
        displacedPopulationPlot->graph(2)->addData(timeElapsed, var3);
        displacedPopulationPlot->yAxis->setRange(0, theTotalNumberOfPeople);
        displacedPopulationPlot->xAxis->setRange(0, timeElapsed + 1);
        displacedPopulationPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        theTickVector << timeElapsed;
        theLabelVector << QString("%1").arg(static_cast<int>(timeElapsed));
        displacedPopulationPlot->xAxis->setTickVector(theTickVector);
        displacedPopulationPlot->xAxis->setTickVectorLabels(theLabelVector);
        displacedPopulationPlot->replot();
    }
}


void RSimpleNeighborhoodModel::plotDisplacedPopulation(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name)
{
    // Number of Repaired Buildings
    if (theOutputLevel >= RObject::Medium) {

        if (displacedPopulationPlot == nullptr) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            displacedPopulationPlot = new QCustomPlot();
            QString theString = QString("Total inhabitable residential building area %1").arg(this->objectName());
            mainWindow->addSubWindow(displacedPopulationPlot, theString, true);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            displacedPopulationPlot->graph(0)->setPen(QColor(0, 255, 0, 255));
            displacedPopulationPlot->graph(0)->setName(var1Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, 7));
            displacedPopulationPlot->graph(1)->setPen(QColor(0, 0, 255, 255));
            displacedPopulationPlot->graph(1)->setName(var2Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(2)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross, 7));
            displacedPopulationPlot->graph(2)->setPen(QColor(255, 0, 0, 255));
            displacedPopulationPlot->graph(2)->setName(var3Name);
            displacedPopulationPlot->addGraph();
            displacedPopulationPlot->graph(3)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangle, 7));
            displacedPopulationPlot->graph(3)->setPen(QColor(255, 0, 255, 255));
            displacedPopulationPlot->graph(3)->setName(var4Name);
            displacedPopulationPlot->xAxis->setLabel("Time (days)");
            displacedPopulationPlot->yAxis->setLabel(theString);
            displacedPopulationPlot->xAxis->setAutoTicks(false);
            displacedPopulationPlot->xAxis->setAutoTickLabels(false);
            displacedPopulationPlot->legend->setVisible(true);
            QFont legendFont;// = font();  // start out with MainWindow's font..
            legendFont.setPointSize(9); // and make a bit smaller for legend
            displacedPopulationPlot->legend->setFont(legendFont);
            displacedPopulationPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
            // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
            displacedPopulationPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
            displacedPopulationPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        }

        double timeElapsed = lastEvaluationTime - timeAnalysisBegan;

        displacedPopulationPlot->graph(0)->addData(timeElapsed, var1);
        displacedPopulationPlot->graph(1)->addData(timeElapsed, var2);
        displacedPopulationPlot->graph(2)->addData(timeElapsed, var3);
        displacedPopulationPlot->graph(3)->addData(timeElapsed, var4);
        displacedPopulationPlot->yAxis->setRange(0, theTotalNumberOfPeople);
        displacedPopulationPlot->xAxis->setRange(0, timeElapsed + 1);
        displacedPopulationPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignCenter|Qt::AlignTop);

        theTickVector << timeElapsed;
        theLabelVector << QString("%1").arg((int) timeElapsed);
        displacedPopulationPlot->xAxis->setTickVector(theTickVector);
        displacedPopulationPlot->xAxis->setTickVectorLabels(theLabelVector);
        displacedPopulationPlot->replot();
    }
}

//// Tell that intensity model what we want
//theIntensityModel->setLocation(theBuildingLocation.at(i)); //theBuildingLocation.at(i)
//theIntensityModel->setIntensityMeasure(QString("Sa"));

//// Run the intensity model and get the response
//theIntensityModel->setPeriod(0.3);
//theIntensityModel->evaluateModel();
//double Sa0_3 = theResponseObject->getCurrentValue();

//theIntensityModel->setPeriod(1.0);
//theIntensityModel->evaluateModel();
//double Sa1 = theResponseObject->getCurrentValue();
