#include "RLocationResponse.h"
#include "RDomain.h"
#include "RLocation.h"
#include "RMainWindow.h"


RLocationResponse::RLocationResponse(QObject *parent, QString name)
	: RResponse(parent, name)
{
	RDomain *theDomain = qobject_cast<RDomain *>(parent);

    // Instantiate an RLocation
    theDomain->createObject(objectName() + "Location", "RLocation");
    theLocation = qobject_cast<RLocation *>(theDomain->getLastAddedObject());
}

RLocationResponse::~RLocationResponse()
{
	if (theLocation) {
		delete theLocation;
	}
	theLocation = 0;
}



QStringList RLocationResponse::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}



QObject * RLocationResponse::getLocation() const
{
	return theLocation;
}

void RLocationResponse::setLocation(QObject *value)
{
    theLocation = qobject_cast<RLocation *>(value);
}

void RLocationResponse::setObjectName(const QString &name)
{
	QString oldName = objectName();
	QObject *object = qobject_cast<QObject *>(this);
	object->setObjectName(name);

	QString locationName = theLocation->objectName();
	if (locationName.contains(oldName)) {
		QString newLocationName = locationName.replace(oldName, name);
		theLocation->setObjectName(newLocationName);
	}
}

void RLocationResponse::backup()
{
	backupVector.clear();
	backupVector << theLocation->getLatitude();
	backupVector << theLocation->getLongitude();
	backupVector << theLocation->getAltitude();
}

void RLocationResponse::restore()
{
	if (backupVector.count() != 3) {
		qCritical() << "Error: There is no backup vector for the response" << objectName() << ".";
		return;
	}

	theLocation->setLatitude(backupVector[0]);
	theLocation->setLongitude(backupVector[1]);
	theLocation->setAltitude(backupVector[2]);
}

QVector<double> RLocationResponse::getValueVector()
{
	QVector<double> resultVector;
	resultVector << theLocation->getLatitude();
	resultVector << theLocation->getLongitude();
	resultVector << theLocation->getAltitude();
	return resultVector;
}

QVector<double> RLocationResponse::getBackupVector()
{
	return backupVector;
}

void RLocationResponse::setValueVector(QVector<double> valueVector)
{
	if (valueVector.count() != 3) {
		qCritical() << "Error: The value vector passed to the response" << objectName() << "does not include enough elements.";
		return;
	}
	theLocation->setLatitude(valueVector[0]);
	theLocation->setLongitude(valueVector[1]);
	theLocation->setAltitude(valueVector[2]);
}
