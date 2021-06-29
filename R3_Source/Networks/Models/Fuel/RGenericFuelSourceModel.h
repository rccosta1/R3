#ifndef RGenericFuelSourceModel_H
#define RGenericFuelSourceModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RGenericFuelSourceModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RFacilityType FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(double StorageCapacity READ getStorageCapacity WRITE setStorageCapacity)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)
    Q_PROPERTY(QString FuelSupplierList READ getFuelSupplierList WRITE setFuelSupplierList)

    Q_ENUMS(RFacilityType)
public:

    RGenericFuelSourceModel(QObject *parent, QString name);
    ~RGenericFuelSourceModel();

    int resetTime();


    enum RFacilityType{NoDamage, OnGroundConcreteAnchored, OnGroundConcreteUnanchored, OnGroundSteelAnchored, OnGroundSteelUnanchored, AboveGroundSteel, AboveGroundWood, BuriedConcrete};

    RFacilityType getFacilityType() const;
    void setFacilityType(RFacilityType value);

    double getStorageCapacity() const;
    void setStorageCapacity(double value);

    QString getFuelSupplierList();
    void setFuelSupplierList(QString value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);


    int evaluateModel();

protected:


private:
    // Data members
    QList<QPointer<RParameter> > theFuelSupplierList;
    QList<QPointer<RParameter> > thePowerSupplierList;
    RFacilityType theFacilityType;

    bool theAvailabilityOfFuel;
    bool theAvailabilityOfPower;
    double theStorageCapacity;
    double theFuelReserve;
    double theTimeUnderRepairs;

    RResponse *theResponse;
};

#endif

