#ifndef RShelterModel_H
#define RShelterModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RShelterModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(int NumberOfBedsAvailable READ getNumberOfBedsAvailable WRITE setNumberOfBedsAvailable)
    Q_PROPERTY(int NumberOfBedsThatCanBeCreated READ getNumberOfBedsThatCanBeCreated WRITE setNumberOfBedsThatCanBeCreated)
    Q_PROPERTY(int NumberOfStoreys READ getNumberOfStoreys WRITE setNumberOfStoreys)
    Q_PROPERTY(int FootPrintArea READ getFootPrintArea WRITE setFootPrintArea)
    Q_PROPERTY(QString BuildingType READ getBuildingType WRITE setBuildingType)
    Q_PROPERTY(QString OccupancyClass READ getOccupancyClass WRITE setOccupancyClass)
    Q_PROPERTY(QString DesignLevel READ getDesignLevel WRITE setDesignLevel)
    Q_PROPERTY(QObject *StructuralDamage READ getStructuralDamage WRITE setStructuralDamage)
    Q_PROPERTY(QObject *DriftDamage READ getDriftDamage WRITE setDriftDamage)
    Q_PROPERTY(QObject *AccelerationDamage READ getAccelerationDamage WRITE setAccelerationDamage)
    Q_PROPERTY(QObject *DirectEconomicLoss READ getDirectEconomicLoss WRITE setDirectEconomicLoss)
    Q_PROPERTY(QObject *RepairTime READ getRepairTime WRITE setRepairTime)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)
    Q_PROPERTY(QString HouseholdsList READ getHouseholdsList WRITE setHouseholdsList)


public:

    RShelterModel(QObject *parent, QString name);
     ~RShelterModel();

    int resetTime();

    int getNumberOfBedsAvailable() const;
    void setNumberOfBedsAvailable(int value);

    int getNumberOfBedsThatCanBeCreated() const;
    void setNumberOfBedsThatCanBeCreated(int value);

    int getNumberOfStoreys() const;
    void setNumberOfStoreys(int value);

    int getFootPrintArea() const;
    void setFootPrintArea(int value);

    QString getBuildingType() const;
    void setBuildingType(QString value);

    QString getOccupancyClass() const;
    void setOccupancyClass(QString value);

    QString getDesignLevel() const;
    void setDesignLevel(QString value);

    QObject* getStructuralDamage() const;
    void setStructuralDamage(QObject *value);

    QObject* getDriftDamage() const;
    void setDriftDamage(QObject *value);

    QObject* getAccelerationDamage() const;
    void setAccelerationDamage(QObject *value);

    QObject* getDirectEconomicLoss() const;
    void setDirectEconomicLoss(QObject *value);

    QObject* getRepairTime() const;
    void setRepairTime(QObject *value);

    QString getWaterSupplierList();
    void setWaterSupplierList(QString value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    QString getHouseholdsList();
    void setHouseholdsList(QString value);

    int evaluateModel(RGradientType theGradientType);

protected:


private:
    // Member functions

    // Data members
    QPointer<RResponse> theStructuralDamageResponse = nullptr;
    QPointer<RResponse> theNDDamageResponse = nullptr;
    QPointer<RResponse> theNADamageResponse = nullptr;
    QPointer<RResponse> theDirectEconomicLossResponse = nullptr;
    QPointer<RResponse> theRepairTimeResponse = nullptr;


    QString theBuildingType;
    QString theOccupancyClass;
    QString theDesignLevel;
    int theNumberOfBedsAvailable;
    int theNumberOfBedsToBeCreated;
    int totalNumberOfBeds;
    int theNumberOfStoreys;
    int currentNumberOfPeople;
    double theFootPrintArea;
    double theTimeUnderRepairs;

    QList<QPointer<RParameter>> theBuildingDamageModelList;
    QList<QPointer<RParameter>> thePowerSupplierList;
    QList<QPointer<RParameter>> theWaterSupplierList;
    QList<QPointer<RParameter>> theHouseholdList;

    bool theAvailabilityOfLPG;
    bool theAvailabilityOfWater;
    bool theAvailabilityOfPower;

    double thePowerShortageDuration;
    double theWaterShortageDuration;

    QVector<double> structuralDamageProbabilities;
    QVector<double> nonStructuralDriftDamageProbabilities;
    QVector<double> nonStructuralAccelerationDamageProbabilities;
    QVector<double> structuralDamageRatios;
    QVector<double> nonStructuralDriftDamageRatios;
    QVector<double> nonStructuralAccelerationDamageRatios;

    double timeNeededForRepairs;
    double timeToResumeServices;
    double waterNetworkExpectedRepairTime;
    double powerNetworkExpectedRepairTime;



    RResponse *theResponse;
};

#endif
