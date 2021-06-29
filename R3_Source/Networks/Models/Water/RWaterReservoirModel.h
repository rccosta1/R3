#ifndef RWaterReservoir_H
#define RWaterReservoir_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RWaterReservoirModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(double Capacity READ getCapacity WRITE setCapacity)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)

public:

    RWaterReservoirModel(QObject *parent, QString name);
     ~RWaterReservoirModel();

    int resetTime();

    double getCapacity() const;
    void setCapacity(double value);

    QString getWaterSupplierList();
    void setWaterSupplierList(QString value);

    int evaluateModel();

protected:

private:

    // Data members
    bool theAvailabilityOfWater;
    double theCapacity;
    double theCurrentVolume;

    QList<QPointer<RParameter>> theWaterSupplierList;
    RResponse *theResponse;

};

#endif
