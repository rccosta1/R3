#ifndef RMultiFamilyContractorRequest_H
#define RMultiFamilyContractorRequest_H

#include "RParameter.h"
#include "RDomain.h"
#include "RRequest.h"

class RModel;

class RMultiFamilyContractorRequest : public RRequest
{
    Q_OBJECT

public:
    RMultiFamilyContractorRequest(QObject *parent, QString name);
    virtual ~RMultiFamilyContractorRequest();


    QVector<int> getRepairClasses();
    void setRepairClasses(int value);

    QVector<int> getNumberOfStoreys();
    void setNumberOfStoreys(int value);

    QVector<QString> getOccupancyClasses();
    void setOccupancyClasses(QString value);

    double getMedianIncome();
    void setMedianIncome(double value);

    QVector<double> getBid();
    void setBid(double value);

    void clearRepairClassList();
    void clearNumberOfStoreysList();
    void clearOccupancyClassList();
    void clearDelayList();


private:

    double theMedianIncome;
    QVector<int> theRepairClasses;
    QVector<int> theNumberOfStoreys;
    QVector<double> theBidList;
    QVector<QString> theOccupancyClasses;

};

#endif // RMultiFamilyContractorRequest_H
