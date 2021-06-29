#ifndef RContractorNegotiationRequest_H
#define RContractorNegotiationRequest_H

#include "RParameter.h"
#include "RDomain.h"
#include "RRequest.h"

class RModel;

class RContractorNegotiationRequest : public RRequest
{
    Q_OBJECT

public:
    RContractorNegotiationRequest(QObject *parent, QString name);
    virtual ~RContractorNegotiationRequest();


    QVector<int> getRepairClasses();
    void setRepairClasses(int value);

    QVector<int> getNumberOfStoreys();
    void setNumberOfStoreys(int value);

    QVector<QString> getOccupancyClasses();
    void setOccupancyClasses(QString value);

    double getMedianIncome();
    void setMedianIncome(double value);

    void clearRepairClassList();
    void clearNumberOfStoreysList();
    void clearOccupancyClassList();
    void clearDelayList();


private:

    double theMedianIncome;
    QVector<int> theRepairClasses;
    QVector<int> theNumberOfStoreys;
    QVector<QString> theOccupancyClasses;

};

#endif // RContractorNegotiationRequest_H
