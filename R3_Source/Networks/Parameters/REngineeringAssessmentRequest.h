#ifndef REngineeringAssessmentRequest_H
#define REngineeringAssessmentRequest_H

#include "RParameter.h"
#include "RDomain.h"
#include "RRequest.h"

class RModel;

class REngineeringAssessmentRequest : public RRequest
{
    Q_OBJECT

public:
    REngineeringAssessmentRequest(QObject *parent, QString name);
    virtual ~REngineeringAssessmentRequest();

    QVector<int> getRepairClasses();
    void setRepairClasses(int value);

    QVector<QString> getOccupancyClasses();
    void setOccupancyClasses(QString value);

    QVector<double> getFundsAvailability();
    void setFundsAvailability(double value);

    QVector<double> getDelayInTakingAction();
    void setDelayInTakingAction(double value);

    void clearRepairClassList();
    void clearOccupancyClassList();
    void clearFundsAvailabilityList();
    void clearDelayInTakingActionList();
private:

    QVector<int> theRepairClasses;
    QVector<QString> theOccupancyClasses;
    QVector<double> theFundsAvailability;
    QVector<double> theDelayInTakingAction;

};

#endif // REngineeringAssessmentRequest_H
