#ifndef RFuelTankFarmModel_H
#define RFuelTankFarmModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"
#include "RFragilityFunctions.h"


class RParameter;
class RResponse;

class RFuelTankFarmModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RFacilityType FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(QObject *FillLevel READ getFillLevel WRITE setFillLevel)
    Q_PROPERTY(double MaxCapacity READ getMaxCapacity WRITE setMaxCapacity)
    Q_PROPERTY(int NumberOfStorageTanks READ getNumberOfStorageTanks WRITE setNumberOfStorageTanks)
    Q_PROPERTY(QString FuelSupplierList READ getFuelSupplierList WRITE setFuelSupplierList)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)

    Q_ENUMS(RFacilityType)
public:

    RFuelTankFarmModel(QObject *parent, QString name);
    ~RFuelTankFarmModel();

    int resetTime();


    enum RFacilityType{NoDamage, OnGroundConcreteAnchored, OnGroundConcreteUnanchored, OnGroundSteelAnchored, OnGroundSteelUnanchored, AboveGroundSteel, AboveGroundWood, BuriedConcrete};

    RFacilityType getFacilityType() const;
    void setFacilityType(RFacilityType value);

    QObject *getFillLevel() const;
    void setFillLevel(QObject *value);

    double getMaxCapacity() const;
    void setMaxCapacity(double value);
    void changeCurrentCapacity(double value);

    void setReserve(double value);

    int getNumberOfStorageTanks() const;
    void setNumberOfStorageTanks(int value);

    QString getFuelSupplierList();
    void setFuelSupplierList(QString value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    void setShortageDuration();

    void printShortageDuration();

    int extractSupply(double amount);

    void getRequests();

    int evaluateModel();

protected:
    // Data members
    QPointer<RParameter> theFillLevel;

    int theNumberOfTanks;

    double maxStorageCapacity;
    double theCurrentCapacity;
    double tankCapacity;
    double theReserve;
    double durationOfShortage;

private:
    void plotCurrentReserves(double delta, double currentReserves);

    // Data members
    QList<QPointer<RParameter> > theFuelSupplierList;
    RFacilityType theFacilityType;


    bool theAvailabilityOfFuel;
    bool theAvailabilityOfPower;
    double theCounter;
    double lim1;
    double lim2;
    double lim3;
    double lim4;
    double theVolumeToBeRequested;
    double timeAnalysisBegan;
    double timeOfLastRequest;


    QList<QPointer<RParameter> > theMaterialsSupplierList;
    QVector<QPointer<RModel>> theAuxDestinationList; // this is a copy of the destination list
    QVector<double> theAuxCriterionList; // this is a copy of the list of bids
    QVector<QVector<double>> theAuxAmountList; // this is a copy of the list of requests
    QVector<QString> theAuxSupplyTypeList;
    QList<QVector<int>> theAuxIDList;


    QList<double> rngs;
    QList<double> theFunctionalityMatrix;
    QVector<double> timeUnderRepairsVector;
    QVector<double> tickVector;
    QVector<QString> labelVector;
    QList<double> shortageDuration;


    QList<QPointer<RNetworkModel >> myPrimaryFuelSupplierList;
    QList<QPointer<RNetworkModel >> myAlternativeFuelSupplierList;
    QList<QPointer<RParameter> > thePowerSupplierList;


    RResponse *theResponse;
    RResponse *thePacketResponse;
};

#endif
