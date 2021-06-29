#ifndef RPowerLine_H
#define RPowerLine_H

#include "RModel.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RPowerLineModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *FromLocation READ getFromLocation WRITE setFromLocation)
    Q_PROPERTY(QObject *ToLocation READ getToLocation WRITE setToLocation)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)

public:
    RPowerLineModel(QObject *parent, QString name);
    ~RPowerLineModel();

    int resetTime();

    QObject *getFromLocation() const;
    void setFromLocation(QObject *value);

    QObject *getToLocation() const;
    void setToLocation(QObject *value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    int evaluateModel();

protected:


private:

    // Data members
    QPointer<RParameter> theFromLocation;
    QPointer<RParameter> theToLocation;
    bool theAvailabilityOfPower;
    QList<QPointer<RParameter> > thePowerSupplierList;
    RResponse *theResponse;

};

#endif
