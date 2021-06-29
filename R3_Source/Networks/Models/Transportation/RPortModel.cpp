#include "RPortModel.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include "RIntensityModel.h"
#include "RNormalDistribution.h"
#include "RMainWindow.h"
#include <math.h>

RPortModel::RPortModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theWFStructureType = RPortModel::WFNoDamage;
    theCranesType = RPortModel::CrNoDamage;
    theFuelFacilityType = RPortModel::FFNoDamage;

    // Ports are the only models for which susceptibility is assumed "High"
    theSoilSusceptibilityLevel = "High";


    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RPortModel::~RPortModel()
{
}


int RPortModel::resetTime()
{
    handlePacketsThatReachedFinalDestination();
    packetsCurrentlyOnboard.clear();
    theDamageProbabilities.clear();
    theWFDamageProbabilities.clear();
    theCranesDamageProbabilities.clear();
    theFFDamageProbabilities.clear();

    theTimeUnderRepairs = 0.0;
    theFunctionality = 1.0;

    return 0;
}



RPortModel::RWFStructureType RPortModel::getWFStructureType() const
{
    return theWFStructureType;
}

void RPortModel::setWFStructureType(RPortModel::RWFStructureType value)
{
    theWFStructureType = value;

    WFPGADamageMedians.clear();
    WFPGADamageDispersions.clear();
    WFPGDDamageMedians.clear();
    WFPGDDamageDispersions.clear();
    WFRestorationMedians.clear();
    WFRestorationStandardDeviations.clear();

    // see Hazus 2.1 pg. 354
    WFRestorationMedians << 0.6  << 3.5  << 22.0  << 85.0 ;
    WFRestorationStandardDeviations << 0.2 << 3.5 << 22.0 << 73.0;

    // No WFs
    if (theWFStructureType == RPortModel::WFNoDamage){

        WFPGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        WFPGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        WFPGDDamageMedians << 1000.0 << 1000.0 << 1000.0 << 1000.0;
        WFPGDDamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;
    }

    // Refer to HAZUS 2.1 pg. 355
    else if (theWFStructureType == RPortModel::WFStructure){

        WFPGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        WFPGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        WFPGDDamageMedians << 5.0 << 12.0 << 17.0 << 43.0;
        WFPGDDamageDispersions << 0.50 << 0.50 << 0.50 << 0.50;
    }

    else {

        qCritical() << " Invalid option for " << this->objectName();
    }
}


RPortModel::RCranesType RPortModel::getCranesType() const
{    
    return theCranesType;
}

void RPortModel::setCranesType(RPortModel::RCranesType value)
{
    theCranesType = value;

    cranesPGADamageMedians.clear();
    cranesPGADamageDispersions.clear();
    cranesPGDDamageMedians.clear();
    cranesPGDDamageDispersions.clear();
    cranesRestorationMedians.clear();
    cranesRestorationStandardDeviations.clear();

    // see Hazus 2.1 pg. 394
    cranesRestorationMedians << 0.4 << 6.0 << 30.0 << 75.0;
    cranesRestorationStandardDeviations << 0.35 << 6.0 << 30.0 << 55.0;

    // No Cranes
    if (theCranesType == RPortModel::CrNoDamage){

        cranesPGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        cranesPGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        cranesPGDDamageMedians << 1000.0 << 1000.0 << 1000.0 << 1000.0;
        cranesPGDDamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;
    }

    // Refer to HAZUS 2.1 pg. 356
    // Anchored/Stationary
    else if (theCranesType == RPortModel::Anchored){

        cranesPGADamageMedians << 0.30 << 0.50 << 1.00 << 1.00;
        cranesPGADamageDispersions << 0.60 << 0.60 << 0.70 << 0.70;

        cranesPGDDamageMedians << 3.0 << 6.0 << 12.0 << 12.0;
        cranesPGDDamageDispersions << 0.6 << 0.7 << 0.7 << 0.7;
    }

    // Unanchored/Rail mounted
    else if (theCranesType == RPortModel::Unanchored){

        cranesPGADamageMedians << 0.15 << 0.35 << 0.80 << 0.80;
        cranesPGADamageDispersions << 0.60 << 0.60 << 0.70 << 0.70;

        cranesPGDDamageMedians << 2.0 << 4.0 << 10.0 << 10.0;
        cranesPGDDamageDispersions << 0.6 << 0.6 << 0.7 << 0.7;
    }

    else {

        qCritical() << " Invalid option for " << this->objectName();
    }
}



RPortModel::RFuelFacilityType RPortModel::getFuelFacilityType() const
{
    return theFuelFacilityType;
}

void RPortModel::setFuelFacilityType(RPortModel::RFuelFacilityType value)
{
    theFuelFacilityType = value;


    FFPGADamageMedians.clear();
    FFPGADamageDispersions.clear();
    FFPGDDamageMedians.clear();
    FFPGDDamageDispersions.clear();
    FFRestorationMedians.clear();
    FFRestorationStandardDeviations.clear();

    // see Hazus 2.1 pg. 320 - same as for railway fuel facilities
    FFRestorationMedians << 0.9  << 1.5  << 15.0  << 65.0;
    FFRestorationStandardDeviations << 0.05 << 1.5 << 15.0 << 50.0;

    // No fuel facilities
    if (theFuelFacilityType == RPortModel::FFNoDamage){

        FFPGADamageMedians  << 10.0 << 10.0 << 10.0 << 10.0;
        FFPGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        FFPGDDamageMedians << 1000.0 << 1000.0 << 1000.0 << 1000.0;
        FFPGDDamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;
    }

    // Refer to HAZUS 2.1 pg. 323
    // Facility with anchored components w/ backup power
    else if (theFuelFacilityType == RPortModel::AnchoredBackUpPower){
        FFPGADamageMedians << 0.23 << 0.43 << 0.64 << 1.10;
        FFPGADamageDispersions << 0.50 << 0.45 << 0.60 << 0.60;

        FFPGDDamageMedians << 1000.0 << 1000.0 << 1000.0 << 1000.0;
        FFPGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with anchored components w/o backup power
    else if (theFuelFacilityType == RPortModel::AnchoredNoBackUpPower){
        FFPGADamageMedians << 0.12 << 0.27 << 0.64 << 1.10;
        FFPGADamageDispersions << 0.55 << 0.50 << 0.60 << 0.60;

        FFPGDDamageMedians << 1000.0 << 1000.0 << 1000.0 << 1000.0;
        FFPGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with unnchored components w/ backup power
    else if (theFuelFacilityType == RPortModel::UnanchoredBackUpPower){
        FFPGADamageMedians << 0.10 << 0.23 << 0.48 << 0.80;
        FFPGADamageDispersions << 0.55 << 0.50 << 0.60 << 0.60;

        FFPGDDamageMedians << 1000.0 << 1000.0 << 1000.0 << 1000.0;
        FFPGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility with unnchored components w/o backup power
    else if (theFuelFacilityType == RPortModel::UnanchoredNoBackUpPower){
        FFPGADamageMedians << 0.09 << 0.20 << 0.48 << 0.80;
        FFPGADamageDispersions << 0.50 << 0.45 << 0.60 << 0.60;

        FFPGDDamageMedians << 1000.0 << 1000.0 << 1000.0 << 1000.0;
        FFPGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Facility w/ buried tanks
    else if (theFuelFacilityType == RPortModel::Buried){
        FFPGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        FFPGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        FFPGDDamageMedians << 4.0 << 8.0 << 24.0 << 24.0;
        FFPGDDamageDispersions << 0.50 << 0.50 << 0.50 << 0.50;
    }

    else {
        qCritical() << " Invalid option for " << this->objectName();
    }
}


QString RPortModel::getFuelSupplierList()
{
    return parameterListToString(theFuelSupplierList);
}

void RPortModel::setFuelSupplierList(QString value)
{
    theFuelSupplierList = stringToParameterList(value);
}



int RPortModel::evaluateModel()
{	
    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }


    // -------------------------------------------------------------------------------------------
    // 1 - DAMAGE AND FUNCTIONALITY
    // -------------------------------------------------------------------------------------------
    // Earthquake
    //calculateIntensities();
    QVector<double> theIntensities = calculateIntensities();
    double thePGAIntensity = theIntensities.at(0);
    double thePGDIntensity = theIntensities.at(1);


    // WATERFRONT STRUCTURES
    RFragilityFunctions theWaterFrontStructures(theDomain);

    // Passing the fragility curves to the physical object that will calculated damage
    theWaterFrontStructures.setPGADamageMediansAndDispersions(WFPGADamageMedians, WFPGADamageDispersions);
    theWaterFrontStructures.setPGDDamageMediansAndDispersions(WFPGDDamageMedians, WFPGDDamageDispersions);
    theWaterFrontStructures.setRestorationMediansAndStandardDeviations(WFRestorationMedians, WFRestorationStandardDeviations);

    // Getting the damage and restoration results
    if(thePGAIntensity > 0.0 || thePGDIntensity > 0.0){
        theWFDamageProbabilities = theWaterFrontStructures.getDamageProbabilities(thePGAIntensity, thePGDIntensity);
    }
    QVector<double> theWFRestorationTimes = theWaterFrontStructures.getRestorationPercentagesGivenDelta(theTimeUnderRepairs);
    double WFFunctionality = 0.0;


    // Calculating the expected functionality
    if (theWFDamageProbabilities.size() > 0){
        for (int i = 0; i < theWFDamageProbabilities.size(); i++){
            WFFunctionality += theWFDamageProbabilities.at(i) * theWFRestorationTimes.at(i);
        }
    }
    else {
        WFFunctionality = 1.0;
    }


    // CRANES
    RFragilityFunctions theCranes(theDomain);

    // Passing the fragility curves to the physical object that will calculated damage
    theCranes.setPGADamageMediansAndDispersions(cranesPGADamageMedians, cranesPGADamageDispersions);
    theCranes.setPGDDamageMediansAndDispersions(cranesPGDDamageMedians, cranesPGDDamageDispersions);
    theCranes.setRestorationMediansAndStandardDeviations(cranesRestorationMedians, cranesRestorationStandardDeviations);


    // Getting the damage and restoration results
    if(thePGAIntensity > 0.0 || thePGDIntensity > 0.0){
        theCranesDamageProbabilities = theCranes.getDamageProbabilities(thePGAIntensity, thePGDIntensity);
    }
    QVector<double> theCranesRestorationTimes = theCranes.getRestorationPercentagesGivenDelta(theTimeUnderRepairs);
    double cranesFunctionality = 0.0;

    // Calculating the expected functionality
    if (theCranesDamageProbabilities.size() > 0){
        for (int i = 0; i < theCranesDamageProbabilities.size(); i++){
            cranesFunctionality += theCranesDamageProbabilities.at(i) * theCranesRestorationTimes.at(i);
        }
    }
    else {
        cranesFunctionality = 1.0;
    }


    //FUEL FACILITIES
    RFragilityFunctions theFuelFacility(theDomain);

    // Passing the fragility curves to the physical object that will calculated damage
    theFuelFacility.setPGADamageMediansAndDispersions(FFPGADamageMedians, FFPGADamageDispersions);
    theFuelFacility.setPGDDamageMediansAndDispersions(FFPGDDamageMedians, FFPGDDamageDispersions);
    theFuelFacility.setRestorationMediansAndStandardDeviations(FFRestorationMedians, FFRestorationStandardDeviations);


    // Getting the damage and restoration results
    if(thePGAIntensity > 0.0 || thePGDIntensity > 0.0){
        theFFDamageProbabilities = theFuelFacility.getDamageProbabilities(thePGAIntensity, thePGDIntensity);
    }

    QVector<double> theFFRestorationTimes = theFuelFacility.getRestorationPercentagesGivenDelta(theTimeUnderRepairs);
    double FFFunctionality = 0.0;


    // Calculating the expected functionality
    if (theFFDamageProbabilities.size() > 0){
        for (int i = 0; i < theFFDamageProbabilities.size(); i++){
            FFFunctionality += theFFDamageProbabilities.at(i) * theFFRestorationTimes.at(i);
        }
    }
    else {
        FFFunctionality = 1.0;
    }


    // OVERALL MINIMUM FUNCTIONALITY
    theFunctionality = qMin(qMin(WFFunctionality,cranesFunctionality),FFFunctionality);


    // LOAD AND UNLOAD PACKETS
    loadPackets(theFuelSupplierList);

    


    // Check if the element of the port with the minimum functionality has it above the threshold
    if (theFunctionality >= theThresholdfunctionality) {
        
        unloadPackets(qobject_cast<RPacketResponse *>(theResponse), packetsCurrentlyOnboard);
    }

    else {

        handlePacketsThatReachedFinalDestination();
    }

    // -------------------------------------------------------------------------------------------
    // 2 - HANDLING RESPONSES GETTING TO THIS MODEL
    // -------------------------------------------------------------------------------------------
    // Checking the availability of the suppliers
    for (int i = 0; i < theFuelSupplierList.size(); i++){

        // Get hold of the input response
        RPacketResponse *theResponseComingInHere = qobject_cast<RPacketResponse *>(theFuelSupplierList.at(i));
        if (!theResponseComingInHere) {
            qCritical() << this->objectName() << "did not find a proper input";
        }
    }


    //Fix to the fuel "reserve vs time" plot
    if(lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }


    // Getting hold of how many days have passed since the facility lost functionality
    if (theFunctionality <= theThresholdfunctionality){
        theTimeUnderRepairs += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    }

    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}


