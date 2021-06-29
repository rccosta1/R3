#ifndef RRefineryModel_H
#define RRefineryModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RGenericResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RRefineryModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RFacilityType FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(double StorageCapacity READ getStorageCapacity WRITE setStorageCapacity)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)
    Q_PROPERTY(QString FuelSupplierList READ getFuelSupplierList WRITE setFuelSupplierList)


    Q_ENUMS(RFacilityType)
public:

    RRefineryModel(QObject *parent, QString name);
    ~RRefineryModel();

    int resetTime();

    enum RFacilityType{NoDamage, SmallAnchored, SmallUnanchored, LargeAnchored, OnGround};

    RFacilityType getFacilityType() const;
    void setFacilityType(RFacilityType value);

    double getStorageCapacity() const;
    void setStorageCapacity(double value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    QString getFuelSupplierList();
    void setFuelSupplierList(QString value);

    void getRequests();

    int evaluateModel();

protected:


private:

    // Data members
    QList<QPointer<RParameter> > theFuelSupplierList;
    QList<QPointer<RParameter> > thePowerSupplierList;
    RFacilityType theFacilityType;

    QList<QPointer<RParameter> > theMaterialsSupplierList;
    QVector<QPointer<RModel>> theAuxDestinationList; // this is a copy of the destination list
    QVector<double> theAuxCriterionList; // this is a copy of the list of bids
    QVector<double> theAuxAmountList; // this is a copy of the list of requests
    QVector<QString> theAuxSupplyTypeList;

    double theTimeUnderRepairs;
    bool theAvailabilityOfFuel;
    bool theAvailabilityOfPower;
    double theStorageCapacity;
    double lastEvaluationTime;
    double theFuelReserve;

    RResponse *theResponse;
};

#endif

