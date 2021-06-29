#ifndef RPacket_H
#define RPacket_H

#include "RParameter.h"
#include "RModel.h"
#include <math.h>
#include <QPointer>

class RPacket : public RParameter
{
    Q_OBJECT

    Q_PROPERTY(QString PacketList READ getPacketList WRITE setPacketList)
    Q_PROPERTY(double PacketCapacity READ getPacketCapacity WRITE setPacketCapacity)
    Q_PROPERTY(double PacketVelocity READ getPacketVelocity WRITE setPacketVelocity)
    //Q_PROPERTY(double PacketContents READ getPacketContents WRITE setPacketContents)

public:
    RPacket(QObject *parent, QString name);
    virtual ~RPacket();

    QString getPacketList();
    void setPacketList(QString value);

    // Give the packet the whole path it needs to go through
    QVector<RModel *> getDestinationList();
    void setDestinationList(QVector<RModel *> theList);
    void setDestinationList(RModel *theModel);

    // Set the next model the packet needs to visit
    void setNextTravelStep(RModel *currentModel);
    RModel *getNextTravelStep();

    QVector<double> getPacketContents() const;
    void setPacketContents(double packetContents);

    void setPacketContentsType(QString theType);
    QString getPacketContentsType();

    QVector<int> getPacketOwner() const;
    void setPacketOwner(int packetOwnerID);

    double getPacketCapacity();
    void setPacketCapacity(double value);

    double getPacketVelocity();
    void setPacketVelocity(double value);



    QStringList getActionList();

protected:
    QList<QPointer<RParameter> > thePacketList;
    QVector<double> thePacketContents;
    QString thePacketContentsType;
    QVector<int> thePacketOwnerID;

private:

    // Data members
    QPointer<RModel> theNextTravelStep;
    RModel *theFinalDestination;
    QVector<RModel *> myDestinationList;
    double thePacketCapacity;
    double thePacketTravelVelocity;
    bool setNextModel;
};

#endif
