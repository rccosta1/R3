#ifndef RSimpleBuildingRepairNeedsModel_H
#define RSimpleBuildingRepairNeedsModel_H

#include "RModel.h"
#include "RContinuousRandomVariable.h"
#include "RProbabilityDistributions.h"


class RParameter;
class RResponse;

class RSimpleBuildingRepairNeedsModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QString OccupancyClass READ getOccupancyClass WRITE setOccupancyClass)
    Q_PROPERTY(QObject *StructuralDamage READ getStructuralDamage WRITE setStructuralDamage)
    Q_PROPERTY(QObject *IrreparabilityMultiplier READ getIrreparabilityMultiplier WRITE setIrreparabilityMultiplier)

public:
    // Define the constructor and destructor
    RSimpleBuildingRepairNeedsModel(QObject *parent, QString name);
    ~RSimpleBuildingRepairNeedsModel();

    int resetTime();


    QString getOccupancyClass() const;
    void setOccupancyClass(QString value);

    QObject *getStructuralDamage() const;
    void setStructuralDamage(QObject* value);

    QObject* getIrreparabilityMultiplier() const;
    void setIrreparabilityMultiplier(QObject* value);


    // Define the method where the actual model is coded
    int evaluateModel();

private:

    // MEMBER FUNCTIONS

    // DATA MEMBERS
    double lastEvaluationTime;

    QString theOccupancyClass;

    // Responses coming to this model
    QList<QPointer<RParameter>> buildingLossModelList;
    QPointer<RResponse> theStructuralDamageRatioResponse;
    QPointer<RResponse> theIrreparabilityMultiplierResponse;


    // Census data
    QString buildingOccupancyClass;

    // Loss data
    QVector<double> theRepairTimeMedians;
    QVector<double> theRepairTimeStandardDeviations;
    QVector<double> serviceInterruptionMultipliers;


    RResponse *theRepairTimeResponse;
    RResponse *theBusinessInterruptionTimeResponse;

};

#endif // RSimpleBuildingRepairNeedsModel_H
