#include "RNetworkModel.h"
#include "RParameter.h"
#include "RModel.h"
#include "RPacket.h"
#include "RIntensityModel.h"
#include "RPGDIntensityModel.h"
#include "RNormalDistribution.h"
#include "RMainWindow.h"
#include "REarthquakeBaseClass.h"
#include <math.h>

RNetworkModel::RNetworkModel(QObject *parent, QString name)
    : RModel(parent, name)
{ 
    thePathMatrixNeed = true;
    timeOfEvent = 0.0;
    theLocation = 0;
    functionalityPlot = 0;
    numberOfNewPacketsCreatedHere = 0;
    theFunctionality = 1.0;
    theSoilSusceptibilityLevel = QString("Low");

    thePGAIntensity = 0.0;
    theSaIntensity  = 0.0;
    thePGVIntensity = 0.0;
    thePGDIntensity = 0.0;

    myProbabilityOfLiquefaction = 0.0;
    theThresholdfunctionality = 0.8;

    replacementCost = 0.0;
    directLoss = 0.0;
    theExpectedRepairTime = 0.0;

    customPlot = 0;

    thePGACounter = 0;
}

RNetworkModel::~RNetworkModel()
{
}



QObject * RNetworkModel::getLocation() const
{
    return theLocation;
}


void RNetworkModel::setLocation(QObject *value)
{
    theLocation = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theLocation);
}


int RNetworkModel::getPacketsCurrentlyOnboard()
{
    return packetsCurrentlyOnboard.size();
}


void RNetworkModel::setTravelPaths(QVector<QVector<RModel *> > pathMatrix)
{
    theTravelPaths = pathMatrix;
}



int RNetworkModel::loadPackets(QList<QPointer<RParameter> > theInputParameterList)
{
    // Initial declaration of items used in this method
    RPacket *stationaryPacket;
    QList<RPacket *> subPacketList;
    QString stringWithListOfPackets;
    QString remainingPackets;


    // Loop over list of input packets (those are either pure packets or packet-responses)
    for (int i = 0; i < theInputParameterList.size(); i++) {

        // Clear and prepare the list of packets that will remain in the input packet
        remainingPackets = "";

        // Get the  packet from the input list; notice that it sits inside a response object,
        // stationary, never to be moved; it is the subpackets that are moved
        stationaryPacket = parameterToPacket(theInputParameterList[i]);

        // Get a list of packets inside the stationary packet
        stringWithListOfPackets = stationaryPacket->getPacketList();
        subPacketList = parameterListToPacketList(stringToParameterList(stringWithListOfPackets));

        //qCritical() << this->objectName() << subPacketList;

        // Loop over the list of those packets
        for (int j = 0; j<subPacketList.size(); j++) {

            // Check whether the packet is supposed to be picked up by this object
            if (subPacketList[j]->getNextTravelStep() == this) {

                // Take the packet and remove it from the input packet list, i.e., do not add it to remainingPackets
                packetsCurrentlyOnboard << subPacketList[j];


                // Display output
                if (theOutputLevel == RDomain::Maximum) {
                    // qDebug() << "     "<< this->objectName() << "is taking on" << subPacketList[j]->objectName() << "at" << theCurrentTime->getTime();
                }
            }
            else {

                // No match, so keep the packet in the input packet list
                remainingPackets.append(subPacketList[j]->objectName() + "; ");
            }
        }

        // Set the list of remaining packets in the stationary packet; could be none if all were loaded
        stationaryPacket->setPacketList(remainingPackets);

    }

    if (theOutputLevel == RDomain::Maximum) {
        // qCritical() << this->objectName() << "loaded" << packetsCurrentlyOnboard;
    }

    return 0;
}


int RNetworkModel::unloadPackets(RPacketResponse *theResponse, QList<QPointer<RPacket> > packets)
{
    if (theFunctionality > theThresholdfunctionality){

        // Make sure to tell the responses that this model has now been analyzed, otherwise it may be re-analyzed (core Rt issue...)
        theResponse->setAnalyzed(true);


        // Get the pointer to the packet that sits inside the response object
        RPacket *thePacketInsideTheResponse = parameterToPacket(theResponse);
        if (!thePacketInsideTheResponse) {
            qCritical() << "Error:" << theResponse->objectName() << "does not have any Packet object.";
            return -1;
        }


        // Loop through packets that should be unloaded INTO THE RESPONSE
        // The decision to unload or not is dependent on each packet currently on board

        for (int i = 0; i < packetsCurrentlyOnboard.size(); i++) {
            //int ii = packetsCurrentlyOnboard.size() - (i + 1);

                // Update the next travel destination, in case something happened since last time
                packetsCurrentlyOnboard.at(i)->setNextTravelStep(this);

                // Set packet list in the response packet
                thePacketInsideTheResponse->setPacketList(thePacketInsideTheResponse->getPacketList() + ";" + packetsCurrentlyOnboard[i]->objectName());

                // Display output
                if (theOutputLevel == RDomain::Maximum) {
                    //qCritical() << "     " << this->objectName() << "is unloading" << packetsCurrentlyOnboard[i]->objectName() << "at" << theCurrentTime->getTime();
                }

                //packetsCurrentlyOnboard.removeAt(packetsCurrentlyOnboard.indexOf(packetsCurrentlyOnboard.at(i)));
        } // for i

        packetsCurrentlyOnboard.clear();
    } // if functional
    return 0;
}


void RNetworkModel::handlePacketsThatReachedFinalDestination()
{
    // Delete packets that got here but could not be loaded
    QList<QPointer<RPacket> > tempPacketList;
    for (int i = 0; i < packetsCurrentlyOnboard.size(); i++) {
        tempPacketList << packetsCurrentlyOnboard.at(i);
    }

    // Delete de packets but first inform the model that made the request that a request is no longer coming
    for (int i = 0; i < packetsCurrentlyOnboard.size(); i++) {
       theDomain->deleteObject(tempPacketList.at(i)->objectName());
    }

    packetsCurrentlyOnboard.clear();
}



QString RNetworkModel::getIntensityList()
{
    return parameterListToString(theIntensityList);
}

void RNetworkModel::setIntensityList(QString value)
{
    theIntensityList = stringToParameterList(value);
}


bool RNetworkModel::getFunctionality()
{
    if (theFunctionality > theThresholdfunctionality){
    return true;

    } else {
        return false;
    }
}

double RNetworkModel::getExpectedRepairTime()
{
   return theExpectedRepairTime;
}


// Supplier methods
bool RNetworkModel::getPathMatrixNeed()
{
    return thePathMatrixNeed;
}


int RNetworkModel::getShortestPath(RModel *theDestinationModel)
{
    // I am saving a list of shortest paths associated with each sink to save computational time
    // If the shortest path for this sink is known, just recover it!
    if (theListOfKnownSinks.contains(theDestinationModel)){

        bool checkPathCondition = true;

        // If confused, see Description (1) at the bottom of this file for clarifications
        theShortestPath = theTravelPaths.at(theListOfKnownPaths.at(theListOfKnownSinks.indexOf(theDestinationModel)));

        // Checking if all models in the shortest path in memory are still functional, if not, a new shortest path needs to be defined
        for (int i = 0; i < theShortestPath.size(); i++){
            RNetworkModel *themodel = qobject_cast<RNetworkModel *>(theShortestPath.at(i));
            if (themodel && themodel->getFunctionality() > theThresholdfunctionality){
                checkPathCondition = false;
                // qCritical() << "The getShortestPath method needs to be recalled because" << themodel->objectName() << "is broken.";
            }
        }

        if (!checkPathCondition){
            theListOfKnownPaths.removeAt(theListOfKnownSinks.indexOf(theDestinationModel));
            theListOfKnownSinks.removeAt(theListOfKnownSinks.indexOf(theDestinationModel));
            int call = getShortestPath(theDestinationModel);
        }
        return 1;
    }

    // If there is no known path between this model and the sink, I need to find one
    else {

        // Checking which paths contain that contain this model also contain the sink model
        QVector<int> theListOfIndexes;

        if (myPossiblePaths.size() > 0){


            for (int i = 0; i < myPossiblePaths.size(); i++){
                // If this path that contains this model also contains the sink model
                if (theTravelPaths.at(myPossiblePaths.at(i)).contains(theDestinationModel)){
                    bool checkPathCondition = true;

                    // Check if all models in this path are available
                    for (int j = 0; j < theTravelPaths.at(myPossiblePaths.at(i)).size(); j++){
                        RNetworkModel *themodel = qobject_cast<RNetworkModel *>(theTravelPaths.at(myPossiblePaths.at(i)).at(j));

                        // If all models are available, add this as a candidate to be the shortest path
                        if (themodel && themodel->getFunctionality() < theThresholdfunctionality){
                            checkPathCondition = false;
                            // qCritical() << themodel->objectName() << "is unavailable";
                        }
                    }

                    if (checkPathCondition){
                        theListOfIndexes << myPossiblePaths.at(i);
                    }
                }
            }

            if (theListOfIndexes.size() > 0){
                int shortestPathIndex = theListOfIndexes.at(0);

                // Start with the first path as a candidate
                theShortestPath = theTravelPaths.at(theListOfIndexes.at(0));
                for (int i = 0; i < theListOfIndexes.size(); i++){

                    // If any other path is shorter, that path becomes the shortest path
                    if (theShortestPath.size() > theTravelPaths.at(theListOfIndexes.at(i)).size()){
                        theShortestPath = theTravelPaths.at(theListOfIndexes.at(i));
                        shortestPathIndex = theListOfIndexes.at(i);
                    }
                }

                // Removing hazard models from the shortest path as it is a list of "physical" members
                removeNonNetworkModels();

                // Adding this source and sink to the list of know shortest paths
                if (!theListOfKnownSinks.contains(theDestinationModel)){
                    theListOfKnownSinks << theDestinationModel;
                    theListOfKnownPaths << shortestPathIndex;
                }

                return 1;
            }

            else{
                // qCritical() << "No available path connects" << this->objectName() << "to" << theDestinationModel->objectName();
                return 0;
            }
        }

        else {
            // qCritical() << "No available path contain this model" << this->objectName();
            return 0;
        }
    }
}


void RNetworkModel::getPossiblePaths()
{
    myPossiblePaths.clear();
    theListOfKnownPaths.clear();
    theListOfKnownSinks.clear();

    for (int i = 0; i < theTravelPaths.size(); i++){

        // Finding the paths that contains the source
        if(theTravelPaths.at(i).contains(this)){
            myPossiblePaths << i;
        }
    }
}


void RNetworkModel::removeNonNetworkModels()
{
    QVector<int> modelsToBeRemoved;
    for (int i = 0; i < theShortestPath.size(); i++)
    {
        RNetworkModel *theCheck;
        theCheck = qobject_cast<RNetworkModel *>(theShortestPath.at(i));
        if (!theCheck){
            modelsToBeRemoved << i;
        }
    }

    for (int i = 0; i < modelsToBeRemoved.size(); i++){
        int index = modelsToBeRemoved.size() - (i+1);
        theShortestPath.removeAt(modelsToBeRemoved.at(index));
    }
}


void RNetworkModel::collectRequests(QString value)
{
    QList<RRequest *> theAllRequestList = theDomain->findChildren<RRequest *>();
    QList<RRequest *> theListOfCollectedRequests;


    for (int i = 0; i < theAllRequestList.size(); i++){

        // Collecting the requests that need construction materials
        if(theAllRequestList.at(i)->getContentType().contains(value) && theAllRequestList.at(i)->getRequestedAmountList().size() > 0){


            // If this request is for money and this model can supply it
            if (this->getShortestPath(theAllRequestList.at(i)->getModel()) != 0){

                // Add this request to the list of requested taken
                theListOfCollectedRequests << theAllRequestList.at(i);

                //qCritical() << this->objectName() << "is taking this request" << theAllRequestList.at(i)->objectName();

                QVector<double> theRequestedAmounts = theAllRequestList.at(i)->getRequestedAmountList();
                QVector<int> theIDs = theAllRequestList.at(i)->getRequesterIDList();


                // Add the information from this request to the list of requests picked by this object
                theRequestDestinationList << theAllRequestList.at(i)->getModel();
                theRequestedSupplyTypeList << theAllRequestList.at(i)->getContentType();
                theRequestedAmountList << theRequestedAmounts;
                theRequesterIDList << theIDs;
            }
        }
    }
}


QVector<double> RNetworkModel::calculateIntensities()
{
    thePGAIntensity = 0.0;
    theSaIntensity = 0.0;
    thePGVIntensity = 0.0;
    thePGDIntensity = 0.0;

    RResponse *theResponseObject;
    RIntensityModel *theIntensityModel;
    QVector<double> theIntensities;


    if (theIntensityList.size() > 0){
        for (int i = 0; i < theIntensityList.count(); i++) {

            // Get hold of the intensity model
            theResponseObject = qobject_cast<RResponse *>(theIntensityList[i]);
            theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());
            theIntensityModel->setLocation(theLocation);


            REarthquakeBaseClass *theGroundAccelerationModel = qobject_cast<REarthquakeBaseClass *>(theResponseObject->getModel());
            RPGDIntensityModel *theGroundDisplacementModel = qobject_cast<RPGDIntensityModel *>(theResponseObject->getModel());

            // Bridges use Sa to calculate damage
//            RBridgeModel *theBridge = qobject_cast<RBridgeModel *>(this);

//            if (theGroundAccelerationModel && theBridge){

//                theIntensityModel->setIntensityMeasure(QString("Sa"));
//                theIntensityModel->evaluateModel();
//                thePGAIntensity = theResponseObject->getCurrentValue();
//                theIntensities << thePGAIntensity;
//            }

            // If the intensity model is Boore Atikinson 2008
            if (theGroundAccelerationModel) {

                theIntensityModel->setIntensityMeasure(QString("PGA"));
                theIntensityModel->evaluateModel();
                thePGAIntensity = theResponseObject->getCurrentValue();
                theIntensities << thePGAIntensity;

                theIntensityModel->setIntensityMeasure(QString("PGV"));
                theIntensityModel->evaluateModel();
                thePGVIntensity = theResponseObject->getCurrentValue();
                theIntensities << thePGVIntensity;
            }

            else if (theGroundDisplacementModel) {
                // Ad hoc solution - forcing ports to be more susceptible to liquefaction
                // roughly checking if the network model is in Richmond
                //if (theLocation->getLongitude() > -123.206929 && theLocation->getLatitude() < 49.204182) {
                //    theSoilSusceptibilityLevel = "VeryHigh";
                //}


                RPGDIntensityModel *thePGDIntensityModel = qobject_cast<RPGDIntensityModel *>(theIntensityModel);
                thePGDIntensityModel->setIntensityMeasure(QString("PGD"));

                // Set the soil susceptibility
                thePGDIntensityModel->setSoilSusceptibilityLevel(theSoilSusceptibilityLevel);

                // Evaluate the intensity model
                thePGDIntensityModel->evaluateModel();

                // Calculate probability of liquefaction - needed for the line models to calculate damage
                myProbabilityOfLiquefaction = thePGDIntensityModel->getProbabilityOfLiquefaction();

                // Calculate the lateral spreading associated with liquefaction - needed for the line models to calculate damage
                lateralPGDIntensity = thePGDIntensityModel->getLateralPGD();

                // Get the PGD
                thePGDIntensity = theResponseObject->getCurrentValue();

                // Add the PGD to the vector of intensities that will be returned
                theIntensities << thePGDIntensity;
            }

        }
    }

    else {
        theIntensities << 0.0 << 0.0 << 0.0 << 0.0;
    }



    // If any intensity is non-zero then an earthquake just happened; set that time
    if (thePGAIntensity != 0.0 || thePGDIntensity != 0.0 || thePGVIntensity != 0.0) {
        timeOfEvent = theCurrentTime->getCurrentValue();
        // qCritical() << this->objectName() << "PGA =" << thePGAIntensity << "PGD =" << thePGDIntensity << "Lateral PGD=" << lateralPGDIntensity << "PGV" << thePGVIntensity;
    }


    return theIntensities;
}


void RNetworkModel::getRequests()
{

}

int RNetworkModel::isAKnowDestination(RModel* theModel)
{
    int index = -1;
    for (int i = 0; i < packetsCurrentlyOnboard.size(); i++){
        QVector<RModel *> theDestinationList = packetsCurrentlyOnboard.at(i)->getDestinationList();

        //qCritical() << theModel << theDestinationList.at(theDestinationList.size() - 1);
        if (theModel == theDestinationList.at(theDestinationList.size() - 1)){
            index = i;
        }
    }

    return index;
}



