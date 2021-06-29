#ifndef RPowerSubstationModel_H
#define RPowerSubstationModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RPowerSubstationModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RFacilityType FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(int YearBuilt READ getYearBuilt WRITE setYearBuilt)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)

    Q_ENUMS(RFacilityType)
public:

    RPowerSubstationModel(QObject *parent, QString name);
    ~RPowerSubstationModel();

    int resetTime();

    enum RFacilityType{LowVoltageAnchored, LowVoltageUnanchored, MediumVoltageAnchored, MediumVoltageUnanchored, HighVoltageAnchored, HighVoltageUnanchored};
    RFacilityType getFacilityType() const;
    void setFacilityType(RFacilityType value);

    int getYearBuilt() const;
    void setYearBuilt(int value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    int evaluateModel();

protected:


private:
    // Data members
    RFacilityType theFacilityType;
    double theYearBuilt;
    QList<QPointer<RParameter>> thePowerSupplierList;
    RResponse *theResponse;
    double theTimeUnderRepairs;

};

#endif
