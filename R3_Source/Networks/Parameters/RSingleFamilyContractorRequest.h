#ifndef RSingleFamilyContractorRequest_H
#define RSingleFamilyContractorRequest_H

#include "RParameter.h"
#include "RDomain.h"
#include "RRequest.h"

class RModel;

class RSingleFamilyContractorRequest : public RRequest
{
    Q_OBJECT

public:
    RSingleFamilyContractorRequest(QObject *parent, QString name);
    virtual ~RSingleFamilyContractorRequest();


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
    void clearBidList();


private:

    double theMedianIncome;
    QVector<int> theRepairClasses;
    QVector<int> theNumberOfStoreys;
    QVector<double> theBidList;
    QVector<QString> theOccupancyClasses;

};

#endif // RSingleFamilyContractorRequest_H
