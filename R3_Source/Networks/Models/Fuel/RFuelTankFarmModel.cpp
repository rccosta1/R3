#include "RFuelTankFarmModel.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include "RMainWindow.h"
#include "RNetworkManagerModel.h"
#include "RPowerLineModel.h"
#include <math.h>
#include <stdlib.h>
#include <algorithm>


RFuelTankFarmModel::RFuelTankFarmModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    durationOfShortage = 0.0;
    theFillLevel = 0;
    maxStorageCapacity = 0.0;
    theNumberOfTanks = 1;
    theReserve = 0.0;
    lastEvaluationTime = 0.0;


    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    thePacketResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePacketResponse->setModel(this);

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "ContinuousResponse", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}

RFuelTankFarmModel::~RFuelTankFarmModel()
{  
}


int RFuelTankFarmModel::resetTime()
{
    handlePacketsThatReachedFinalDestination();
    rngs.clear();
    packetsCurrentlyOnboard.clear();
    theDamageProbabilities.clear();
    theFunctionalityMatrix.clear();
    myAlternativeFuelSupplierList.clear();
    myPrimaryFuelSupplierList.clear();
    timeUnderRepairsVector.clear();
    numberOfRequestsCreatedHere = 0;
    timeOfLastRequest = 0.0;


    theAvailabilityOfPower = false;
    theCounter = 0.0;
    lim1 = 0.0;
    lim2 = 0.0;
    lim3 = 0.0;
    lim4 = 0.0;
    durationOfShortage = 0.0;
    lastEvaluationTime = 0.0;

    // Reserves
    theResponse->setCurrentValue(0.0);
    theReserve = maxStorageCapacity * theFillLevel->getCurrentValue();
    theCurrentCapacity = maxStorageCapacity;

    tickVector.clear();
    labelVector.clear();
    tickVector << 0;
    labelVector << QString("%1").arg(0);

    return 0;
}




RFuelTankFarmModel::RFacilityType RFuelTankFarmModel::getFacilityType() const
{
    return theFacilityType;
}

void RFuelTankFarmModel::setFacilityType(RFuelTankFarmModel::RFacilityType value)
{
    theFacilityType = value;

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 394
    theRepairTimeMedians << 1.2  << 3.1  << 93.0  << 155.0;
    theRepairTimeStandardDeviations << 0.4 << 2.7 << 85.0 << 120.0;

    // See Hazus 2.1 pg. 639
    theDamageRatios << 0.13 << 0.4 << 0.8 << 1.0;



    // If the tank cannot suffer damage
    if (value == RFuelTankFarmModel::NoDamage){

        thePGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;
    }


    // Refer to HAZUS 2.1 pg. 394 - same as for water
    // On-Ground Anchored Concrete Tank
    else if (value == RFuelTankFarmModel::OnGroundConcreteAnchored){

        thePGADamageMedians << 0.25 << 0.52 << 0.95 << 1.64;
        thePGADamageDispersions << 0.55 << 0.70 << 0.60 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // On-Ground Unanchored Concrete Tank
    else if (value == RFuelTankFarmModel::OnGroundConcreteUnanchored){

        thePGADamageMedians << 0.18 << 0.42 << 0.70 << 1.04;
        thePGADamageDispersions << 0.60 << 0.70 << 0.55 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // On-Ground Anchored Steel Tank
    else if (value == RFuelTankFarmModel::OnGroundSteelAnchored){

        thePGADamageMedians << 0.30 << 0.70 << 1.25 << 1.60;
        thePGADamageDispersions << 0.60 << 0.60 << 0.65 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // On-Ground Unanchored Steel Tank
    else if (value == RFuelTankFarmModel::OnGroundSteelUnanchored){

        thePGADamageMedians << 0.15 << 0.35 << 0.68 << 0.95;
        thePGADamageDispersions << 0.70 << 0.75 << 0.75 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // Above-Ground Steel Tank
    else if (value == RFuelTankFarmModel::AboveGroundSteel){

        thePGADamageMedians << 0.18 << 0.55 << 1.15 << 1.50;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // On-Ground Wood Tank
    else if (value == RFuelTankFarmModel::AboveGroundWood){

        thePGADamageMedians << 0.15 << 0.40 << 0.70 << 0.90;
        thePGADamageDispersions << 0.60 << 0.60 << 0.70 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // Buried Concrete Tank
    else if (value == RFuelTankFarmModel::BuriedConcrete){

        thePGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 2.0 << 4.0 << 8.0 << 12.0;
        thePGDDamageDispersions << 0.50 << 0.50 << 0.50 << 0.50;
    }

    else {

        qCritical() << " Invalid option for " << this->objectName();
    }
}


QString RFuelTankFarmModel::getFuelSupplierList()
{
    return parameterListToString(theFuelSupplierList);
}

void RFuelTankFarmModel::setFuelSupplierList(QString value)
{
    theFuelSupplierList = stringToParameterList(value);
}


double RFuelTankFarmModel::getMaxCapacity() const
{
    return maxStorageCapacity;
}


void RFuelTankFarmModel::setMaxCapacity(double value)
{
    maxStorageCapacity = value;
    theReserve = maxStorageCapacity * theFillLevel->getCurrentValue();
}


void RFuelTankFarmModel::changeCurrentCapacity(double value)
{
    theCurrentCapacity = value;
}


int RFuelTankFarmModel::getNumberOfStorageTanks() const
{
    return theNumberOfTanks;
}

void RFuelTankFarmModel::setNumberOfStorageTanks(int value)
{
    theNumberOfTanks = value;
}


QObject * RFuelTankFarmModel::getFillLevel() const
{
    return theFillLevel;
}

void RFuelTankFarmModel::setFillLevel(QObject *value)
{
    theFillLevel = qobject_cast<RParameter *>(value);
}


void RFuelTankFarmModel::setReserve(double value)
{
    theReserve = value;
    // avoid negative fuel levels
    if (theReserve < 0.0) {
        theReserve = 0.0;
    }
}


void RFuelTankFarmModel::setShortageDuration()
{
    shortageDuration << durationOfShortage;
}


void RFuelTankFarmModel::printShortageDuration()
{
    qDebug() << this->objectName() << shortageDuration;
}


int RFuelTankFarmModel::extractSupply(double amount)
{
    theReserve -= amount;

    // avoid negative fuel levels
    if (theReserve < 0.0) {
        theReserve = 0.0;
        return 0;
    }

    else {
        return 1;
    }
}



QString RFuelTankFarmModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RFuelTankFarmModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}


void RFuelTankFarmModel::getRequests()
{
    if (myPossiblePaths.size() == 0){
        getPossiblePaths();
    }


    // -------------------------------------------------------------------------------------------
    // 2 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    this->collectRequests(QString("Fuel"));


    // -------------------------------------------------------------------------------------------
    // Get hold of the requests received
    // -------------------------------------------------------------------------------------------

    // Next, get hold of what each requester needs
    double amountOfSuppliesRequested = 0.0;


    // First I get hold of all requests received from different objects. These are stored in a single-column vector.
    // For each model in need of financial assistance
    QVector<QPointer<RModel>> theUnsortedDestinationList;
    QVector<double> theUnsortedCriterionList;
    QVector<QVector<double>> theUnsortedAmountList;
    QVector<QString> theUnsortedSupplyTypeList;
    QList<QVector<int>> theUnsortedIDList;


    for (int i = 0; i < theRequestDestinationList.size(); i++){

        QVector<double> materialsForThisDestination;

        // Create an auxiliary list with the variable used as criteria to determine priority from all requests
        theUnsortedDestinationList << theRequestDestinationList.at(i);
        theUnsortedAmountList << theRequestedAmountList.at(i);
        theUnsortedSupplyTypeList << theRequestedSupplyTypeList.at(i);
        theUnsortedIDList << theRequesterIDList.at(i);


        // For each request that the model passed
        for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){


            materialsForThisDestination << 0.0;
            amountOfSuppliesRequested += theRequestedAmountList.at(i).at(j);
        }
    }



    while (theUnsortedCriterionList.size() > 0){

        // Find the requester with the highest bid
        int highestPriorityObject = theUnsortedCriterionList.indexOf(*std::max_element(theUnsortedCriterionList.constBegin(), theUnsortedCriterionList.constEnd()));

        theAuxDestinationList << theUnsortedDestinationList.at(highestPriorityObject);
        theAuxCriterionList << theUnsortedCriterionList.at(highestPriorityObject);
        theAuxAmountList << theUnsortedAmountList.at(highestPriorityObject);
        theAuxSupplyTypeList << theUnsortedSupplyTypeList.at(highestPriorityObject);
        theAuxIDList << theUnsortedIDList.at(highestPriorityObject);


        theUnsortedDestinationList.removeAt(highestPriorityObject);
        theUnsortedCriterionList.removeAt(highestPriorityObject);
        theUnsortedAmountList.removeAt(highestPriorityObject);
        theUnsortedSupplyTypeList.removeAt(highestPriorityObject);
        theUnsortedIDList.removeAt(highestPriorityObject);
    }




    if (theReserve < 0.5 * theCurrentCapacity && theCurrentTime->getCurrentValue() - timeOfLastRequest > 1.0){

        // Request construction materials
        QString materialsRequestName = QString(this->objectName() + "Request%1").arg(numberOfRequestsCreatedHere++);
        theDomain->createObject(materialsRequestName, "RRequest");
        RRequest *theNewRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());


        theNewRequest->setModel(this);
        theNewRequest->setContentType(QString("Fuel"));
        theNewRequest->setRequesterIDList(0);
        theNewRequest->setRequestedAmountList(maxStorageCapacity - theReserve);
        //qCritical() << this->objectName() << "is requesting" << materialsRequestName << "at" << theCurrentTime->getTime();

        timeOfLastRequest = theCurrentTime->getCurrentValue();
    }


    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();
}



int RFuelTankFarmModel::evaluateModel()
{
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }

    // "Reserve vs time" plot
    if(lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
        timeAnalysisBegan = theCurrentTime->getCurrentValue();
    }

    theCounter += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    double theInitialReserves = theReserve;


    // Define the tank capacity equal for all tanks (note: a tank farm contains several storage tanks)
    tankCapacity = maxStorageCapacity / theNumberOfTanks;


    // ***************************************************************
    // 1 - DAMAGE AND FUNCTIONALITY
    // ***************************************************************
    QVector<double> theIntensities = calculateIntensities();
    double thePGAIntensity = theIntensities.at(0);
    double thePGDIntensity = theIntensities.at(1);


    // Fragility Functions
    RFragilityFunctions theFragilityFunction(theDomain);


    // Passing the fragility curves to the physical object that will calculated damage
    theFragilityFunction.setPGADamageMediansAndDispersions(thePGADamageMedians, thePGADamageDispersions);
    theFragilityFunction.setPGDDamageMediansAndDispersions(thePGDDamageMedians, thePGDDamageDispersions);
    theFragilityFunction.setRestorationMediansAndStandardDeviations(theRepairTimeMedians, theRepairTimeStandardDeviations);


    // Getting the damage and restoration results
    if(thePGAIntensity > 0.0 || thePGDIntensity > 0.0){

        theDamageProbabilities = theFragilityFunction.getDamageProbabilities(thePGAIntensity, thePGDIntensity);

        // |----DS0----|---DS1---|--DS2--|--DS3--|-DS4-|
        // 0----------lim1------lim2----lim3----lim4---1
        lim1 = theDamageProbabilities.at(0);
        lim2 = lim1 + theDamageProbabilities.at(1);
        lim3 = lim2 + theDamageProbabilities.at(2);
        lim4 = lim3 + theDamageProbabilities.at(3);


        // Getting the damage and restoration realizations
        for (int i = 0; i < theNumberOfTanks; i++){
            double rng = rand();
            rngs << (double)rng/RAND_MAX;
            timeUnderRepairsVector << 0.0;
        }
    }


    // Now that damage was defined, check if the object has the necessary resources to repair
    // Define a list of packets to be loaded
    QList<QPointer <RParameter>> thePacketList;


    // Skipping this for now
    theAvailabilityOfPower = true;

    // Check if power is available
    for (int i = 0; i < thePowerSupplierList.size(); i++){

        // Cast the input as a response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(thePowerSupplierList.at(i));

        if (theResponseComingIntoThisModel){

            // Check for availability of power
            if (theResponseComingIntoThisModel->getCurrentValue() == 1.0){
                theAvailabilityOfPower = true;
            }
        }
    }


    for (int i = 0; i < theFuelSupplierList.size(); i++){


        // Get hold of the input response
        RPacketResponse  *thePacketResponseComingIntoThisModel = qobject_cast<RPacketResponse *>(theFuelSupplierList.at(i));


        // if this is a packet response
        if (thePacketResponseComingIntoThisModel){

            // Add this response to the packet list
            thePacketList << theFuelSupplierList.at(i);
        }
    }


    // Now, calculate functionality
    for (int i = 0; i < theNumberOfTanks; i++){

        double theTankFunctionality = 1.0;
        if (theFunctionalityMatrix.size() < theNumberOfTanks){
            theFunctionalityMatrix << 1.0;
        }


        // Check if the limits were defined before trying to compute functionality of each tank
        if (theDamageProbabilities.size() > 0){


            // Check for the availability of power to see if repairs can be performed
            if (theAvailabilityOfPower == true && theCurrentCapacity < maxStorageCapacity){
                timeUnderRepairsVector[i] += (theCurrentTime->getCurrentValue() - lastEvaluationTime) * 1.0;
            }
            else {
                //qCritical() << this->objectName() << "cannot perform repair actions without power";
            }

            QVector<double> theRestorationPercentages = theFragilityFunction.getRestorationPercentagesGivenDelta(timeUnderRepairsVector.at(i));


            // Generating realizations of a discrete random variable for the damage of the individual tanks
            if (rngs.at(i) < lim1){
                theTankFunctionality = theRestorationPercentages.at(0);
            }

            else if (rngs.at(i) >= lim1 && rngs.at(i) < lim2){
                theTankFunctionality = theRestorationPercentages.at(1);
            }

            else if (rngs.at(i) >= lim2 && rngs.at(i) < lim3){
                theTankFunctionality = theRestorationPercentages.at(2);
            }

            else if (rngs.at(i) >= lim3 && rngs.at(i) < lim4){
                theTankFunctionality = theRestorationPercentages.at(3);
            }

            else if (rngs.at(i) >= lim4){
                theTankFunctionality = theRestorationPercentages.at(4);
            }

            else {

                qCritical() << "Something went wrong in the RFuelTankFarmModel, the rng is" << rngs.at(i);
            }
        }

        else {
            theTankFunctionality = 1.0;
        }


        if (theTankFunctionality < theThresholdfunctionality && theFunctionalityMatrix.at(i) > theThresholdfunctionality){

            // If this tank is damaged, reduce the tank farm capacity
            changeCurrentCapacity(theCurrentCapacity - tankCapacity);


            if (theCurrentCapacity > 0.0){
                setReserve(theReserve - tankCapacity * (theReserve / maxStorageCapacity));
            }

            else {
                setReserve(0.0);
            }
        }

        // If a tank is repaired, add its capacity to the current capacity
        else if (theTankFunctionality > theThresholdfunctionality && theFunctionalityMatrix.at(i) < theThresholdfunctionality){

            changeCurrentCapacity(theCurrentCapacity + tankCapacity);
        }

        // Define the functionality matrix
        theFunctionalityMatrix[i] = theTankFunctionality;
    }

    // Calculating the functionality based on the functionality of the storage tanks
    theFunctionality = 0.0;

    theFunctionality = theFunctionalityMatrix.at(0);

    // If there is one functional tank, the tank farm is functional - its capacity was already reduced to reflect the loss of storage tanks
    for (int i = 0; i < theFunctionalityMatrix.size(); i++){
        if (theFunctionalityMatrix.at(i) > theFunctionality){
            theFunctionality = theFunctionalityMatrix.at(i);
        }
    }


    // Reset availability and response
    theResponse->setCurrentValue(0.0);


    // ***************************************************************
    // 2 - HANDLING RESPONSES COMING TO THIS MODEL
    // ***************************************************************
    // Checking the availability of the FuelSuppliers
    if (theFunctionality >= theThresholdfunctionality){


        int numPackets = 0;
        loadPackets(thePacketList);
        numPackets = packetsCurrentlyOnboard.size();


        if (numPackets > 0){

            // Extract contents
            for (int j = 0; j < numPackets; j++) {

                // Get hold of what is the content of this packet
                QVector<double> packetContents = packetsCurrentlyOnboard[j]->getPacketContents();
                QString packetType = packetsCurrentlyOnboard[j]->getPacketContentsType();

                if (packetType == QString("Fuel")){

                    for (int k = 0; k < packetContents.size(); k++){
                        if (theOutputLevel == RObject::Maximum) {
                            //qCritical() << "";
                            //qDebug() << this->objectName() << "is now extracting" << packetContents.at(k) << packetType << "from" << packetsCurrentlyOnboard[j]->objectName() << theCurrentTime->getTime();
                        }


                        theReserve += packetContents.at(k);


                        if (theReserve > theCurrentCapacity){
                            theReserve = theCurrentCapacity;
                        }
                    }
                }
            }
        }

        theResponse->setCurrentValue(1.0);
    }


    // This is a sink model - delete packets independently of functionality/availability
    handlePacketsThatReachedFinalDestination();




    // ***************************************************************
    // * 4 - Outputs
    // ***************************************************************
    if (theReserve <= 0.0){
        durationOfShortage += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    }

    // Maximum time under repairs
    //timeUnderRepairs = *std::max_element(timeUnderRepairsVector.constBegin(), timeUnderRepairsVector.constEnd());


    double theFinalReserve = theReserve;

    // If a shipment was received
    if ((theFinalReserve - theInitialReserves) > 0.0){
        this->plotCurrentReserves(0.0, theInitialReserves);
        this->plotCurrentReserves(0.01, theFinalReserve);
    }

    // If there a storage tank was lost
    else if ((thePGAIntensity > 0.0 || thePGDIntensity > 0.0) && theCurrentCapacity < maxStorageCapacity){
        this->plotCurrentReserves(0.0, theInitialReserves);
        this->plotCurrentReserves(0.01, theFinalReserve);
    }

    // If it has been more than X day since last plot
    if (theCounter > 1.0){
        this->plotCurrentReserves(0.0, theFinalReserve);
        theCounter = 0.0;
    }



    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}


void RFuelTankFarmModel::plotCurrentReserves(double delta, double currentReserves)
{
    if (theOutputLevel >= RObject::Medium) {

        if (customPlot == 0) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            customPlot = new QCustomPlot();
            QString theString = QString("Gasoline in %1").arg(this->objectName());
            mainWindow->addSubWindow(customPlot, theString, true);
            customPlot->addGraph();
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            customPlot->xAxis->setLabel("Time");
            customPlot->yAxis->setLabel(theString);
            customPlot->xAxis->setAutoTicks(false);
            customPlot->xAxis->setAutoTickLabels(false);
        }
        else {
            //customPlot->graph(0)->clearData();
        }


        double timeElapsed = lastEvaluationTime - timeAnalysisBegan + delta;

        if (delta <= 0.0){

            customPlot->graph(0)->addData(timeElapsed, currentReserves);
            customPlot->yAxis->setRange(0, maxStorageCapacity);
            customPlot->xAxis->setRange(0, timeElapsed + 1);

            tickVector << timeElapsed;
            labelVector << QString("%1").arg(timeElapsed);
            //customPlot->rescaleAxes();
            customPlot->xAxis->setTickVector(tickVector);
            customPlot->xAxis->setTickVectorLabels(labelVector);
            customPlot->replot();

        }
        else if (delta > 0.0){
            customPlot->graph(0)->addData(timeElapsed, currentReserves);
            theCounter = 0.0;
        }
    }
}



// ***************************************************************
// * Requests
// ***************************************************************
//if (theReserve < 0.5 * theCurrentCapacity){

//    // Get hold of all object in need of supplies
//    QList<RRequest *> theAllRequestList = theDomain->findChildren<RRequest *>();

//    bool requestChecker = false;
//    for (int i = 0; i < theAllRequestList.size(); i++){
//        if (theAllRequestList.at(i)->objectName().contains(this->objectName())) {
//            //qCritical() << this->objectName() << "is trying to put a new request but the previous one was not taken yet!";
//            requestChecker = true;
//        }
//    }

//    if (requestChecker == false){
//        QString fuelRequestName = QString(this->objectName() + "FuelRequest%1").arg(numberOfFuelRequestsCreatedHere++);
//        theDomain->createObject(fuelRequestName, "RRequest");
//        RRequest *theNewFuelRequest = qobject_cast<RRequest *>(theDomain->getLastAddedObject());
//        theNewFuelRequest->setModel(this);
//        theNewFuelRequest->setContentType(QString("Fuel"));
//        theNewFuelRequest->setRequestCriteria(0.0);
//            theNewFuelRequest->setRequesterIDList(qMakePair(0,0));
//        theNewFuelRequest->setRequestedAmountList(theCurrentCapacity - theReserve);
//    }
//}






/*
// ALA - American Lifeline Alliance, Seismic fragility formulation for water system: Part 1-Guideline, ASCE, 2001a.
if(level < 0.5){

   thePGADamageMedians << 0.56 << 2.0 << 10.0 << 10.0;
    thePGADamageDispersions << 0.8 << 0.4 << 0.8 << 0.8;
    //qCritical() << "The " << this->objectName() << " is using a FC for FillLevel less than 50% since it is " << theFillLevel << " full.";
}

else if(level >= 0.5 && level < 0.6){

   thePGADamageMedians << 0.18 << 0.73 << 1.14 << 1.16;
    thePGADamageDispersions << 0.8 << 0.8 << 0.8 << 0.40;
    //qCritical() << "The " << this->objectName() << " is using a FC for FillLevel between 50% and 60% since it is " << theFillLevel << " full.";
}

else if(level >= 0.6 && level < 0.9){

   thePGADamageMedians << 0.22 << 0.70 << 1.09 << 1.16;
    thePGADamageDispersions << 0.80 << 0.80 << 0.80 << 0.41;
    //qCritical() << "The " << this->objectName() << " is using a FC for FillLevel between 60% and 90% since it is " << theFillLevel << " full.";
}

else if(level >= 0.9){

   thePGADamageMedians << 0.13 << 0.67 << 1.01 << 1.15;
    thePGADamageDispersions << 0.07 << 0.80 << 0.80 << 0.10;
    //qCritical() << "The " << this->objectName() << " is using a FC for FillLevel > 90% since it is " << theFillLevel << " full.";
}
*/


/*
//Calculating the capacity of each tank assuming they all have the same size
double theTankCapacity = theCurrentCapacity/theNumberOfTanks;
double thisTankFillLevel = 0.0;
double theRemainingFuel = theReserve;


//Distributing the fuel to the tanks and calculate their damage states
for (int i=0; i<theNumberOfTanks; i++){

    //If this is a full tank
    if (theRemainingFuel >= theTankCapacity){
        thisTankFillLevel = 1.0;
        theRemainingFuel = theRemainingFuel - theTankCapacity;
    }

    //If this is a half-full or empty tank
    else if (theRemainingFuel < theTankCapacity){
        thisTankFillLevel = theRemainingFuel/theTankCapacity;
        theRemainingFuel = 0.0;
    }

    /*
    theStorageTank.at[i]->setPGAFragilityCurve(thisTankFillLevel);
    theStorageTank.at[i]->calculateIntensities();

    //If the tank collapses (DS=4) the reserves stored in it are lost
    if (meanDamageRatio == 4.0){
        theReserve = theReserve - 1.0*thisTankFillLevel*theTankCapacity;
    }

    //If the tank experiencces severe damage (3<DS<4) there is partial lost of reserves (assuming 50%)
    else if (meanDamageRatio >= 3.0){
        theReserve = theReserve - 0.5*thisTankFillLevel*theTankCapacity;
    }

    if (meanRestorationRatio < 0.8){
        theTankCapacity = theCurrentCapacity - theTankCapacity;
    }

    setPGAFragilityCurve(thisTankFillLevel);
    //calculateIntensitiesForSubElement();
    //thisTankMeanRestorationRatio[i] = meanDamageRatio;
    //qCritical() << "The value inside is " << meanDamageRatio;

    thisTankFillLevel = 0.0;
}
*/

// PGA FRAGILITY CURVES - DEPENDENT ON FUEL LEVEL
//theFragilityCurves->setDamageMediansAndDispersions (PGADamageMedians, thePGADamageDispersions);

/*
// Histogram
double sum = theDamageProbabilities[2] + theDamageProbabilities[3] + theDamageProbabilities[4];

for (int i=0; i<20; i++) {

    if (sum >= 0.05*i && sum < 0.05*(i+1)){
        theDamageBins[i] += 1.0;
    }
}
*/
