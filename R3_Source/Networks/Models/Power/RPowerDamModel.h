#ifndef RPowerDamModel_H
#define RPowerDamModel_H

#include "RModel.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "RContinuousRandomVariable.h"


class RParameter;
class RResponse;

class RPowerDamModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RFacilityType FacilityType READ getFacilityType WRITE setFacilityType)
    Q_PROPERTY(double Capacity READ getCapacity WRITE setCapacity)
    Q_PROPERTY(int YearBuilt READ getYearBuilt WRITE setYearBuilt)
    Q_PROPERTY(double Length READ getLength WRITE setLength)
    Q_PROPERTY(double Height READ getHeight WRITE setHeight)
    Q_PROPERTY(QString PowerSupplierList READ getPowerSupplierList WRITE setPowerSupplierList)

    Q_ENUMS(RFacilityType)
public:

    RPowerDamModel(QObject *parent, QString name);
    ~RPowerDamModel();

    int resetTime();

    enum RFacilityType{SmallAnchored, SmallUnanchored, LargeAnchored, LargeUnanchored};
    RFacilityType getFacilityType() const;
    void setFacilityType(RFacilityType value);

    double getCapacity() const;
    void setCapacity(double value);

    int getYearBuilt() const;
    void setYearBuilt(int value);

    double getLength() const;
    void setLength(double value);

    double getHeight() const;
    void setHeight(double value);

    QString getPowerSupplierList();
    void setPowerSupplierList(QString value);

    int evaluateModel();
protected:


private:
    // Data Members
    RFacilityType theFacilityType;
    double theCapacity;
    int theYearBuilt;
    double theLength;
    double theHeight;
    double theTimeUnderRepairs;

    QList<QPointer<RParameter> > thePowerSupplierList;
    RResponse *theResponse;
};

#endif
