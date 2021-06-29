#ifndef RWaterDamModel_H
#define RWaterDamModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RWaterDamModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(int FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)

public:

    RWaterDamModel(QObject *parent, QString name);
    ~RWaterDamModel();

    int resetTime();

    int getFacilityType() const;
    void setFacilityType(int value);

    QString getWaterSupplierList();
    void setWaterSupplierList(QString value);

    int evaluateModel();

protected:

private:

    // Data members
    int theFacilityType;
    bool theAvailabilityOfWater;
    double theTimeUnderRepairs;

    QList<QPointer<RParameter> > theWaterSupplierList;
    RResponse *theResponse;

};

#endif
