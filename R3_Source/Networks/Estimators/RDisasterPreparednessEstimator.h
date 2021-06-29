#ifndef RDisasterPreparednessEstimator_H
#define RDisasterPreparednessEstimator_H

#include "RObject.h"
#include <math.h>

class RDomain;
class RRandomVariable;

class RDisasterPreparednessEstimator : public RObject
{
    Q_OBJECT

public:

    RDisasterPreparednessEstimator(QObject *parent, QString name);
    ~RDisasterPreparednessEstimator();

    void setTenure(QString value);
    void setIncome(QString value);
    void setImmigrationStatus(QString value);
    void setChildren(bool value);
    void setElders(bool value);
    void setSize(int value);
    void setAge(QString value);
    void setCarOwnership(bool value);
    void setHispanicFlag(bool value);
    void setHigherEducationFlag(bool value);

    virtual bool getGeneratorAvailability();
    virtual bool getWaterAvailability();
    virtual bool getSocialSupportAvailability();
    virtual bool getFinancialResourceAvailability();
    virtual QString getDisasterDestination();

protected:
    QString theTenure;
    QString theHouseholdIncome;
    QString theImmigrationStatus;
    bool thePresenceOfChildren;
    bool thePresenceOfSeniors;
    int theHouseholdSize;
    QString theAge;
    QString theWeatherConditions;
    bool theCarOwnership;
    bool theHispanicFlag;
    bool theHigherEducationFlag;

private:




};

#endif
