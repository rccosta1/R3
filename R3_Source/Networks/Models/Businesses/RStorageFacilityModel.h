#ifndef RStorageFacilityModel_H
#define RStorageFacilityModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"
#include "RContinuousRandomVariable.h"
#include "RFragilityFunctions.h"


class RParameter;
class RResponse;

class RStorageFacilityModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *FillLevel READ getFillLevel WRITE setFillLevel)
    Q_PROPERTY(double MaxCapacity READ getMaxCapacity WRITE setMaxCapacity)
    Q_PROPERTY(int NumberOfStorageTanks READ getNumberOfStorageTanks WRITE setNumberOfStorageTanks)

public:

    RStorageFacilityModel(QObject *parent, QString name);
    ~RStorageFacilityModel();

    QObject *getFillLevel() const;
    void setFillLevel(QObject *value);

    double getMaxCapacity() const;
    void setMaxCapacity(double value);
    void changeCurrentCapacity(double value);

    void setReserve(double value);

    int getNumberOfStorageTanks() const;
    void setNumberOfStorageTanks(int value);

    void setShortageDuration();

    void printShortageDuration();

    int extractSupply(double amount);

protected:

    QPointer<RParameter> theFillLevel;

    int theNumberOfTanks;

    double maxStorageCapacity;
    double theCurrentCapacity;
    double tankCapacity;
    double theReserve;
    double durationOfShortage;

private:
    // Data member
    QList<double> shortageDuration;


};

#endif
