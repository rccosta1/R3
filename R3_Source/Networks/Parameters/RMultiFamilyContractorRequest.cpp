#include "RMultiFamilyContractorRequest.h"
#include "RModel.h"
#include "RDomain.h"


RMultiFamilyContractorRequest::RMultiFamilyContractorRequest(QObject *parent, QString name)
    : RRequest(parent, name)
{

}

RMultiFamilyContractorRequest::~RMultiFamilyContractorRequest()
{

}


QVector<int> RMultiFamilyContractorRequest::getRepairClasses()
{
    return theRepairClasses;
}

void RMultiFamilyContractorRequest::setRepairClasses(int value)
{
    theRepairClasses << value;
}


QVector<int> RMultiFamilyContractorRequest::getNumberOfStoreys()
{
    return theNumberOfStoreys;
}

void RMultiFamilyContractorRequest::setNumberOfStoreys(int value)
{
    theNumberOfStoreys << value;
}


QVector<QString> RMultiFamilyContractorRequest::getOccupancyClasses()
{
    return theOccupancyClasses;
}

void RMultiFamilyContractorRequest::setOccupancyClasses(QString value)
{
    theOccupancyClasses << value;
}

double RMultiFamilyContractorRequest::getMedianIncome()
{
    return theMedianIncome;
}

void RMultiFamilyContractorRequest::setMedianIncome(double value)
{
    theMedianIncome = value;
}

QVector<double> RMultiFamilyContractorRequest::getBid()
{
    return theBidList;
}

void RMultiFamilyContractorRequest::setBid(double value)
{
    theBidList << value;
}



void RMultiFamilyContractorRequest::clearRepairClassList()
{
    theRepairClasses.clear();
}

void RMultiFamilyContractorRequest::clearNumberOfStoreysList()
{
    theNumberOfStoreys.clear();
}

void RMultiFamilyContractorRequest::clearOccupancyClassList()
{
    theOccupancyClasses.clear();
}




