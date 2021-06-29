#include "RWaterPipelineModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>
#include "RNormalDistribution.h"
#include "RLocation.h"
#include "RNetworkSamplingModel.h"

RWaterPipelineModel::RWaterPipelineModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{ 
    theFunctionality = 1.0;
    numberOfLeaks = 0;
    numberOfBreaks = 0;
    theDuctilePipelineLength = 0.0;
    theBritlePipelineLength = 0.0;
    theSoilSusceptibilityLevel = QString("Low");

    thePipelineType = RWaterPipelineModel::NoDamage;

    // Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);

}

RWaterPipelineModel::~RWaterPipelineModel()
{
}

int RWaterPipelineModel::resetTime()
{
    handlePacketsThatReachedFinalDestination();

    theAvailabilityOfWater = false;

    timeOfEvent = 0.0;
    theTimeUnderRepairs = 0.0;
    numberOfLeaks = 0;
    numberOfBreaks = 0;
    directLoss = 0.0;
    theLateralPGD = 0.0;
    theProbabilityOfLiquefaction = 0.0;

    theResponse->setCurrentValue(0.0);

    // Not the most ellegant solution
    // PGA [0 , 1 , 2 ..., n]
    QVector<double> thePGAs = stringToDoubleVector(qobject_cast<RLocation*>(theLocation)->getPGAIntensity());

    if (thePGACounter > (thePGAs.size() - 1)){
        qCritical() << this->objectName() << "Hey, I do not have enough PGA values!";
        thePGACounter = -1;
    }

    thePGAIntensity = thePGAs.at(thePGACounter);
    thePGACounter++;

    // Clear the fragility curve variables
    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    return 0;
}


QObject * RWaterPipelineModel::getMagnitude()
{
    return theMagnitude;
}

void RWaterPipelineModel::setMagnitude(QObject *value)
{
    theMagnitude = qobject_cast<RParameter*>(value);
}


QObject * RWaterPipelineModel::getFromLocation() const
{
    return theFromLocation;
}

void RWaterPipelineModel::setFromLocation(QObject *value)
{
    theFromLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theFromLocation);
}



QObject * RWaterPipelineModel::getToLocation() const
{
    return theToLocation;
}

void RWaterPipelineModel::setToLocation(QObject *value)
{
    theToLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theToLocation);
}


double RWaterPipelineModel::getDuctilePipelineLength() const
{
    return theDuctilePipelineLength;
}

void RWaterPipelineModel::setDuctilePipelineLength(double value)
{
    theDuctilePipelineLength = value;
}

double RWaterPipelineModel::getBritlePipelineLength() const
{
    return theBritlePipelineLength;
}

void RWaterPipelineModel::setBritlePipelineLength(double value)
{
    theBritlePipelineLength = value;
}


double RWaterPipelineModel::getDiameter() const
{
    return theDiameter;
}

void RWaterPipelineModel::setDiameter(double value)
{
    theDiameter = value;
}


double RWaterPipelineModel::getWorkforce() const
{
    return theWorkforce;
}

void RWaterPipelineModel::setWorkforce(double value)
{
    theWorkforce = value;
}


double RWaterPipelineModel::getPercentageOfLiquefiableSoil() const
{
    return theFractionOnLiquefiableSoil;
}

void RWaterPipelineModel::setPercentageOfLiquefiableSoil(double value)
{
    theFractionOnLiquefiableSoil = value;
}

QString RWaterPipelineModel::getWaterSupplierList()
{
    return parameterListToString(theWaterSupplierList);
}

void RWaterPipelineModel::setWaterSupplierList(QString value)
{
    theWaterSupplierList = stringToParameterList(value);
}


double RWaterPipelineModel::calculatePGDIntensity(double thePGA)
{
    // THIS MODEL CALCULATES PGD CAUSED BY LIQUEFACTION
    // This model was double checked against the algorithm presented in the latest Hazus manual.
    // It suffers from the problem of not having a map of liquefaction susceptibility built-in R3.
    // It is assumed that ports are at high susceptibility of liquefaction due to being close to the water.
    // All other models are considered at LOW susceptibility of liquefaction.
    // This will need to be improved eventually.

    double theKm = 0.0;
    double theKw = 0.0;
    double theGroundwaterDepth = 5.0; //in feet (the factors were obtained for GWD of 5 feet - page 140 Hazus 2.1)
    double theDisplacementCorrectionFactor = 0.0;
    double theThresholdPGA = 0.0;
    double theProportionOfTheMap = 0.0;
    double theLiquefactionSusceptibility;
    double theVerticalPGD;
    double theExpectedPGD;

    // 1 - Define if the fixed structure is or is not on a zone susceptible to liquefaction
    // This is not being considered

    // 2 - Conditional probability of liquefaction for a susceptibility category (this is the Prob. of Liquefaction for a given set of conditions)
    // The vertical PGD is a simple function of the susceptibility level, therefore it is calculated here too
    // See Hazus 2.1 item 4.2.2.1.3 on page 138 of the pdf file
    // See Table 4.13 Hazus report - a large number here denotes a soil with no susceptibility to liquefaction.
    if (theSoilSusceptibilityLevel == QString("None")){
        theThresholdPGA = 1000.0;
        theVerticalPGD = 0.0;
        theLiquefactionSusceptibility = 0.0;
        theProportionOfTheMap = 0.0;

    } else if (theSoilSusceptibilityLevel == QString("VeryLow")){
        theThresholdPGA = 0.26;
        theVerticalPGD = 0.0;
        theLiquefactionSusceptibility = 4.16 * thePGA - 1.08;
        theProportionOfTheMap = 0.02;

    } else if (theSoilSusceptibilityLevel == QString("Low")){
        theThresholdPGA = 0.21;
        theVerticalPGD = 1.0;
        theLiquefactionSusceptibility = 5.57 * thePGA - 1.18;
        theProportionOfTheMap = 0.05;

    } else if (theSoilSusceptibilityLevel == QString("Moderate")){
        theThresholdPGA = 0.15;
        theVerticalPGD = 2.0;
        theLiquefactionSusceptibility = 6.67 * thePGA - 1.0;
        theProportionOfTheMap = 0.10;

    } else if (theSoilSusceptibilityLevel == QString("High")){
        theThresholdPGA = 0.12;
        theVerticalPGD = 6.0;
        theLiquefactionSusceptibility = 7.67 * thePGA - 0.92;
        theProportionOfTheMap = 0.20;

    } else if (theSoilSusceptibilityLevel == QString("VeryHigh")){
        theThresholdPGA = 0.09;
        theVerticalPGD = 12.0;
        theLiquefactionSusceptibility = 9.09 * thePGA - 0.82;
        theProportionOfTheMap = 0.25;

    } else {
        qCritical() << this->objectName() << "could not identify the Soil Susceptibility Level!";
        theThresholdPGA = 0.0;
        theVerticalPGD = 0.0;
        theLiquefactionSusceptibility = 0.0;
        theProportionOfTheMap = 0.0;
    }

    // 2.5 - Uncertainty in the ground displacement estimates
    // The uncertainty associated with vertical settlement values is assumed to have a uniform probability distribution within bounds of one-half to two times the respective value.
    // The following expression guarantees a number between 0.5 and 2.0 the PGDs
    theVerticalPGD = theVerticalPGD - 0.5 * theVerticalPGD + 1.5 * theVerticalPGD * rand() / static_cast<double>(RAND_MAX);


    // Bounds for the conditional probability of liquefaction
    if (theLiquefactionSusceptibility < 0.0){
        theLiquefactionSusceptibility = 0.0;

    } else if (theLiquefactionSusceptibility > 1.0) {
        theLiquefactionSusceptibility = 1.0;
    }


    // 3 - Expected PGD
    // get the earthquake magnitude
    double M = theMagnitude->getCurrentValue();

    // Calculating the probability of soil liquefaction
    theKm = 0.0027 * M*M*M - 0.0267 * M*M - 0.2055 * M + 2.9188;
    theKw = 0.022 * theGroundwaterDepth + 0.93;
    theProbabilityOfLiquefaction = theLiquefactionSusceptibility*(theProportionOfTheMap/(theKm*theKw));

    // For lateral spreading Eq 4.24 Hazus 2.1
    theDisplacementCorrectionFactor = 0.0086 * M*M*M - 0.0914 * M*M + 0.4698 * M - 0.9835;

    // Depending on the ratio between PGA and threshold PGA, calculate the Lateral Spreading in inches
    if (thePGA/theThresholdPGA < 1.0){
        theLateralPGD = 0.0;

        // See Figure 4.9 Hazus 2.1 page 143 of the pdf file
    } else if ( (thePGA/theThresholdPGA) > 1.0 && (thePGA/theThresholdPGA) < 2.0 ) {
        theLateralPGD = (12.0*(thePGA / theThresholdPGA) - 12.0) * theDisplacementCorrectionFactor;

    } else if ( (thePGA/theThresholdPGA) > 2.0 && (thePGA/theThresholdPGA) < 3.0) {
        theLateralPGD = (18.0*(thePGA / theThresholdPGA) - 24.0) * theDisplacementCorrectionFactor;

    } else if ( (thePGA/theThresholdPGA) > 3.0 && (thePGA/theThresholdPGA) < 4.0){
        theLateralPGD = (70.0*(thePGA / theThresholdPGA) - 180.0) * theDisplacementCorrectionFactor;

        // Defining an forced upper bound for lateral spreading
    } else if (thePGA / theThresholdPGA > 4.0){
        theLateralPGD = (70.0*4.0 - 180.0) * theDisplacementCorrectionFactor;

    } else {
        qCritical() << this->objectName() << "The ratio of PGA to PGA threshold is out of bounds!";
        theLateralPGD = 0.0;
    }


    // Ground Settlement
    // 3.5 - Define the dominant PGD
    if(theLateralPGD != 0.0 || theVerticalPGD != 0.0){

        theExpectedPGD = qMax(theLateralPGD,theVerticalPGD) * theProbabilityOfLiquefaction;
        //qCritical() << theDisplacementCorrectionFactor << thePGA / theThresholdPGA << theProbabilityOfLiquefaction;
    }
    else {

        theExpectedPGD = 0.0;
    }

    return theExpectedPGD;
}


int RWaterPipelineModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    // Reseting the responses
    theFunctionality = 0.0;
    double daysToFinishAllRepairs = 0.0;
    theAvailabilityOfWater = false;


    // -------------------------------------------------------------------------------------------
    // 2 - HANDLING RESPONSES COMING TO THIS MODEL
    // -------------------------------------------------------------------------------------------
    for (int i = 0; i < theWaterSupplierList.size(); i++){

        // Get hold of the input response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(theWaterSupplierList.at(i));

        if (theResponseComingIntoThisModel == nullptr){
            qCritical() << "There is a problem with water supplier list for" << this->objectName();
        }

        // If this is a continuous response - this is what the model needs to function
        if (theResponseComingIntoThisModel->getCurrentValue() <= 0.0){
            theAvailabilityOfWater = true;
            theResponse->setCurrentValue(0.0);

        } else {
            // The response of this model is how long it will take for it to be operational again
            // Hence why I am taking the max between the time for water and power to be restored
            theResponse->setCurrentValue(qMin(theResponse->getCurrentValue(), theResponseComingIntoThisModel->getCurrentValue()));
        }
    }

    // ------------------------------
    // Make sure this is evaluated only once
    if (timeOfEvent == 0.0){

        // Calculate the number of breaks and leaks for the ductile section then for the brittle section
        QVector<QString> theDuctilityVector;
        theDuctilityVector << QString("ductile") << QString("brittle");
        for (int i = 0; i < theDuctilityVector.size(); i++){
            double thePipelineLength = 0.0;

            // This is redundant but just making the code extra clear
            if (i == 0){
                thePipelineType = RWaterPipelineModel::Ductile;
                thePipelineLength = theDuctilePipelineLength;
            } else {
                thePipelineType = RWaterPipelineModel::Brittle;
                thePipelineLength = theBritlePipelineLength;
            }

            // Ductile pipeline
            if (thePipelineType == RWaterPipelineModel::Ductile){
                theVulnerabilityFactor = 0.3;
                replacementCost = 1000.0;
            }

            // Brittle pipeline
            else if (thePipelineType == RWaterPipelineModel::Brittle){
                theVulnerabilityFactor = 1.0;
                replacementCost = 1000.0;

            } else {
                theVulnerabilityFactor = 0.0;
            }

            // Calculate the pipeline length, if that was not provided by the user
            if (thePipelineLength < 0.0){
                RLocation *myFromLocation = qobject_cast<RLocation *>(theFromLocation);
                RLocation *myToLocation = qobject_cast<RLocation *>(theToLocation);

                // Get the distance between the start and end point of this road
                double lat1 = myFromLocation->getLatitude() / 180.0 * 3.14159265359;
                double lng1 = myFromLocation->getLongitude() / 180.0 * 3.14159265359;

                double lat2 = myToLocation->getLatitude() / 180.0 * 3.14159265359;
                double lng2 = myToLocation->getLongitude() / 180.0 * 3.14159265359;

                // Calculate the tentative distance for the end node of this road element - road length + how far you are from the departing node
                thePipelineLength = acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lng2 - lng1)) * EarthRadius;
            }


            // -------------------------------------------------------------------------------------------
            // 2 - DAMAGE AND FUNCTIONALITY
            // -------------------------------------------------------------------------------------------

            // Calculate the number of breaks and leaks for the section of pipelines on hard soils then on soft soils
            QVector<QString> theLiquefactionSusceptibilityVector;
            theLiquefactionSusceptibilityVector << QString("None") << QString("High");
            double theFractionOnThisSoilType = 0.0;

            for (int j = 0; j < theLiquefactionSusceptibilityVector.size(); j++){

                if (j == 0){
                    theSoilSusceptibilityLevel = QString("None");
                    theFractionOnThisSoilType = 1.0 - theFractionOnLiquefiableSoil;
                } else {
                    theSoilSusceptibilityLevel = QString("High");
                    theFractionOnThisSoilType = theFractionOnLiquefiableSoil;
                }

                // Earthquake
                //this->calculateIntensities();

                // Get hold of the PGA intensities at the start, mid, and end points of the pipeline
                double theSa1From = (stringToDoubleVector(qobject_cast<RLocation*>(theFromLocation)->getSa1Intensity())).at(thePGACounter - 1); // -1 because the counter is one time-step ahead when it gets here
                double theSa1Mid = (stringToDoubleVector(qobject_cast<RLocation*>(theLocation)->getSa1Intensity())).at(thePGACounter - 1);
                double theSa1To = (stringToDoubleVector(qobject_cast<RLocation*>(theToLocation)->getSa1Intensity())).at(thePGACounter - 1);

                QVector<double> theSaVector;
                theSaVector << theSa1From << theSa1Mid << theSa1To; // start, mid, and end points



                // *********
                //RNetworkSamplingModel * theSamplingModel = theDomain->findChild<RNetworkSamplingModel*>();
                //double theMultiplier = theSamplingModel->getSamplingMultiplier();
                //theSaVector << theMultiplier * (stringToDoubleVector(qobject_cast<RLocation*>(theFromLocation)->getSa1Intensity())).at(0) << theMultiplier * (stringToDoubleVector(qobject_cast<RLocation*>(theLocation)->getSa1Intensity())).at(0) << theMultiplier * (stringToDoubleVector(qobject_cast<RLocation*>(theToLocation)->getSa1Intensity())).at(0);
                // *********




                // Loop over the PGAs at the start, mid, and end point
                for (int k = 0; k < theSaVector.size();k++){

                    // Separate the pipeline into three sections with 25%, 50%, and 25% of the total length
                    // Each section will be exposed to one level of Sa1
                    double theFractionOfThePipeline = 0.25;
                    if (k == 1){ // this means this is the mid section
                        theFractionOfThePipeline = 0.5;
                    }

                    double theSa1 = theSaVector.at(k);

                    // Calculate PGV as a function of the Sa1 - HAZUS approach
                    double thePGVIntensity = (386.4 / 3.14159265359 / 2) * theSa1 / 1.65;

                    // Calculate the PGD as a function of the PGA
                    thePGDIntensity = calculatePGDIntensity(thePGAIntensity);

                    if (thePGAIntensity > 0.0 || thePGDIntensity > 0.0 || thePGVIntensity > 0.0){
                        timeOfEvent = theCurrentTime->getCurrentValue();

                        // Damage algorithm for buried pipes - Hazus 2.1 page 401
                        // For PGV
                        double thePGVRepairRate = theVulnerabilityFactor * 0.0001 * qPow(thePGVIntensity, 2.25);
                        int numberOfRepairsDueToPGV = qRound(theFractionOfThePipeline * thePipelineLength * thePGVRepairRate);

                        // For PGD
                        double thePGDRepairRate = theVulnerabilityFactor * theProbabilityOfLiquefaction * qPow(theLateralPGD, 0.56);
                        int numberOfRepairsDueToPGD = qRound(theFractionOfThePipeline * thePipelineLength * thePGDRepairRate);

                        numberOfLeaks  += qRound(theFractionOnThisSoilType * (0.8 * numberOfRepairsDueToPGV + 0.2 * numberOfRepairsDueToPGD));
                        numberOfBreaks += qRound(theFractionOnThisSoilType * (0.2 * numberOfRepairsDueToPGV + 0.8 * numberOfRepairsDueToPGD));

                        // see Hazus 2.1 page 638: damage ratio for a break is 0.7 and for a leak is 0.1 of the replacement cost of a 20ft pipeline
                        directLoss += theFractionOnThisSoilType * (0.7 * numberOfBreaks + 0.1 * numberOfLeaks) * replacementCost;

                    }
                } // end if time
            } // end if j < 2
        } // end if i < 2
    } // if time of event = 0

    if (numberOfLeaks > 0 || numberOfBreaks > 0){

        // Assuming breaks take twice as long to fix then breaks
        int numberOfRepairs = numberOfBreaks + numberOfLeaks * 0.5;
        int numberOfRemainingRepairs;
        double theDiameterInInches = theDiameter / 25.4;
        double averageRepairRate;

        if (theDiameterInInches < 20){
            // See table 8.1 Hazus 2.1 page 395 for the worker / repair ratios
            // It takes a 32 member crew to fix a break in 1h.
            // One crew can fix one break in 8 hrs
            // Daily shifts are 16 hrs
            numberOfRemainingRepairs = qFloor(numberOfRepairs - (8.0/16.0) * theWorkforce * (theCurrentTime->getCurrentValue() - timeOfEvent));
            daysToFinishAllRepairs = numberOfRemainingRepairs / ((8.0/16.0) * theWorkforce); // in days
        }

        else {
            // See table 8.1 Hazus 2.1 page 395 for the worker / repair ratios
            // It takes a 48 member crew to fix a break in 1h.
            // One crew can fix one break in 12 hrs
            // Daily shifts are 16 hrs
            numberOfRemainingRepairs = qFloor(numberOfRepairs - (12.0/16.0) * theWorkforce *  (theCurrentTime->getCurrentValue() - timeOfEvent));
            daysToFinishAllRepairs = numberOfRemainingRepairs / ((12.0/16.0) * theWorkforce); // in days
        }

        if (numberOfRemainingRepairs <= 0){
            numberOfRemainingRepairs = 0;
            daysToFinishAllRepairs = 0.0;
        }

        averageRepairRate = numberOfRemainingRepairs / (theDuctilePipelineLength + theBritlePipelineLength + qPow(10,-8)); // qPow(10,-8) is to be extra careful with a possible zero length pipeline

        // Create a normal distribution tool
        RNormalDistribution theNormalDistribution(theDomain);
        theFunctionality = 1 - theNormalDistribution.getNormalCDF((1.0 / 0.85) * log(averageRepairRate / 0.1),1);

    } else if (theAvailabilityOfWater == false) {
        theFunctionality = 0.0;

    } else {
        theFunctionality = 1.0;
    }

    // Checking the availability of the suppliers
    // if (theFunctionality < theThresholdfunctionality || theAvailabilityOfWater == false){
    // theResponse->setCurrentValue(qMax(theResponse->getCurrentValue(), 0.5 * daysToFinishAllRepairs));
    // }

    theResponse->setCurrentValue(theFunctionality);

                                 //    if (this->objectName().contains("Pipelines")){
                                 //        qCritical() << this->objectName() <<  0.8 * daysToFinishAllRepairs <<  theResponse->getCurrentValue() << "";
                                 //    }



                                 return 1;
}
