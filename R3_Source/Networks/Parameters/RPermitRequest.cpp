#include "RPermitRequest.h"
#include "RModel.h"
#include "RDomain.h"


RPermitRequest::RPermitRequest(QObject *parent, QString name)
    : RRequest(parent, name)
{

}

RPermitRequest::~RPermitRequest()
{

}


QVector<int> RPermitRequest::getRepairClasses()
{
    return theRepairClasses;
}

void RPermitRequest::setRepairClasses(int value)
{
    theRepairClasses << value;
}


QVector<QString> RPermitRequest::getOccupancyClasses()
{
    return theOccupancyClasses;
}

void RPermitRequest::setOccupancyClasses(QString value)
{
    theOccupancyClasses << value;
}

void RPermitRequest::clearRepairClassList()
{
    theRepairClasses.clear();
}

void RPermitRequest::clearOccupancyClassList()
{
    theOccupancyClasses.clear();
}

