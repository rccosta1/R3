#include "RFuelPipelineModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>
#include "RNormalDistribution.h"

RFuelPipelineModel::RFuelPipelineModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theFunctionality = 1.0;
    numberOfLeaks = 0;
    numberOfBreaks = 0;

    thePipelineType = RFuelPipelineModel::NoDamage;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RFuelPipelineModel::~RFuelPipelineModel()
{
}

int RFuelPipelineModel::resetTime()
{
    timeOfEvent = 0.0;
    handlePacketsThatReachedFinalDestination();
    numberOfLeaks = 0;
    numberOfBreaks = 0;

    return 0;
}


RFuelPipelineModel::RPipelineType RFuelPipelineModel::getPipelineType() const
{
    return thePipelineType;
}

void RFuelPipelineModel::setPipelineType(RFuelPipelineModel::RPipelineType value)
{

    thePipelineType = value;

    /*
    The most significant earthquake-triggered geohazard the pipeline(s)
    is exposed to is horizontal ground displacement from landslides and liquefaction
    induced lateral ground displacement (known as lateral spreading).
    Understanding potential location, estimated frequency and magnitude of the potential
    lateral spreading is sought by designers and operators of any infrastructure in seismically active regions.

    https://www.transmountain.com/seismic-safety-measures
    */

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 452
    theRepairTimeMedians << 0.5 << 3.6  << 22.0  << 65.0 ;
    theRepairTimeStandardDeviations << 0.1 << 3.6 << 21.0 << 30.0;

    // Ductile pipeline
    if (thePipelineType == RFuelPipelineModel::Ductile){
        theVulnerabilityFactor = 0.3;
    }

    // Brittle pipeline
    else if (thePipelineType == RFuelPipelineModel::Brittle){
        theVulnerabilityFactor = 1.0;
    }

    else {

        qCritical() << " Invalid option for " << this->objectName();
    }

}


double RFuelPipelineModel::getDiameter() const
{
    return theDiameter;
}

void RFuelPipelineModel::setDiameter(double value)
{
    theDiameter = value;
}


double RFuelPipelineModel::getPipelineLength() const
{
    return thePipelineLength;
}

void RFuelPipelineModel::setPipelineLength(double value)
{
    thePipelineLength = value;
}



int RFuelPipelineModel::getWorkforce() const
{
    return theWorkforce;
}

void RFuelPipelineModel::setWorkforce(int value)
{
    theWorkforce = value;
}


QString RFuelPipelineModel::getFuelSupplierList()
{
    return parameterListToString(theFuelSupplierList);
}

void RFuelPipelineModel::setFuelSupplierList(QString value)
{
    theFuelSupplierList = stringToParameterList(value);
}


int RFuelPipelineModel::evaluateModel()
{
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    // Reseting the response
    theResponse->setCurrentValue(1.0);

    // ------------------------------
    // Ductile pipeline
    if (thePipelineType == 1){
        theVulnerabilityFactor = 0.3;
    }

    // Brittle pipeline
    else if (thePipelineType == 2){
        theVulnerabilityFactor = 1.0;
    }

    else {
        theVulnerabilityFactor = 0.0;
    }
    // ------------------------------


    // Earthquake
    QVector<double> theIntensities = calculateIntensities();
    double thePGAIntensity = theIntensities.at(0);
    double thePGDIntensity = theIntensities.at(1);
    double thePGVIntensity = theIntensities.at(2);

    if (thePGAIntensity > 0.0 || thePGDIntensity > 0.0 || thePGVIntensity > 0.0){
        // Damage algorithm for buried pipes - Hazus 2.1 page 401
        // For PGV
        double thePGVRepairRate = theVulnerabilityFactor * 0.0001 * qPow(thePGVIntensity, 2.25);
        int numberOfRepairsDueToPGV = qCeil(thePipelineLength * thePGVRepairRate);

        // For PGD
        double thePGDRepairRate = theVulnerabilityFactor * myProbabilityOfLiquefaction * qPow(lateralPGDIntensity, 0.56);
        int numberOfRepairsDueToPGD = qCeil(thePipelineLength * thePGDRepairRate);

        numberOfLeaks  = 0.8 * numberOfRepairsDueToPGV + 0.2 * numberOfRepairsDueToPGD;
        numberOfBreaks = 0.2 * numberOfRepairsDueToPGV + 0.8 * numberOfRepairsDueToPGD;
    }


    if (numberOfLeaks > 0 || numberOfBreaks > 0){

        // Assuming breaks take twice as long to fix then breaks
        int numberOfRepairs = numberOfBreaks + numberOfLeaks * 0.5;
        int numberOfRemainingRepairs;
        double theDiameterInInches = theDiameter / 25.4;
        double averageRepairRate;


        if (theDiameterInInches < 20){
            // See table 8.1 Hazus 2.1 page 395 for the worker / repair ratios
            numberOfRemainingRepairs = qCeil(numberOfRepairs - 0.5 * theWorkforce *(theCurrentTime->getCurrentValue() - timeOfEvent));
        }

        else {
            // See table 8.1 Hazus 2.1 page 395 for the worker / repair ratios
            numberOfRemainingRepairs = qCeil(numberOfRepairs - 0.33 * theWorkforce *(theCurrentTime->getCurrentValue() - timeOfEvent));
        }

        if (numberOfRemainingRepairs < 0){
            numberOfRemainingRepairs = 0;
        }

        averageRepairRate = numberOfRemainingRepairs / thePipelineLength + qPow(10,-8);

        // Create a normal distribution tool
        RNormalDistribution theNormalDistribution(theDomain);
        theFunctionality = 1 - theNormalDistribution.getNormalCDF((1.0 / 0.85) * log(averageRepairRate / 0.1),1);

    }
    else {
        theFunctionality = 1.0;
    }


    // Check the availability of this model based on its current damage state
    

    if (theFunctionality >= theThresholdfunctionality) {
        theResponse->setCurrentValue(1.0);
        
        unloadPackets(qobject_cast<RPacketResponse *>(theResponse), packetsCurrentlyOnboard);
    }



    // Handle the responses coming to this model
    int numPackets = 0;


    // Checking the availability of the suppliers
    if (theFunctionality >= theThresholdfunctionality){


        // Define a list of packets to be loaded
        QList<QPointer <RParameter>> handlePacketsThatReachedFinalDestination;

        for (int i = 0; i < theFuelSupplierList.size(); i++){

            // Get hold of the input response
            RPacketResponse *thePacketResponseComingInHere = qobject_cast<RPacketResponse *>(theFuelSupplierList.at(i));
            RGenericResponse *theResponseComingInHere = qobject_cast<RGenericResponse *>(theFuelSupplierList.at(i));


            if (!thePacketResponseComingInHere && !theResponseComingInHere) {
                qCritical() << this->objectName() << "did not find a proper input";
            }


            // If this is a packet response
            else if (thePacketResponseComingInHere){

                // Add this response to the packet list
                handlePacketsThatReachedFinalDestination << theFuelSupplierList.at(i);
            }

            // If this is a continuous response
            else if (theResponseComingInHere){

            }

        }


        // Loading and handling packets coming to this model
        loadPackets(handlePacketsThatReachedFinalDestination);
        numPackets = packetsCurrentlyOnboard.size();;
    }


    else {
        // Delete packets that got here but could not be loaded
        handlePacketsThatReachedFinalDestination();
    }


    //Fix to the fuel "reserve vs time" plot
    if(lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }


    // Getting hold of how many days have passed since the facility lost functionality
    if (theFunctionality <= theThresholdfunctionality){
        //timeUnderRepairs += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    }

    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}
