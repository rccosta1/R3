#ifndef RWaterTreatmentFacilityModel_H
#define RWaterTreatmentFacilityModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RWaterTreatmentFacilityModel : public RNetworkModel
{
	Q_OBJECT
    Q_PROPERTY(RFacilityType FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(int YearBuilt READ getYearBuilt WRITE setYearBuilt)
    Q_PROPERTY(double Capacity READ getCapacity WRITE setCapacity)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)

    Q_ENUMS(RFacilityType)
public:

    RWaterTreatmentFacilityModel(QObject *parent, QString name);
     ~RWaterTreatmentFacilityModel();

    int resetTime();

    enum RFacilityType{PWT1, PWT2, PWT3, PWT4, PWT5, PWT6};

    RFacilityType getFacilityType() const;
    void setFacilityType(RFacilityType value);

    int getYearBuilt() const;
    void setYearBuilt(int value);

    double getCapacity() const;
    void setCapacity(double value);

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
    double theCapacity;
    double theTimeUnderRepairs;

    QList<QPointer<RParameter> > theWaterSupplierList;
    RResponse *theResponse;

};

#endif
