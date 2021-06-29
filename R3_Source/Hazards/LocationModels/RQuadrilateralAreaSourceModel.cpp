#include "RQuadrilateralAreaSourceModel.h"	
	
#include "RParameter.h"	
#include "RLocationResponse.h"
#include "RContinuousRandomVariable.h"
#include "RLocation.h"

#include <math.h>

RQuadrilateralAreaSourceModel::RQuadrilateralAreaSourceModel(QObject *parent, QString name)	
	: RModel(parent, name)
{	
	// Initializing the pointer data members
    thePoint1Location = 0;
    thePoint2Location = 0;
    thePoint3Location = 0;
	theTheta2 = 0;
	theTheta1 = 0;
	theDepth = 0;
	
	// Instantiating the response object
    theDomain->createObject(objectName() + "Response", "RLocationResponse");
    theEpicentreLocation = qobject_cast<RLocationResponse *>(theDomain->getLastAddedObject());
    theEpicentreLocation->setModel(this);
}	

RQuadrilateralAreaSourceModel::~RQuadrilateralAreaSourceModel()	
{	
	
}	

QObject * RQuadrilateralAreaSourceModel::getPoint1Location() const
{	
    return thePoint1Location;
}	

void RQuadrilateralAreaSourceModel::setPoint1Location(QObject *value)
{	
    thePoint1Location = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(thePoint1Location);
}	

QObject * RQuadrilateralAreaSourceModel::getPoint2Location() const
{	
    return thePoint2Location;
}	

void RQuadrilateralAreaSourceModel::setPoint2Location(QObject *value)
{	
    thePoint2Location = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(thePoint2Location);
}	

QObject * RQuadrilateralAreaSourceModel::getPoint3Location() const
{	
    return thePoint3Location;
}	

void RQuadrilateralAreaSourceModel::setPoint3Location(QObject *value)
{	
    thePoint3Location = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(thePoint3Location);
}	

QObject * RQuadrilateralAreaSourceModel::getTheta2() const	
{	
	return theTheta2;
}	

void RQuadrilateralAreaSourceModel::setTheta2(QObject *value)	
{	
	theTheta2 = qobject_cast<RParameter *>(value);
}	

QObject * RQuadrilateralAreaSourceModel::getTheta1() const	
{	
	return theTheta1;
}	

void RQuadrilateralAreaSourceModel::setTheta1(QObject *value)	
{	
	theTheta1 = qobject_cast<RParameter *>(value);
}	

QObject * RQuadrilateralAreaSourceModel::getDepth() const	
{	
	return theDepth;
}	

void RQuadrilateralAreaSourceModel::setDepth(QObject *value)	
{	
	theDepth = qobject_cast<RParameter *>(value);
}

QList<RParameter *> RQuadrilateralAreaSourceModel::getPhysicalParameterList()
{
	QList<RParameter *> paramList;
	paramList << theTheta1 << theTheta2 << theDepth;
	return paramList;
}

int RQuadrilateralAreaSourceModel::evaluateModel()	
{	
    RLocation *thePoint1LocationObject = parameterToLocation(thePoint1Location);
    RLocation *thePoint2LocationObject = parameterToLocation(thePoint2Location);
    RLocation *thePoint3LocationObject = parameterToLocation(thePoint3Location);
	
	// Get information about the area source geometry
    double lng1 = thePoint1LocationObject->getLongitude();
    double lat1 = thePoint1LocationObject->getLatitude();
    double lng2 = thePoint2LocationObject->getLongitude();
    double lat2 = thePoint2LocationObject->getLatitude();
    double lng3 = thePoint3LocationObject->getLongitude();
    double lat3 = thePoint3LocationObject->getLatitude();

    // Get information about the uncertainty in the hypocentre Location
	double rv1 = theTheta1->getCurrentValue();
	double rv2 = theTheta2->getCurrentValue();
		
    // Getting the pointer to the RLocation object from the RLocationResponse object (theEpicentreLocation)
    RLocation *theEpicentreLocationObject = parameterToLocation(theEpicentreLocation);
    if (!theEpicentreLocationObject) {
        qCritical() << "Error: The Location response" << theEpicentreLocation->objectName() << "does not include any Location object.";
		return -1;
	}

	double lng = lng1 + (lng2 - lng1) * rv1 + (lng3 - lng1) * rv2;// + (lng4 - lng1 + (lng3 - lng1 + lng2 - lng1)) * (rv1 + rv2) / 2.0;
	double lat = lat1 + (lat2 - lat1) * rv1 + (lat3 - lat1) * rv2;// + (lat4 - lat1 + (lat3 - lat1 + lat2 - lat1)) * (rv1 + rv2) / 2.0;
	
    theEpicentreLocationObject->setLongitude(lng);
    theEpicentreLocationObject->setLatitude(lat);
    theEpicentreLocationObject->setAltitude(-1.0 * theDepth->getCurrentValue());

    theEpicentreLocationObject->redisplayonMap();

	return 0;
}	

