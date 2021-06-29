#ifndef RPermitRequest_H
#define RPermitRequest_H

#include "RParameter.h"
#include "RDomain.h"
#include "RRequest.h"

class RModel;

class RPermitRequest : public RRequest
{
    Q_OBJECT

public:
    RPermitRequest(QObject *parent, QString name);
    virtual ~RPermitRequest();

    QVector<int> getRepairClasses();
    void setRepairClasses(int value);

    QVector<QString> getOccupancyClasses();
    void setOccupancyClasses(QString value);

    void clearRepairClassList();
    void clearOccupancyClassList();

private:

    QVector<int> theRepairClasses;
    QVector<QString> theOccupancyClasses;

};

#endif // RPermitRequest_H
