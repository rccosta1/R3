#ifndef RPortModel_H
#define RPortModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "QVector"
#include "qcustomplot.h"
#include "RFragilityFunctions.h"

class RParameter;
class RResponse;

class RPortModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(QString FuelSupplierList READ getFuelSupplierList WRITE setFuelSupplierList)
    Q_PROPERTY(RWFStructureType WFStructureType READ getWFStructureType WRITE setWFStructureType)
    Q_PROPERTY(RCranesType CranesType READ getCranesType WRITE setCranesType)
    Q_PROPERTY(RFuelFacilityType FuelFacilityType READ getFuelFacilityType WRITE setFuelFacilityType)

    Q_ENUMS(RWFStructureType)
    Q_ENUMS(RFuelFacilityType)
    Q_ENUMS(RCranesType)

public:

    RPortModel(QObject *parent, QString name);
    ~RPortModel();

    int resetTime();

    enum RWFStructureType{WFNoDamage, WFStructure};
    enum RFuelFacilityType{FFNoDamage, AnchoredBackUpPower, AnchoredNoBackUpPower, UnanchoredBackUpPower, UnanchoredNoBackUpPower, Buried};
    enum RCranesType{CrNoDamage, Anchored, Unanchored};

    QString getFuelSupplierList();
    void setFuelSupplierList(QString value);

    RWFStructureType getWFStructureType() const;
    void setWFStructureType(RWFStructureType value);

    RCranesType getCranesType() const;
    void setCranesType(RCranesType value);

    RFuelFacilityType getFuelFacilityType() const;
    void setFuelFacilityType(RFuelFacilityType value);

    int evaluateModel();

protected:

    // Waterfront Structures
    QVector<double> WFPGADamageMedians;
    QVector<double> WFPGADamageDispersions;
    QVector<double> WFPGDDamageMedians;
    QVector<double> WFPGDDamageDispersions;
    QVector<double> WFRestorationMedians;
    QVector<double> WFRestorationStandardDeviations;

    // Cranes
    QVector<double> cranesPGADamageMedians;
    QVector<double> cranesPGADamageDispersions;
    QVector<double> cranesPGDDamageMedians;
    QVector<double> cranesPGDDamageDispersions;
    QVector<double> cranesRestorationMedians;
    QVector<double> cranesRestorationStandardDeviations;

    // Fuel Facilities
    QVector<double> FFPGADamageMedians;
    QVector<double> FFPGADamageDispersions;
    QVector<double> FFPGDDamageMedians;
    QVector<double> FFPGDDamageDispersions;
    QVector<double> FFRestorationMedians;
    QVector<double> FFRestorationStandardDeviations;


private:

    QList<QPointer<RParameter> > theFuelSupplierList;
    QVector<double> theWFDamageProbabilities;
    QVector<double> theCranesDamageProbabilities;
    QVector<double> theFFDamageProbabilities;
    RWFStructureType theWFStructureType;
    RCranesType theCranesType;
    RFuelFacilityType theFuelFacilityType;
    double theTimeUnderRepairs;

    RResponse *theResponse;
};

#endif
