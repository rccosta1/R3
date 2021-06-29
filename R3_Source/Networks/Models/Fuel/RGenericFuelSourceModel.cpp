#include "RGenericFuelSourceModel.h"
#include "RParameter.h"
#include "RGenericFuelSourceModel.h"
#include "RMainWindow.h"
#include "RPacket.h"
#include <math.h>

RGenericFuelSourceModel::RGenericFuelSourceModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{    
    theFuelReserve = 0.0;
    numberOfNewPacketsCreatedHere = 0;

    theFacilityType = RGenericFuelSourceModel::NoDamage;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RGenericFuelSourceModel::~RGenericFuelSourceModel()
{

}


int RGenericFuelSourceModel::resetTime()
{
    handlePacketsThatReachedFinalDestination();
    packetsCurrentlyOnboard.clear();
    theDamageProbabilities.clear();
    theFuelReserve = theStorageCapacity;
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();

    numberOfNewPacketsCreatedHere = 0;
    lastEvaluationTime = 0.0;
    theFunctionality = 1.0;
    theTimeUnderRepairs = 0.0;
    timeOfEvent = 0.0;

    theResponse->setCurrentValue(0.0);

    return 0;
}


RGenericFuelSourceModel::RFacilityType RGenericFuelSourceModel::getFacilityType() const
{
    return theFacilityType;
}

void RGenericFuelSourceModel::setFacilityType(RGenericFuelSourceModel::RFacilityType value)
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
    if (value == RGenericFuelSourceModel::NoDamage){

        thePGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;
    }


    // Refer to HAZUS 2.1 pg. 394 - same as for water
    // On-Ground Anchored Concrete Tank
    else if (value == RGenericFuelSourceModel::OnGroundConcreteAnchored){

        thePGADamageMedians << 0.25 << 0.52 << 0.95 << 1.64;
        thePGADamageDispersions << 0.55 << 0.70 << 0.60 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // On-Ground Unanchored Concrete Tank
    else if (value == RGenericFuelSourceModel::OnGroundConcreteUnanchored){

        thePGADamageMedians << 0.18 << 0.42 << 0.70 << 1.04;
        thePGADamageDispersions << 0.60 << 0.70 << 0.55 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // On-Ground Anchored Steel Tank
    else if (value == RGenericFuelSourceModel::OnGroundSteelAnchored){

        thePGADamageMedians << 0.30 << 0.70 << 1.25 << 1.60;
        thePGADamageDispersions << 0.60 << 0.60 << 0.65 << 0.60;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // On-Ground Unanchored Steel Tank
    else if (value == RGenericFuelSourceModel::OnGroundSteelUnanchored){

        thePGADamageMedians << 0.15 << 0.35 << 0.68 << 0.95;
        thePGADamageDispersions << 0.70 << 0.75 << 0.75 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // Above-Ground Steel Tank
    else if (value == RGenericFuelSourceModel::AboveGroundSteel){

        thePGADamageMedians << 0.18 << 0.55 << 1.15 << 1.50;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // On-Ground Wood Tank
    else if (value == RGenericFuelSourceModel::AboveGroundWood){

        thePGADamageMedians << 0.15 << 0.40 << 0.70 << 0.90;
        thePGADamageDispersions << 0.60 << 0.60 << 0.70 << 0.70;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }


    // Buried Concrete Tank
    else if (value == RGenericFuelSourceModel::BuriedConcrete){

        thePGADamageMedians << 10.0 << 10.0 << 10.0 << 10.0;
        thePGADamageDispersions << 0.10 << 0.10 << 0.10 << 0.10;

        thePGDDamageMedians << 2.0 << 4.0 << 8.0 << 12.0;
        thePGDDamageDispersions << 0.50 << 0.50 << 0.50 << 0.50;
    }

    else {

        qCritical() << " Invalid option for " << this->objectName();
    }
}


double RGenericFuelSourceModel::getStorageCapacity() const
{
    return theStorageCapacity;
}

void RGenericFuelSourceModel::setStorageCapacity(double value)
{
    theFuelReserve = value;
    theStorageCapacity = value;
}


QString RGenericFuelSourceModel::getFuelSupplierList()
{
    return parameterListToString(theFuelSupplierList);
}

void RGenericFuelSourceModel::setFuelSupplierList(QString value)
{
    theFuelSupplierList = stringToParameterList(value);
}


QString RGenericFuelSourceModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RGenericFuelSourceModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}




int RGenericFuelSourceModel::evaluateModel()
{
//    if (theOutputLevel == RDomain::Maximum) {
//        qDebug() << "";
//        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
//    }


//    // Creating a list of paths that contain this model - getting hold of its connections
//    if (myPossiblePaths.size() == 0){
//        getPossiblePaths();
//    }


//    // -------------------------------------------------------------------------------------------
//    // 1 - Handling responses coming to this model
//    // -------------------------------------------------------------------------------------------
//    // Now that damage was defined, check if the object has the necessary resources to repair
//    // Handle the packets delivered
//    QList<QPointer <RParameter>> packetList;


//    for (int i = 0; i < theFuelSupplierList.size(); i++){

//        // Get hold of the input response
//        RResponse *theResponseComingIntoThisModel = qobject_cast<RGenericResponse *>(theFuelSupplierList.at(i));


//        // If this is a packet response
//        if (theResponseComingIntoThisModel){


//            // Add this response to the packet list
//            packetList << theFuelSupplierList.at(i);
//        }
//    }

//    // ignoring this for now
//    theAvailabilityOfPower = true;

//    // Check if power is available
//    for (int i = 0; i < thePowerSupplierList.size(); i++){

//        // Cast the input as a response
//        RResponse *theResponseComingIntoThisModel = qobject_cast<RResponse *>(thePowerSupplierList.at(i));

//        if (theResponseComingIntoThisModel){

//            // Check for availability of power
//            if (theResponseComingIntoThisModel->getCurrentValue() == 1.0){
//                theAvailabilityOfPower = true;
//            }
//        }
//    }



//    // -------------------------------------------------------------------------------------------
//    // 2 - DAMAGE AND FUNCTIONALITY
//    // -------------------------------------------------------------------------------------------
//    // Earthquake
//    QVector<double> theIntensities = calculateIntensities();
//    double thePGAIntensity = theIntensities.at(0);
//    double thePGDIntensity = theIntensities.at(1);


//    // Fragility Functions
//    RFragilityFunctions theFragilityFunction(theDomain);


//    // Passing the fragility curves to the physical object that will calculated damage
//    theFragilityFunction.setPGADamageMediansAndDispersions(thePGADamageMedians, thePGADamageDispersions);
//    theFragilityFunction.setPGDDamageMediansAndDispersions(thePGDDamageMedians, thePGDDamageDispersions);
//    theFragilityFunction.setRestorationMediansAndStandardDeviations(theRepairTimeMedians, theRepairTimeStandardDeviations);


//    // Getting the damage and restoration results
//    if(thePGAIntensity > 0.0 || thePGDIntensity > 0.0){
//        theDamageProbabilities = theFragilityFunction.getDamageProbabilities(thePGAIntensity, thePGDIntensity);
//    }


//    // Calculating the expected functionality
//    if (theDamageProbabilities.size() > 0){

//        // Check for the availability of power to see if repairs can be performed
//        if (theAvailabilityOfPower == true && theFunctionality <= theThresholdfunctionality){
//            theTimeUnderRepairs += (theCurrentTime->getCurrentValue() - lastEvaluationTime) * 1.0;
//        }

//        else {
//            //qCritical() << this->objectName() << "cannot perform repair actions without power";
//        }

//        QVector<double> theRestorationPercentages = theFragilityFunction.getRestorationPercentagesGivenDelta(theTimeUnderRepairs);


//        theFunctionality = 0.0;


//        for (int i = 0; i < theDamageProbabilities.size(); i++){
//            theFunctionality += theDamageProbabilities.at(i) * theRestorationPercentages.at(i);
//        }
//    }

//    else {
//        theFunctionality = 1.0;
//    }


//    // Loading and handling packets coming to this model
//    if (theFunctionality >= theThresholdfunctionality){
//        loadPackets(packetList);
//    }



//    // -------------------------------------------------------------------------------------------
//    // Check if there are any requests to be picked up in the domain
//    // -------------------------------------------------------------------------------------------

//    // Checking the availability of the suppliers
//    if (theFunctionality >= theThresholdfunctionality){


//        // Creating a list of paths that contain this model - getting hold of its connections
//        if (myPossiblePaths.size() == 0){
//            getPossiblePaths();
//        }


//        // Get hold of all object in need of supplies
//        QList<RRequest *> theAllRequestList = theDomain->findChildren<RRequest *>();
//        QList<RRequest *> theListOfCollectedRequests;


//        for (int i = 0; i < theAllRequestList.size(); i++){


//            // Collecting the requests that need money
//            if(theAllRequestList.at(i)->getContentType() == QString("Fuel")){

//                // Define the shortest path between this model and the requester (if one exists)
//                this->getShortestPath(theAllRequestList.at(i)->getModel());


//                // If this request is for money and this model can supply it
//                if (theShortestPath.size() > 0){

//                    // Add this request to the list of requested taken
//                    theListOfCollectedRequests << theAllRequestList.at(i);


//                    QVector<double> theRequestedAmounts = theAllRequestList.at(i)->getRequestedAmountList();
//                    QVector<QPair<int,int>> theIDs = theAllRequestList.at(i)->getRequesterIDList();
//                    QVector<double> theCriteria = theAllRequestList.at(i)->getListOfCriteria();


//                    // Add the information from this request to the list of requests picked by this object
//                    this->setListOfRequesters(theAllRequestList.at(i)->getModel());
//                    this->setListOfRequestedSupplies(theRequestedAmounts, theAllRequestList.at(i)->getContentType(), theCriteria, theIDs);
//                }
//            }
//        }


//        // Delete the requests that were picked up by this object
//        int numberOfRequestsCollected = theListOfCollectedRequests.size();
//        for (int i = 0; i < numberOfRequestsCollected; i++){
//            theDomain->deleteObject(theListOfCollectedRequests.at(i)->objectName());
//        }



//        // -------------------------------------------------------------------------------------------
//        // Get hold of the requests received
//        // -------------------------------------------------------------------------------------------
//        // Next, get hold of what each requester needs
//        QVector<QVector<double>> fuelToBeProviveded;
//        QVector<QPair<int,int>> theMatrixOfIndexes;
//        QVector<double> theAuxCriterionList; // this is a copy of the list of bids
//        QVector<double> theAuxAmountList; // this is a copy of the list of requests


//        // First I get hold of all requests received from different objects. These are stored in a single-column vector.
//        // For each model in need of financial assistance
//        for (int i = 0; i < theRequestDestinationList.size(); i++){

//            QVector<double> assistance;

//            // For each request that the model passed
//            for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){

//                theMatrixOfIndexes << qMakePair(i,j);

//                // Create an auxiliary list with the variable used as criteria to determine priority from all requests
//                theAuxCriterionList << theRequestCriterionList.at(i).at(j);
//                theAuxAmountList << theRequestedAmountList.at(i).at(j);

//                assistance << 0.0;
//            }

//            fuelToBeProviveded << assistance;
//        }



//        // -------------------------------------------------------------------------------------------
//        // Provide assistance for the households that have the lowest income
//        // -------------------------------------------------------------------------------------------
//        // Now, I start supply objects based on the decision criteria. In this case, lowest income (a proxy of economic status) gets it first.
//        // NOTE: if two or more requests present the same value for a given criterion, then it becomes 'first-come-first-serve'.
//        while (theAuxCriterionList.size() > 0){


//            // Find the household with lowest income
//            double highestPriorityObject = *std::min_element(theAuxCriterionList.constBegin(), theAuxCriterionList.constEnd());
//            int highestPriorityI = theMatrixOfIndexes.at(theAuxCriterionList.indexOf(highestPriorityObject)).first;
//            int highestPriorityJ = theMatrixOfIndexes.at(theAuxCriterionList.indexOf(highestPriorityObject)).second;


//            // Provide the requested assistance if possible
//            if (theFuelReserve >= theAuxAmountList.at(theAuxCriterionList.indexOf(highestPriorityObject))){

//                fuelToBeProviveded[highestPriorityI][highestPriorityJ] = theAuxAmountList.at(theAuxCriterionList.indexOf(highestPriorityObject));
//                theFuelReserve -= theAuxAmountList.at(theAuxCriterionList.indexOf(highestPriorityObject));
//            }

//            // If not, provide as much as there is available
//            else {
//                fuelToBeProviveded[highestPriorityI][highestPriorityJ] = theFuelReserve;
//                theFuelReserve -= theFuelReserve;
//            }


//            theAuxAmountList.removeAt(theAuxCriterionList.indexOf(highestPriorityObject));
//            theAuxCriterionList.removeAt(theAuxCriterionList.indexOf(highestPriorityObject));
//            theMatrixOfIndexes.removeAt(theMatrixOfIndexes.indexOf(qMakePair(highestPriorityI,highestPriorityJ)));
//        }



//        // Create only one packet per object requesting supplies. This packet will contain a list of requests.
//        // For each object requesting financial aid
//        int requestCounter = 0;
//        for (int i = 0; i < theRequestDestinationList.size(); i++){


//            // Creating a packet
//            QString packetName = QString(this->objectName() + "Packet%1").arg(numberOfNewPacketsCreatedHere++);
//            theDomain->createObject(packetName, "RPacket");
//            RPacket *theNewPacket = qobject_cast<RPacket *>(theDomain->getLastAddedObject());
//            //theNewPacket->setPacketFinalDestination(theRequestDestinationList.at(i));
//            theNewPacket->setPacketContentsType(theRequestedSupplyTypeList.at(i));


//            // For each request that the model passed
//            for (int j = 0; j < theRequestedAmountList.at(i).size(); j++){
//                double thePacketContents = fuelToBeProviveded.at(i).at(j);
//                theNewPacket->setPacketContents(thePacketContents);
//                theNewPacket->setPacketOwner(theRequesterIDList.at(i).at(j));
//                theNewPacket->setDestinationList(theShortestPath);
//                requestCounter += 1;

//                //qCritical() << this->objectName() << "creating a packet to" << theNewPacket->getPacketOwner() << "that will go through" << theShortestPath << "carrying" <<  theNewPacket->getPacketContents();
//            }

//            packetsCurrentlyOnboard << theNewPacket;
//        }


//        // Unload packets into the packet response
//        unloadPackets(qobject_cast<RPacketResponse *>(theResponse), packetsCurrentlyOnboard);
//        packetsCurrentlyOnboard.clear();



//        // Clear all lists
//        theRequestDestinationList.clear();
//        theRequesterIDList.clear();
//        theRequestCriterionList.clear();
//        theRequestedSupplyTypeList.clear();
//        theRequestedAmountList.clear();


//    } // if functionality

//    // If this object is not functional, destroy the packets arriving here
//    else {

//        // Delete packets that got here but could not be loaded
//        handlePacketsThatReachedFinalDestination();
//        theResponse->setCurrentValue(0.0);
//    }


//    // "Reserve vs time" plot
//    if(lastEvaluationTime == 0.0){
//        lastEvaluationTime = theCurrentTime->getCurrentValue();
//    }

//    // -------------------------------------------------------------------------------------------
//    // 2 - OUTPUT
//    // -------------------------------------------------------------------------------------------
//    if (theOutputLevel >= RObject::Medium) {

//        if (customPlot == 0) {

//            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

//            customPlot = new QCustomPlot();
//            QString theString = QString("Reserve in %1").arg(this->objectName());
//            mainWindow->addSubWindow(customPlot, theString, true);

//            customPlot->addGraph();
//            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));

//            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
//            customPlot->xAxis->setLabel("Time");
//            customPlot->yAxis->setLabel(theString);

//            customPlot->xAxis->setAutoTicks(true);
//            customPlot->xAxis->setAutoTickLabels(false);
//        }

//        customPlot->graph(0)->addData(lastEvaluationTime, theFuelReserve);
//        customPlot->rescaleAxes();
//        customPlot->replot();
//    }

//    lastEvaluationTime = theCurrentTime->getCurrentValue();

    return 1;
}




