#ifndef RSimpleBuildingLossModel_H
#define RSimpleBuildingLossModel_H

#include "RModel.h"
#include "RResponse.h"
#include "RProbabilityDistributions.h"

class RParameter;
class RResponse;


class RSimpleBuildingLossModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *StructuralDamage READ getStructuralDamage WRITE setStructuralDamage)
    Q_PROPERTY(QObject *NSDriftDamage READ getNSDriftDamage WRITE setNSDriftDamage)
    Q_PROPERTY(QObject *NSAccelerationDamage READ getNSAccelerationDamage WRITE setNSAccelerationDamage)
    Q_PROPERTY(QObject *ContentDamageRatios READ getContentDamageRatios WRITE setContentDamageRatios)
    Q_PROPERTY(double BuildingValue READ getBuildingValue WRITE setBuildingValue)
    Q_PROPERTY(QString BuildingOccupancyClass READ getBuildingOccupancyClass WRITE setBuildingOccupancyClass)




public:
    // Define the constructor and destructor
    RSimpleBuildingLossModel(QObject *parent, QString name);
    ~RSimpleBuildingLossModel();

    int resetTime();

    QObject* getStructuralDamage() const;
    void setStructuralDamage(QObject* value);

    QObject* getNSDriftDamage() const;
    void setNSDriftDamage(QObject* value);

    QObject* getNSAccelerationDamage() const;
    void setNSAccelerationDamage(QObject* value);

    QObject* getBuildingDamageRatios() const;
    void setBuildingDamageRatios(QObject* value);

    QObject* getContentDamageRatios() const;
    void setContentDamageRatios(QObject* value);

    double getBuildingValue() const;
    void setBuildingValue(double value);

    QString getBuildingOccupancyClass() const;
    void setBuildingOccupancyClass(QString value);



    // Define the method where the actual model is coded
    int evaluateModel();

private:

    // DATA MEMBERS
    // Responses coming to this model
    QPointer<RResponse> theStructuralDamageRatioResponse;
    QPointer<RResponse> theNDDamageRatioResponse;
    QPointer<RResponse> theNADamageRatioResponse;
    QPointer<RResponse> theContentDamageRatiosResponse;


    // Attributes
    double theBuildingValue;
    QString theOccupancyClass;
    QVector<double> theSRepairCost;
    QVector<double> theNSARepairCost;
    QVector<double> theNSDRepairCost;
    QVector<double> theContentReplacementCost;


    // Responses from this model
    RResponse *theBuildingLossResponse;
    RResponse *theDisruptionCostResponse;
    RResponse *theContentLossResponse;
    RResponse *theTotalLossResponse;

};

#endif // RSimpleBuildingLossModel_H
