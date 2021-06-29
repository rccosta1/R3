#ifndef RWaterStorageTankModel_H
#define RWaterStorageTankModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RWaterStorageTankModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RFacilityType FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(int YearBuilt READ getYearBuilt WRITE setYearBuilt)
    Q_PROPERTY(QObject *FillLevel READ getFillLevel WRITE setFillLevel)
    Q_PROPERTY(double MaxCapacity READ getMaxCapacity WRITE setMaxCapacity)
    Q_PROPERTY(int NumberOfStorageTanks READ getNumberOfStorageTanks WRITE setNumberOfStorageTanks)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)


    Q_ENUMS(RFacilityType)
public:

    RWaterStorageTankModel(QObject *parent, QString name);
    ~RWaterStorageTankModel();

    int resetTime();

    enum RFacilityType{NoDamage, OnGroundConcreteAnchored, OnGroundConcreteUnanchored, OnGroundSteelAnchored, OnGroundSteelUnanchored, AboveGroundSteel, AboveGroundWood, BuriedConcrete};

    RFacilityType getFacilityType() const;
    void setFacilityType(RFacilityType value);

    int getYearBuilt() const;
    void setYearBuilt(int value);

    double getMaxCapacity() const;
    void setMaxCapacity(double value);
    void changeCurrentCapacity(double value);

    QObject *getFillLevel() const;
    void setFillLevel(QObject *value);

    void setReserve(double value);

    int getNumberOfStorageTanks() const;
    void setNumberOfStorageTanks(int value);

    QString getWaterSupplierList();
    void setWaterSupplierList(QString value);

    int extractSupply(double amount);

    int evaluateModel();

protected:



private:
    // Data members
    RFacilityType theFacilityType;
    double theTankCapacity;
    QPointer<RParameter> theFillLevel;

    bool theAvailabilityOfWater;
    int theYearBuilt;
    int theNumberOfTanks;
    double lim1;
    double lim2;
    double lim3;
    double lim4;
    double theMaxStorageCapacity;
    double theCurrentCapacity;
    double theReserve;
    double theShortageDuration;

    QVector<double> theTimeUnderRepairsVector;
    QVector<int> theTankDamageStateVector;

    QList<QPointer<RParameter> > theWaterSupplierList;
    RResponse *theResponse;

};

#endif
