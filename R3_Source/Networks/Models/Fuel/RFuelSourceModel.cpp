#include "RFuelSourceModel.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include <math.h>

RFuelSourceModel::RFuelSourceModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RFuelSourceModel::~RFuelSourceModel()
{
}

RFuelSourceModel::RFacilityType RFuelSourceModel::getFacilityType() const
{
    return theFacilityType;
}

void RFuelSourceModel::setFacilityType(RFuelSourceModel::RFacilityType value)
{
    theFacilityType = value;

    thePGADamageMedians.clear();
    thePGADamageDispersions.clear();
    thePGDDamageMedians.clear();
    thePGDDamageDispersions.clear();
    theRepairTimeMedians.clear();
    theRepairTimeStandardDeviations.clear();

    // see Hazus 2.1 pg. 452
    theRepairTimeMedians << 0.5 << 3.6 << 22.0 << 65.0;
    theRepairTimeStandardDeviations << 0.1 << 3.6 << 21.0 << 30.0;

    // Refer to HAZUS 2.1 pg. 455 subcomponents
    // Small plants with anchored subcomponents
    if (value == 1){
        thePGADamageMedians << 0.10 << 0.21 << 0.48 << 0.78;
        thePGADamageDispersions << 0.55 << 0.55 << 0.50 << 0.50;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Small plants with unanchored subcomponents
    else if (value == 2){
        thePGADamageMedians << 0.10 << 0.17 << 0.42 << 0.58;
        thePGADamageDispersions << 0.50 << 0.50 << 0.50 << 0.55;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Medium/large plants with anchored subcomponents
    else if (value == 3){
        thePGADamageMedians << 0.10 << 0.25 << 0.52 << 0.92;
        thePGADamageDispersions << 0.60 << 0.60 << 0.55 << 0.55;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    // Medium/large plants with unanchored subcomponents
    else if (value == 4){
        thePGADamageMedians << 0.10 << 0.22 << 0.49 << 0.79;
        thePGADamageDispersions << 0.60 << 0.55 << 0.50 << 0.50;

        thePGDDamageMedians << 100.0 << 100.0 << 100.0 << 100.0;
        thePGDDamageDispersions << 0.1 << 0.1 << 0.1 << 0.1;
    }

    else {

        qCritical() << " Invalid option for " << this->objectName();
    }
}


double RFuelSourceModel::getStorageCapacity() const
{
    return theStorageCapacity;
}

void RFuelSourceModel::setStorageCapacity(double value)
{
    theFuelReserve = value;
    theStorageCapacity = value;
}


QString RFuelSourceModel::getFuelSupplierList()
{
    return parameterListToString(theFuelSupplierList);
}

void RFuelSourceModel::setFuelSupplierList(QString value)
{
    theFuelSupplierList = stringToParameterList(value);
}


QString RFuelSourceModel::getPowerSupplierList()
{
    return parameterListToString(thePowerSupplierList);
}

void RFuelSourceModel::setPowerSupplierList(QString value)
{
    thePowerSupplierList = stringToParameterList(value);
}


void RFuelSourceModel::getRequests()
{
    if (myPossiblePaths.size() == 0){
        getPossiblePaths();
    }


    // -------------------------------------------------------------------------------------------
    // 2 - Checking requests received
    // -------------------------------------------------------------------------------------------
    // Creating a list of paths that contain this model - getting hold of its connections
    // Get hold of all object in need of supplies
    this->collectRequests(QString("WoodLogs"));


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


    for (int i = 0; i < theRequestDestinationList.size(); i++){

        QVector<double> materialsForThisDestination;

        // Create an auxiliary list with the variable used as criteria to determine priority from all requests
        theUnsortedDestinationList << theRequestDestinationList.at(i);
        theUnsortedAmountList << theRequestedAmountList.at(i);
        theUnsortedSupplyTypeList << theRequestedSupplyTypeList.at(i);


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


        theUnsortedDestinationList.removeAt(highestPriorityObject);
        theUnsortedCriterionList.removeAt(highestPriorityObject);
        theUnsortedAmountList.removeAt(highestPriorityObject);
        theUnsortedSupplyTypeList.removeAt(highestPriorityObject);
    }



    theRequesterIDList.clear();
    theRequestedSupplyTypeList.clear();
    theRequestedAmountList.clear();
    theRequestDestinationList.clear();
}



int RFuelSourceModel::evaluateModel()
{
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }



    // -------------------------------------------------------------------------------------------
    // 1 - Handling responses coming to this model
    // -------------------------------------------------------------------------------------------
    // Now that damage was defined, check if the object has the necessary resources to repair
    // Handle the packets delivered
    QList<QPointer <RParameter>> thePacketList;


    for (int i = 0; i < theFuelSupplierList.size(); i++){

        // Get hold of the input response
        RResponse *theResponseComingIntoThisModel = qobject_cast<RGenericResponse *>(theFuelSupplierList.at(i));


        // If this is a packet response
        if (theResponseComingIntoThisModel){


            // Add this response to the packet list
            thePacketList << theFuelSupplierList.at(i);
        }
    }

    // ignoring this for now
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



    // -------------------------------------------------------------------------------------------
    // 2 - DAMAGE AND FUNCTIONALITY
    // -------------------------------------------------------------------------------------------
    // Earthquake
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
    }


    // Calculating the expected functionality
    if (theDamageProbabilities.size() > 0){

        // Check for the availability of power to see if repairs can be performed
        if (theAvailabilityOfPower == true && theFunctionality <= theThresholdfunctionality){
            theTimeUnderRepairs += (theCurrentTime->getCurrentValue() - lastEvaluationTime) * 1.0;
        }

        else {
            //qCritical() << this->objectName() << "cannot perform repair actions without power";
        }

        QVector<double> theRestorationPercentages = theFragilityFunction.getRestorationPercentagesGivenDelta(theTimeUnderRepairs);


        theFunctionality = 0.0;


        for (int i = 0; i < theDamageProbabilities.size(); i++){
            theFunctionality += theDamageProbabilities.at(i) * theRestorationPercentages.at(i);
        }
    }

    else {
        theFunctionality = 1.0;
    }


    // Loading and handling packets coming to this model
    if (theFunctionality >= theThresholdfunctionality){
        loadPackets(thePacketList);
    }


    this->handlePacketsThatReachedFinalDestination();



    // -------------------------------------------------------------------------------------------
    // Provide materials for the households that have the highest bids
    // -------------------------------------------------------------------------------------------
    // Create only one packet per object requesting supplies. This packet will contain a list of requests.
    // For each object requesting financial aid
    if (theFuelReserve < 1){
        return 0;
    }


    if (theFunctionality >= theThresholdfunctionality){
        for (int i = 0; i < theAuxDestinationList.size(); i++){

            this->getShortestPath(theAuxDestinationList.at(i));


            // Creating a packet
            QString packetName = QString(this->objectName() + "Packet%1").arg(numberOfNewPacketsCreatedHere++);
            theDomain->createObject(packetName, "RPacket");
            RPacket *theNewPacket = qobject_cast<RPacket *>(theDomain->getLastAddedObject());
            theNewPacket->setPacketContentsType(theAuxSupplyTypeList.at(i));
            theNewPacket->setDestinationList(theShortestPath);


            double materialsToBeProviveded;

            // If I have materials
            if (theFuelReserve >= theAuxAmountList.at(i)){

                materialsToBeProviveded = theAuxAmountList.at(i);
                theFuelReserve -= theAuxAmountList.at(i);

                theAuxAmountList[i] = 0.0;
            }

            // If I do not have the requested materials
            else {

                // subtract what was already supplied from this request
                theAuxAmountList[i] -= theFuelReserve;
                materialsToBeProviveded = theFuelReserve;
                theFuelReserve -= theFuelReserve;
            }


            theNewPacket->setPacketContents(materialsToBeProviveded);


            packetsCurrentlyOnboard << theNewPacket;
        } // for i



        unloadPackets(qobject_cast<RPacketResponse *>(theResponse), packetsCurrentlyOnboard);
    }



    QVector<int> indexesToRemove;
    for (int i = 0; i < theAuxDestinationList.size(); i++){

        int ii = theAuxDestinationList.size() - (i + 1);

        double sum = 0.0;

        sum += theAuxAmountList.at(ii);


        if (sum == 0.0){
            indexesToRemove << ii;
        }

    } // for i


    for (int i = 0; i < indexesToRemove.size(); i++){
        theAuxDestinationList.removeAt(indexesToRemove.at(i));
        theAuxAmountList.removeAt(indexesToRemove.at(i));
        theAuxCriterionList.removeAt(indexesToRemove.at(i));
        theAuxSupplyTypeList.removeAt(indexesToRemove.at(i));
    }




    // "Reserve vs time" plot
    if(lastEvaluationTime == 0.0){
        lastEvaluationTime = theCurrentTime->getCurrentValue();
    }

    // -------------------------------------------------------------------------------------------
    // OUTPUT
    // -------------------------------------------------------------------------------------------
    if (theOutputLevel >= RObject::Medium) {

        if (customPlot == 0) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            customPlot = new QCustomPlot();
            QString theString = QString("Reserve in %1").arg(this->objectName());
            mainWindow->addSubWindow(customPlot, theString, true);

            customPlot->addGraph();
            customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));

            customPlot->graph(0)->setPen(QColor(0, 0, 255, 255));
            customPlot->xAxis->setLabel("Time");
            customPlot->yAxis->setLabel(theString);

            customPlot->xAxis->setAutoTicks(true);
            customPlot->xAxis->setAutoTickLabels(false);
        }

        customPlot->graph(0)->addData(lastEvaluationTime, theFuelReserve);
        customPlot->rescaleAxes();
        customPlot->replot();
    }


    return 1;
}
