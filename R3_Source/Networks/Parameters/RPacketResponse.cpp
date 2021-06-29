#include "RPacketResponse.h"
#include "RDomain.h"
#include "RPacket.h"
#include "RMainWindow.h"


RPacketResponse::RPacketResponse(QObject *parent, QString name)
	: RResponse(parent, name)
{
	RDomain *theDomain = qobject_cast<RDomain *>(parent);

    thePacket = 0;

	// Instantiate an RPacket
    theDomain->createObject(objectName() + "Packet", "RPacket");
    thePacket = qobject_cast<RPacket *>(theDomain->getLastAddedObject());
}

RPacketResponse::~RPacketResponse()
{
    if (thePacket) {
        delete thePacket;
	}
    thePacket = 0;
}


QStringList RPacketResponse::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}



QObject * RPacketResponse::getPacket() const
{
    return thePacket;
}

void RPacketResponse::setPacket(QObject *value)
{
    thePacket = qobject_cast<RPacket *>(value);
}

void RPacketResponse::setObjectName(const QString &name)
{
	QString oldName = objectName();
	QObject *object = qobject_cast<QObject *>(this);
	object->setObjectName(name);

    QString packetName = thePacket->objectName();
    if (packetName.contains(oldName)) {
        QString newpacketName = packetName.replace(oldName, name);
        thePacket->setObjectName(newpacketName);
	}
}
