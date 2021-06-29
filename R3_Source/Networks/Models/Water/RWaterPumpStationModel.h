#ifndef RWaterPumpStationModel_H
#define RWaterPumpStationModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RWaterPumpStationModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RFacilityType FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(int YearBuilt READ getYearBuilt WRITE setYearBuilt)
    Q_PROPERTY(double PumpingCapacity READ getPumpingCapacity WRITE setPumpingCapacity)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)

    Q_ENUMS(RFacilityType)
public:

    RWaterPumpStationModel(QObject *parent, QString name);
    ~RWaterPumpStationModel();

    int resetTime();

    enum RFacilityType{NoDamage, SmallAnchored, SmallUnanchored, LargeAnchored, LargeUnanchored};

    RFacilityType getFacilityType() const;
    void setFacilityType(RFacilityType value);

    int getYearBuilt() const;
    void setYearBuilt(int value);

    double getPumpingCapacity() const;
    void setPumpingCapacity(double value);

    QString getWaterSupplierList();
    void setWaterSupplierList(QString value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    int evaluateModel();

protected:
    // Data members
    QList<QPointer<RParameter> > thePowerSupplierList;

private:
    // Data members
    RFacilityType theFacilityType;
    bool theAvailabilityOfWater;
    bool theAvailabilityOfPower;
    int theYearBuilt;
    double thePumpingCapacity;
    double theTimeUnderRepairs;

    QList<QPointer<RParameter> > theWaterSupplierList;
    RResponse *theResponse;

};

#endif
