#ifndef RPacketResponse_H
#define RPacketResponse_H

#include "RResponse.h"

class RPacket;

class RPacketResponse : public RResponse
{
	Q_OBJECT

    Q_PROPERTY(QObject *Packet READ getPacket WRITE setPacket)

public:
	RPacketResponse(QObject *parent, QString name);
	~RPacketResponse();
	
    QObject *getPacket() const;
    void setPacket(QObject *value);

	// Specific implementation of the setObjectName, which sets the object name for the response and its auto-generated RPacket object
	void setObjectName(const QString &name);

    QStringList getActionList();


private:

    RPacket *thePacket;
	
};

#endif
