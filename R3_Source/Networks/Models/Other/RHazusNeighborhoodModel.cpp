#include "RHazusNeighborhoodModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RMainWindow.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"
#include "RNormalDistribution.h"
#include "RIntensityModel.h"
#include "RMCEERShelterNeedsEstimator.h"
#include "RAmericanHousingSurveyPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"
#include "RSimpleShelterNeedsEstimator.h"
#include <math.h>

RHazusNeighborhoodModel::RHazusNeighborhoodModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theNumberOfBuildings = 0;
    theTotalNumberOfDwellings = 0;
    theNumberOfPeopleTimesDaysDisplaced = 0.0;
    theNumberOfPeopleRequiringShortermHousing = 0.0;
    theSa03Counter = -1;
    theSa1Counter = -1;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);

}

RHazusNeighborhoodModel::~RHazusNeighborhoodModel()
{
}

int RHazusNeighborhoodModel::resetTime()
{
    // Reset data member on each new scenario
    timeOfEvent = 0.0;
    lastEvaluationTime = 0.0;
    theAverageDamageState = 0.0;
    thePercentageSingleFamilyModerateDS = 0.0;
    thePercentageSingleFamilyExtensiveDS = 0.0;
    thePercentageSingleFamilyCompleteDS = 0.0;
    thePercentageMultiFamilyModerateDS = 0.0;
    thePercentageMultiFamilyExtensiveDS = 0.0;
    thePercentageMultiFamilyCompleteDS = 0.0;

    thePercentageLowIncome = 0.0;
    thePercentageLowIncomePerson = 0.0;
    thePercentageModerateIncome = 0.0;
    thePercentageModerateIncomePerson = 0.0;
    thePercentageHighIncome = 0.0;
    thePercentageHighIncomePerson = 0.0;
    thePercentageRecentImmigrants = 0.0;
    thePercentageRecentImmigrantsPerson = 0.0;
    thePercentageEstablishedImmigrants =0.0;
    thePercentageEstablishedImmigrantsPerson = 0.0;
    thePercentageResidents = 0.0;
    thePercentageResidentsPerson = 0.0;

    theNumberOfSingleFamilyDwellings = 0;
    theNumberOfMultiFamilyDwellings = 0;
    theNumberOfSingleFamilyBuildings = 0;
    theNumberOfMultiFamilyBuildings = 0;

    thePercentageOfHI1 = 0.0;
    thePercentageOfHI2 = 0.0;
    thePercentageOfHI3 = 0.0;
    thePercentageOfHI4 = 0.0;
    thePercentageOfHI5 = 0.0;

    thePercentageOfHO1 = 0.0;
    thePercentageOfHO2 = 0.0;

    thePercentageDS0 = 0.0;
    thePercentageDS1 = 0.0;
    thePercentageDS2 = 0.0;
    thePercentageDS3 = 0.0;
    thePercentageDS4 = 0.0;
    theDS0 = 0.0;
    theDS1 = 0.0;
    theDS2 = 0.0;
    theDS3 = 0.0;
    theDS4 = 0.0;

    theHIVector.clear();
    theHEVector.clear();
    theHOVector.clear();
    theHAVector.clear();

    theIncomeModificationFactors.clear();
    theEthnicityModificationFactors.clear();
    theOwnershipModificationFactors.clear();
    theAgeModificationFactors.clear();

    theWeightFactorsOfSingleFamily.clear();
    theWeightFactorsOfMultiFamily.clear();
    thePercentageOfDisplacedSingleFamilyHouseholds = 0.0;
    thePercentageOfDisplacedMultiFamilyHouseholds = 0.0;
    thePercentageOfHouseholdWithChild = 0.0;
    thePercentageOfHouseholdWithSenior = 0.0;

    // Not the most ellegant solution
    // PGA [0 , 1 , 2 ..., n]

    QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());

    if (thePGACounter > (theSa0_3Vector.size() - 1)){
        qCritical() << this->objectName() << "Hey, I do not have enough Sa values!";
        thePGACounter = 0;
    }

    theSa03Counter++;
    theSa1Counter++;
    theStructuralDamageState.clear();
    theStructuralDamageState.resize(theNumberOfBuildings);
    theStructuralDamageState.fill(0);

    qCritical() << this->objectName() << ":";
    qCritical() << "The Counter for SA(T=0.3s)" <<theSa03Counter ;

    return 0;
}


QObject * RHazusNeighborhoodModel::getHouseholdPortfolio() const
{
    return theHouseholdPortfolio;
}

void RHazusNeighborhoodModel::setHouseholdPortfolio(QObject *value)
{
    theHouseholdPortfolio = qobject_cast<RHouseholdPortfolio*>(value);
}

QObject* RHazusNeighborhoodModel::getBuildingDamageModel()
{
    return theBuildingModelResponse;
}

void RHazusNeighborhoodModel::setBuildingDamageModel(QObject* value)
{
    theBuildingModelResponse= qobject_cast<RResponse*>(value);
}


int RHazusNeighborhoodModel::getNumberOfDwellings()
{
    return theTotalNumberOfDwellings;
}


int RHazusNeighborhoodModel::getNumberOfPeople()
{
    return theTotalNumberOfPeople;
}


int RHazusNeighborhoodModel::getNumberOfPeopleRequiringShortermHousing()
{
    return static_cast<int>(theNumberOfPeopleRequiringShortermHousing);
}

double RHazusNeighborhoodModel::getNumberOfPeopleTimesDaysDisplaced()
{
    return theNumberOfPeopleTimesDaysDisplaced;
}

int RHazusNeighborhoodModel::evaluateModel(RGradientType theGradientType)
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    RNormalDistribution theNormalDistribution(theDomain);

    if (lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }

    // -------------------------------------------------------------------------------------------
    // 1 - Get information from census tract
    // -------------------------------------------------------------------------------------------

    if (theNumberOfBuildings < 1){

        theMedianHouseholdIncome          = theHouseholdPortfolio->getMedianHouseholdIncome();

        // Tell the Census Information to define the buildings
        theHouseholdPortfolio->instantiateHouseholds();

        theNumberOfBuildings              = theHouseholdPortfolio->getNumberOfBuildings();
        theBuildingType                   = theHouseholdPortfolio->getBuildingTypesVector();
        theCodeLevel                      = theHouseholdPortfolio->getBuildingCodeLevelVector();
        theOccupancyClass                 = theHouseholdPortfolio->getBuildingOccupancyClassVector();
        theNumberOfDwellingsInTheBuilding = theHouseholdPortfolio->getBuildingDwellingsVector();

        // Tell the Census Information to define the households
        theHouseholdIncome                = theHouseholdPortfolio->getHouseholdHouseholdIncomeVector();
        theHouseholdSize                  = theHouseholdPortfolio->getHouseholdSizeVector();
        theHousingTenure                  = theHouseholdPortfolio->getHouseholdTenureVector();
        theHouseholdAgeCategory           = theHouseholdPortfolio->getHouseholdAgeCategoryVector();
        theHouseholdImmigrantStatus       = theHouseholdPortfolio->getHouseholdImmigrantStatusVector();
        HouseholdSizeDistribution         = stringToDoubleVector(theHouseholdPortfolio->getHouseholdSizeDistribution());
        thePercentageLowIncome            = theHouseholdPortfolio->getPercentageLowIncome();
        thePercentageModerateIncome       = theHouseholdPortfolio->getPercentageModerateIncome();
        thePercentageHighIncome           = theHouseholdPortfolio->getPercentageHighIncome();
        thePercentageRecentImmigrants     = theHouseholdPortfolio->getPercentageOfRecentImmigrants();
        thePercentageEstablishedImmigrants= theHouseholdPortfolio->getPercentageOfEstablishedImmigrants();
        thePercentageResidents            = 1.0 - thePercentageRecentImmigrants - thePercentageEstablishedImmigrants;




        //not used for now
        //        thePercentageLowIncome = theHouseholdPortfolio->getPercentageLowIncome();
        //        thePercentageModerateIncome = theHouseholdPortfolio->getPercentageModerateIncome();
        //        thePercentageHighIncome = theHouseholdPortfolio->getPercentageHighIncome();

        theNumberOfSingleFamilyBuildings  = theHouseholdPortfolio->getNumberOfSingleFamilyBuildings();
        theNumberOfMultiFamilyBuildings   = theHouseholdPortfolio->getNumberOfMultiFamilyBuildings();
        theNumberOfDwellingsInTheBuilding = theHouseholdPortfolio->getBuildingDwellingsVector();
        theHouseholdSize                  = theHouseholdPortfolio->getHouseholdSizeVector();


        // initalize all these vectors
        theStructuralDamageState.resize(theNumberOfBuildings);
        theStructuralDamageState.fill(0);

        theTotalNumberOfDwellings = 0;
        theTotalNumberOfPeople = 0;


        // Calculate total number of dwellings, total number of people in the census tract, the percentage of households in each age categories

        for (int i = 0; i < theNumberOfBuildings; i++){
            theTotalNumberOfDwellings += theNumberOfDwellingsInTheBuilding.at(i);
            for (int j = 0; j < theNumberOfDwellingsInTheBuilding.at(i); j++){
                theTotalNumberOfPeople += theHouseholdSize.at(i).at(j);
                if (theHouseholdAgeCategory.at(i).at(j) == QString("Children")){
                    thePercentageOfHouseholdWithChild += 1;
                }
                if (theHouseholdAgeCategory.at(i).at(j) == QString("Senior")){
                    thePercentageOfHouseholdWithSenior += 1;
                }

                // calculate the number of persons in each income levels
                if (theHouseholdIncome.at(i).at(j) == QString("Low")){
                    thePercentageLowIncomePerson += theHouseholdSize.at(i).at(j);
                }
                if (theHouseholdIncome.at(i).at(j) == QString("Moderate")){
                    thePercentageModerateIncomePerson += theHouseholdSize.at(i).at(j);
                }
                if (theHouseholdIncome.at(i).at(j) == QString("High")){
                    thePercentageHighIncomePerson += theHouseholdSize.at(i).at(j);
                }

                // calculate the number of persons in each immigration status categories
                if (theHouseholdImmigrantStatus.at(i).at(j) == QString("RecentImmigrant")){
                    thePercentageRecentImmigrantsPerson += theHouseholdSize.at(i).at(j);
                }
                if (theHouseholdImmigrantStatus.at(i).at(j) == QString("EstablishedImmigrant")){
                    thePercentageEstablishedImmigrantsPerson += theHouseholdSize.at(i).at(j);
                }
                if (theHouseholdImmigrantStatus.at(i).at(j) == QString("Resident")){
                    thePercentageResidentsPerson += theHouseholdSize.at(i).at(j);
                }

            }//j
        }//i

        thePercentageOfHouseholdWithChild /= theTotalNumberOfDwellings;
        thePercentageOfHouseholdWithSenior /= theTotalNumberOfDwellings;
        thePercentageLowIncomePerson /= theTotalNumberOfPeople;
        thePercentageModerateIncomePerson /= theTotalNumberOfPeople;
        thePercentageHighIncomePerson /= theTotalNumberOfPeople;
        thePercentageRecentImmigrantsPerson /= theTotalNumberOfPeople;
        thePercentageEstablishedImmigrantsPerson /= theTotalNumberOfPeople;
        thePercentageResidentsPerson /= theTotalNumberOfPeople;
    }


    //------------------------------------------------------
    // 2 - Get Response from Building Model
    //------------------------------------------------------



    if (timeOfEvent == 0.0){

        theBuildingModel = qobject_cast<RHazusBuildingModel*>(theBuildingModelResponse->getModel());

        // For all the buildings
        for (int i = 0; i < theNumberOfBuildings; i++){

            QVector<double> theSa0_3Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa0_3Intensity());
            double Sa0_3 = theSa0_3Vector.at(theSa03Counter);

            QVector<double> theSa1Vector = stringToDoubleVector(qobject_cast<RLocation *>(theLocation)->getSa1Intensity());
            double Sa1 = theSa1Vector.at(theSa1Counter);

            if(Sa0_3 >0 || Sa1>0){
                timeOfEvent = theCurrentTime->getCurrentValue();
            }

            // Give the building model the properties of this building
            theBuildingModel->setBuildingType(theBuildingType.at(i));
            theBuildingModel->setCodeDesignLevel(theCodeLevel.at(i));
            theBuildingModel->setOccupancyClass(theOccupancyClass.at(i));
            theBuildingModel->setSa1(Sa1);
            theBuildingModel->setSa2(Sa0_3);
            theBuildingModel->evaluateModel(RGradientType::NONE);

            // Get the damage responses
            theStructuralDamageState[i] = theBuildingModel->getStrDS();
            theAverageDamageState += theStructuralDamageState.at(i);

            // Calculate the Percentages of households and percentages of buildings in each Damage States (0 to 4)

            if (theStructuralDamageState.at(i) == 0){
                thePercentageDS0 += theNumberOfDwellingsInTheBuilding.at(i);
                theDS0 += 1;
            }

            if (theStructuralDamageState.at(i) == 1){
                thePercentageDS1 += theNumberOfDwellingsInTheBuilding.at(i);
                theDS1 += 1;
            }

            if (theStructuralDamageState.at(i) == 2){
                thePercentageDS2 += theNumberOfDwellingsInTheBuilding.at(i);
                theDS2 += 1;
            }

            if (theStructuralDamageState.at(i) == 3){
                thePercentageDS3 += theNumberOfDwellingsInTheBuilding.at(i);
                theDS3 += 1;
            }

            if (theStructuralDamageState.at(i) == 4){
                thePercentageDS4 += theNumberOfDwellingsInTheBuilding.at(i);
                theDS4 += 1;
            }


            //---------------------------------------------------------------------
            // calculate the Damage state probabilities for moderate, extensive and complete structural damage in the single-family and multifamily residential occupancy classes
            // %SFM, %SFE, %SFC, %MFM, %MFE, and %MFC
            //---------------------------------------------------------------------

            QString theOccupancyClassAux = theOccupancyClass.at(i);

//            qCritical()<<theOccupancyClassAux;

            //calculate %SFM and %MFM
            if (theStructuralDamageState.at(i) == 2){
                if (theOccupancyClassAux.contains("RES1")){
                    thePercentageSingleFamilyModerateDS += theNumberOfDwellingsInTheBuilding.at(i);
                }else if (theOccupancyClassAux.contains("RES3")){
                    thePercentageMultiFamilyModerateDS += theNumberOfDwellingsInTheBuilding.at(i);
                }
            }

            //calculate %SFE and %MFE
            if (theStructuralDamageState.at(i) == 3){
                if (theOccupancyClassAux.contains("RES1")){
                    thePercentageSingleFamilyExtensiveDS += theNumberOfDwellingsInTheBuilding.at(i);
                }else if (theOccupancyClassAux.contains("RES3")){
                    thePercentageMultiFamilyExtensiveDS += theNumberOfDwellingsInTheBuilding.at(i);
                }
            }

            //calculate %SFC and %MFC
            if (theStructuralDamageState.at(i) == 4){
                if (theOccupancyClassAux.contains("RES1")){
                    thePercentageSingleFamilyCompleteDS += theNumberOfDwellingsInTheBuilding.at(i);
                }else if (theOccupancyClassAux.contains("RES3")){
                    thePercentageMultiFamilyCompleteDS += theNumberOfDwellingsInTheBuilding.at(i);
                }
            }

            //calculate number of Single Family Units (#SFU) and Multi Family Unit (#MFU)

            if (theOccupancyClassAux.contains("RES1")){
                theNumberOfSingleFamilyDwellings += theNumberOfDwellingsInTheBuilding.at(i);
            }

            if(theOccupancyClassAux.contains("RES3")){
                theNumberOfMultiFamilyDwellings += theNumberOfDwellingsInTheBuilding.at(i);
            }
        }// i

//        qCritical() << "The Number of Single Family Units with Moderate Damage State: " << thePercentageSingleFamilyModerateDS;
//        qCritical() << "The Number of Multi Family Units with Moderate Damage State: " << thePercentageMultiFamilyModerateDS;
//        qCritical() << "The Number of Single Family Units with Extensive Damage State: " << thePercentageSingleFamilyExtensiveDS;
//        qCritical() << "The Number of Multi Family Units with Moderate Damage State: " << thePercentageMultiFamilyExtensiveDS;
//        qCritical() << "The Number of Single Family Units with Complete Damage State: " << thePercentageSingleFamilyCompleteDS;
//        qCritical() << "The Number of Multi Family Units with Complete Damage State: " << thePercentageMultiFamilyCompleteDS;
//        qCritical() << "The Number of Single-family Dwellings: " << theNumberOfSingleFamilyDwellings;

        if(theNumberOfSingleFamilyDwellings > 0 && thePercentageSingleFamilyModerateDS >0){
            thePercentageSingleFamilyModerateDS /= theNumberOfSingleFamilyDwellings;
        }
        else{
            thePercentageSingleFamilyModerateDS = 0.0;
        }

        if(theNumberOfSingleFamilyDwellings > 0 && thePercentageSingleFamilyExtensiveDS >0){
            thePercentageSingleFamilyExtensiveDS /= theNumberOfSingleFamilyDwellings;
        }
        else{
            thePercentageSingleFamilyExtensiveDS = 0.0;
        }

        if(theNumberOfSingleFamilyDwellings > 0 && thePercentageSingleFamilyCompleteDS >0){
            thePercentageSingleFamilyCompleteDS /= theNumberOfSingleFamilyDwellings;
        }
        else{
            thePercentageSingleFamilyCompleteDS = 0.0;
        }

        if(theNumberOfMultiFamilyDwellings > 0 && thePercentageMultiFamilyModerateDS >0){
            thePercentageMultiFamilyModerateDS /= theNumberOfMultiFamilyDwellings;
        }
        else{
            thePercentageMultiFamilyModerateDS = 0.0;
        }

        if(theNumberOfMultiFamilyDwellings > 0 && thePercentageMultiFamilyExtensiveDS >0){
            thePercentageMultiFamilyExtensiveDS /= theNumberOfMultiFamilyDwellings;
        }
        else{
            thePercentageMultiFamilyExtensiveDS = 0.0;
        }

        if(theNumberOfMultiFamilyDwellings > 0 && thePercentageMultiFamilyCompleteDS >0){
            thePercentageMultiFamilyCompleteDS /= theNumberOfMultiFamilyDwellings;
        }
        else{
            thePercentageMultiFamilyCompleteDS = 0.0;
        }

        theAverageDamageState /= theNumberOfBuildings;
        theDS0 /= theNumberOfBuildings;
        theDS1 /= theNumberOfBuildings;
        theDS2 /= theNumberOfBuildings;
        theDS3 /= theNumberOfBuildings;
        theDS4 /= theNumberOfBuildings;
        thePercentageDS0 /= theTotalNumberOfDwellings;
        thePercentageDS1 /= theTotalNumberOfDwellings;
        thePercentageDS2 /= theTotalNumberOfDwellings;
        thePercentageDS3 /= theTotalNumberOfDwellings;
        thePercentageDS4 /= theTotalNumberOfDwellings;


//        qCritical() <<  "thePercentageSingleFamilyModerateDS: " << thePercentageSingleFamilyModerateDS;
//        qCritical() <<  "thePercentageSingleFamilyExtensiveDS: " << thePercentageSingleFamilyExtensiveDS;
//        qCritical() <<  "thePercentageSingleFamilyCompleteDS: " << thePercentageSingleFamilyCompleteDS;
//        qCritical() <<  "thePercentageMultiFamilyModerateDS: " << thePercentageMultiFamilyModerateDS;
//        qCritical() <<  "thePercentageMultiFamilyExtensiveDS: " << thePercentageMultiFamilyExtensiveDS;
//        qCritical() <<  "thePercentageMultiFamilyCompleteDS: " << thePercentageMultiFamilyCompleteDS;


    }//timeOfEvent == 0

    //------------------------------------------------------------
    // 3 - Create a Hazus-based displacement estimator
    //------------------------------------------------------------

    //---------------------------------------------------
    // Get HI, HE, HO, and HA (Vectors)
    //---------------------------------------------------

    // 3.1. get HI: percentage of households belonging to five different income categories:
    // Percentage of households whose income is under $10,000 (HI1)
    // Percentage of households whose income is $10,001 to $15,000 (HI2)
    // Percentage of households whose income is $15,001 to $25,000 (HI3)
    // Percentage of households whose income is $25,001 to $35,000 (HI4)
    // Percentage of households whose income is over $35,000 (HI5)
    // The above numbers does NOT apply to San Francisco
    // Assume 1/5 of the low-income households belong to HI1 group, 1/5 belongs to HI2, and the remaining 3/5 belong to HI3
    // Assume all moderate-income households belong to HI4 group
    // Assume all high-income households belong to HI5 group

    thePercentageOfHI1 = 0.2 * theHouseholdPortfolio->getPercentageLowIncome();
    thePercentageOfHI2 = 0.2 * theHouseholdPortfolio->getPercentageLowIncome();
    thePercentageOfHI3 = 0.6 * theHouseholdPortfolio->getPercentageLowIncome();
    thePercentageOfHI4 = theHouseholdPortfolio->getPercentageModerateIncome();
    thePercentageOfHI5 = theHouseholdPortfolio->getPercentageHighIncome();


    // 3.2. get percentage of households belonging to five different ethnicity groups
    // Percentage of white households (HE1)
    // Percentage of black households (HE2)
    // Percentage of Hispanic households (HE3)
    // Percentage of Native American households (HE4)
    // Percentage of Asian households (HE5)
    // Did not seem to have information in the householdportfolio
    // Assume HE1 = 0.4, HE2 = 0.2, HE3 = 0.2, HE4 = 0.0, HE5 = 0.2


    // 3.3. get percentage of households of two different ownership status: owner (HO1)  / renter (HO2)
    // Can get this information directly from theHouseholdPortfolio

    thePercentageOfHO1  = theHouseholdPortfolio->getPercentageOfRentedBuildings();
    thePercentageOfHO2  = 1.0 - thePercentageOfHO1;

    // 3.4. get percentage of population of three different age categories
    //Percentage of population under 16 years old (HA1)
    //Percentage of population between 16 and 65 years old (HA2)
    //Percentage of population over 65 years old (HA3)
    //Can get this directly from theHouseholdPortfolio class

    theHA1          = theHouseholdPortfolio->getPercentageOfChildren();
    theHA3          = theHouseholdPortfolio->getPercentageOfSeniors();
    theHA2          = 1.0- theHA1 - theHA3;

    // 3.5. Assemble the HI, HE, HO, HA vectors

    theHIVector << thePercentageOfHI1 << thePercentageOfHI2 << thePercentageOfHI3 << thePercentageOfHI4 << thePercentageOfHI5;
    theHEVector << 0.4 << 0.2 << 0.2 << 0.0 << 0.2;
    theHOVector << thePercentageOfHO1 << thePercentageOfHO2;
    theHAVector << theHA1 << theHA2 << theHA3;

//    qCritical() << "theHIVector:" << theHIVector;
//    qCritical() << "theHEVector:" << theHEVector;
//    qCritical() << "theHOVector:" << theHOVector;
//    qCritical() << "theHAVector:" << theHAVector;

    //---------------------------------------------------------------------
    //3.6. calculate the number of displaced households using hazus methods
    //calculate the number of people requiring shorterm housing #STP
    //---------------------------------------------------------------------

    // 3.6.1. Define IM EM OM AM vector (modification factors for income, ethnicity, ownership status, and age)
    theIncomeModificationFactors << 0.64 << 0.42 << 0.29 << 0.22 << 0.13;
    theEthnicityModificationFactors << 0.24 << 0.48 << 0.47 << 0.26 << 0.26;
    theOwnershipModificationFactors << 0.40 << 0.40;
    theAgeModificationFactors << 0.40 << 0.40 << 0.40;

    // 3.6.2. <WSFM, WSFE, WSFC> and <WMFM, WMFEE, WMFC>
    theWeightFactorsOfSingleFamily << 0.0 << 0.0 << 1.0;
    theWeightFactorsOfMultiFamily << 0.0 << 0.9 << 1.0;

    // 3.6.3. calculate %SF and %MF
    thePercentageOfDisplacedSingleFamilyHouseholds = theWeightFactorsOfSingleFamily.at(0) * thePercentageSingleFamilyModerateDS + theWeightFactorsOfSingleFamily.at(1) * thePercentageSingleFamilyExtensiveDS + theWeightFactorsOfSingleFamily.at(2) * thePercentageSingleFamilyCompleteDS;
    thePercentageOfDisplacedMultiFamilyHouseholds = theWeightFactorsOfMultiFamily.at(0) * thePercentageMultiFamilyModerateDS + theWeightFactorsOfMultiFamily.at(1) * thePercentageMultiFamilyExtensiveDS + theWeightFactorsOfMultiFamily.at(2) * thePercentageMultiFamilyCompleteDS;

    // 3.6.4. calculate #DH
    theNumberOfDisplacedHouseholds = (theNumberOfSingleFamilyDwellings * thePercentageOfDisplacedSingleFamilyHouseholds + theNumberOfMultiFamilyDwellings * thePercentageOfDisplacedMultiFamilyHouseholds)*(static_cast<double>(theTotalNumberOfDwellings)/static_cast<double>((theNumberOfSingleFamilyDwellings + theNumberOfMultiFamilyDwellings)));

//    qCritical() << "The number of displaced household: " << theNumberOfDisplacedHouseholds;


    // 3.6.5. calculate the number of people requiring shorterm housing #STP

    double theAlphaModificationFactor;
    // This methodology estimates the number of people seeking public shelter
    theNumberOfPeopleRequiringShortermHousing = 0.0;

    for (int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            for(int k = 0; k < 2; k++){
                for(int l = 0; l < 3; l++){

                    //Calculate the alpha modification factor
                    theAlphaModificationFactor = 0.73 * theIncomeModificationFactors.at(i) + 0.27 * theEthnicityModificationFactors.at(j) + 0.0 * theOwnershipModificationFactors.at(k) + 0.0 * theAgeModificationFactors.at(l);
                    //We do not necessarily need theAlphaModificationFactor to be stored
                    theNumberOfPeopleRequiringShortermHousing += theAlphaModificationFactor * (theNumberOfDisplacedHouseholds * static_cast<double>(theTotalNumberOfPeople) / static_cast<double>(theTotalNumberOfDwellings)) * theHIVector.at(i) * theHEVector.at(j) * theHOVector.at(k) * theHAVector.at(l);
                }
            }
        }
    }

    //Assume all displaced people are displaced for 30 days
    double theAverageHouseholdSize = 1.0*HouseholdSizeDistribution.at(0) + 2.0*HouseholdSizeDistribution.at(1) + 3.0*HouseholdSizeDistribution.at(2) + 4.0*HouseholdSizeDistribution.at(3) + 5.0*HouseholdSizeDistribution.at(4);
    theNumberOfPeopleTimesDaysDisplaced = theNumberOfDisplacedHouseholds * theAverageHouseholdSize * 30;

    qCritical() << "Now evaluating" <<this->objectName() << ": has" << theNumberOfBuildings << "Buildings and" << theTotalNumberOfDwellings << "Dwellings and" << theTotalNumberOfPeople << "Persons";
    qCritical() << "Average Damage State, PercentageDS0, PercentageDS1, PercentageDS2, PercentageDS3, PercentageDS4: " << theAverageDamageState << thePercentageDS0 << thePercentageDS1 << thePercentageDS2 << thePercentageDS3 << thePercentageDS4 <<" DS0, DS1, DS2, DS3, DS4: "<< theDS0 << theDS1 << theDS2 << theDS3 << theDS4 << " The Number of People Times Days Displaced: " << theNumberOfPeopleTimesDaysDisplaced<< " The Number of People Requiring Short-term Housing: " << static_cast<int>(theNumberOfPeopleRequiringShortermHousing) << " The Number of Displaced Households: "<<theNumberOfDisplacedHouseholds;
    qCritical() << "the percentages of low, moderate, high income household: " << thePercentageLowIncome << thePercentageModerateIncome << thePercentageHighIncome << " The percentages of recent immigrants, established immigrants, residents household: " << thePercentageRecentImmigrants << thePercentageEstablishedImmigrants << thePercentageResidents << " The Percentages low, moderate, high income persons: "<< thePercentageLowIncomePerson << thePercentageModerateIncomePerson << thePercentageHighIncomePerson << " The Percentages recent immigrants, established immigrants, residents (persons): "<< thePercentageRecentImmigrantsPerson << thePercentageEstablishedImmigrantsPerson << thePercentageResidentsPerson << " The number of single-family dwellings, multi-family dwellings, single-family buildngs, multi-family buildings: " << theNumberOfSingleFamilyDwellings << theNumberOfMultiFamilyDwellings << theNumberOfSingleFamilyBuildings << theNumberOfMultiFamilyBuildings << " The Age Percentage Vector: theHAVector " << theHAVector << " The Percentage of households with child, with senior: " << thePercentageOfHouseholdWithChild << thePercentageOfHouseholdWithSenior;
    return 1;
}
