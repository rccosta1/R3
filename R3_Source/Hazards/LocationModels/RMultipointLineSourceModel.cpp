#include "RMultipointLineSourceModel.h"	

#include "RLocation.h"
#include "RParameter.h"	
#include "RResponse.h"	
#include "RLocationResponse.h"
#include "RMainWindow.h"

#include <math.h>

RMultipointLineSourceModel::RMultipointLineSourceModel(QObject *parent, QString name)	
	: RModel(parent, name)
{	
	// Initializing the pointer data members
	theTheta = 0;
	theDepth = 0;

	theFaultLength = 0.0;
	
	// Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RLocationResponse");
    theEpicentreLocation = qobject_cast<RLocationResponse *>(theDomain->getLastAddedObject());
    theEpicentreLocation->setModel(this);

    // Create the  uniformly distributed random variable
    theDomain->createObject(objectName() + "_theta", "RContinuousRandomVariable");
    theTheta = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theTheta->setDistributionType(QString("Uniform (a, b)"));
    theTheta->setParameter1("0.0");
    theTheta->setParameter2("1.0");
    theAutoParametersList << theTheta;
}	

RMultipointLineSourceModel::~RMultipointLineSourceModel()	
{	
	
}	

QString RMultipointLineSourceModel::getPointLocationList()
{
    return parameterListToString(thePointLocationList);
}

void RMultipointLineSourceModel::setPointLocationList(QString value)
{
    thePointLocationList = stringToParameterList(value);
	
    // Checking if all the passed parameters are either RLocation or RLocationResponse
    parameterListToLocationList(thePointLocationList, true);
}

QObject * RMultipointLineSourceModel::getDepth() const	
{	
	return theDepth;
}	



void RMultipointLineSourceModel::setDepth(QObject *value)	
{	
	theDepth = qobject_cast<RParameter *>(value);
}	

QList<RParameter *> RMultipointLineSourceModel::getPhysicalParameterList()
{
	QList<RParameter *> paramList;
	paramList << theTheta << theDepth;
	return paramList;
}

void RMultipointLineSourceModel::reset()
{
	theFaultLength = 0;
	thePieceLengthVector.clear();
}

int RMultipointLineSourceModel::evaluateModel()	
{	
    if (thePointLocationList.count() < 2) {
        qCritical() << "Error in" << objectName() << ": The number of point Locations should be at least 2.";
		return -1;
	}

    // Converting the list of parameters to the list of Locations
    QList<RLocation *> theLocationList = parameterListToLocationList(thePointLocationList);
	
	if (theFaultLength == 0.0) {
        for (int i = 0; i < theLocationList.count() - 1; i++) {
            double pieceLength = theLocationList[i]->computeSurfaceDistance(theLocationList[i+1]);
			thePieceLengthVector << pieceLength;
			theFaultLength += pieceLength;
		}
	}

	double lengthRealization = theTheta->getCurrentValue() * theFaultLength;

    // Getting the pointer to the RLocation object from the RLocationResponse object (theEpicentreLocation)
    RLocation *theHypocentreLocationObject = qobject_cast<RLocation *>(theEpicentreLocation->getLocation());
    if (!theHypocentreLocationObject) {
        qCritical() << "Error: The Location response" << theEpicentreLocation->objectName() << "does not include any Location object.";
		return -1;
	}

	double theFaultLengthSoFar = 0;
	for (int i = 0; i < thePieceLengthVector.count(); i++) {
		double oldFaultLength = theFaultLengthSoFar;
		theFaultLengthSoFar += thePieceLengthVector[i];
		if ((theFaultLengthSoFar >= lengthRealization) && (lengthRealization > oldFaultLength)) {
			double lengthRatio = (lengthRealization - oldFaultLength) / thePieceLengthVector[i];
			
			// Implementation assuming that the earth is flat!
            /*double lng1 = theLocationList[i]->getLongitude();
            double lat1 = theLocationList[i]->getLatitude();
            double lng2 = theLocationList[i+1]->getLongitude();
            double lat2 = theLocationList[i+1]->getLatitude();
			double deltaLng = lng2 - lng1;
			double deltaLat = lat2 - lat1;
            double LocationLat = lat1 + deltaLat * lengthRatio;
            double LocationLng = lng1 + deltaLng * lengthRatio;
            theHypocentreLocationObject->setLatitude(LocationLat);
            theHypocentreLocationObject->setLongitude(LocationLng);*/

			// Implementation assuming that the earth is spherical
            theLocationList[i]->computeIntermediatePoint(theLocationList[i+1], theHypocentreLocationObject, lengthRatio);

			break;
		}
	}

    double LocationAlt = -1.0 * theDepth->getCurrentValue();
    theHypocentreLocationObject->setAltitude(LocationAlt);
	

    if (theOutputLevel >= RObject::Medium) {
        qDebug() << qPrintable(QString("%1\t%2").arg(theHypocentreLocationObject->getLongitude(), 0, 'g', 12).arg(theHypocentreLocationObject->getLatitude(), 0, 'g', 12));
	}

    theHypocentreLocationObject->redisplayonMap();

	return 0;
}	

QStringList RMultipointLineSourceModel::getActionList()
{
	QStringList actionList;
	actionList << "&Display on Map" << "&Remove from Map";
	return actionList;
}

void RMultipointLineSourceModel::displayonMap()
{
    /*
    QList<RLocation *> LocationList = parameterListToLocationList(thePointLocationList);
    if (LocationList.count() < 2) {
        qCritical() << "Warning: Please enter at least 2 Location objects in the model" << objectName() << ".";
		return;
	}
	RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
	RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
	theMainWindow->setActiveSubWindow(theMapWidget);
	if (!theMapWidget) {
        LocationList[0]->openMap();
		return;
	}
    theMapWidget->drawPolyline(this, LocationList, Qt::black);
    */
}

void RMultipointLineSourceModel::removefromMap()
{
    /*
	RMainWindow *theMainWindow = qobject_cast<RMainWindow *>(theDomain->parent());
	RMapWidget *theMapWidget = theMainWindow->findChild<RMapWidget *>();
	if (theMapWidget) {
		theMainWindow->setActiveSubWindow(theMapWidget);
		theMapWidget->removePolyline(this);
	}
    */
}
