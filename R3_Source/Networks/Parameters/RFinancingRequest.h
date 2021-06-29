#ifndef RFinancingRequest_H
#define RFinancingRequest_H

#include "RRequest.h"
#include "RParameter.h"
#include "RDomain.h"

class RModel;

class RFinancingRequest : public RRequest
{
    Q_OBJECT

public:
    RFinancingRequest(QObject *parent, QString name);
    virtual ~RFinancingRequest();

    QVector<double> getRequesterBuildingValue();
    void setRequesterBuildingValue(double value);

    QVector<QString> getRequesterOccupancyType();
    void setRequesterOccupancyType(QString value);

    QVector<QString> getRequesterIncome();
    void setRequesterIncome(QString value);

    void clearBuildingValueList();
    void clearOccupancyTypeList();
    void clearIncomeList();


private:


    QVector<double> theRequesterBuildingValue;
    QVector<QString> theRequesterOccupancyType;
    QVector<QString> theRequesterIncome;
};

#endif // RFinancingRequest_H
