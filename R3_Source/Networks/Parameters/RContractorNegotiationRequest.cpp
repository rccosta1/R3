#include "RContractorNegotiationRequest.h"
#include "RModel.h"
#include "RDomain.h"


RContractorNegotiationRequest::RContractorNegotiationRequest(QObject *parent, QString name)
    : RRequest(parent, name)
{

}

RContractorNegotiationRequest::~RContractorNegotiationRequest()
{

}


QVector<int> RContractorNegotiationRequest::getRepairClasses()
{
    return theRepairClasses;
}

void RContractorNegotiationRequest::setRepairClasses(int value)
{
    theRepairClasses << value;
}


QVector<int> RContractorNegotiationRequest::getNumberOfStoreys()
{
    return theNumberOfStoreys;
}

void RContractorNegotiationRequest::setNumberOfStoreys(int value)
{
    theNumberOfStoreys << value;
}


QVector<QString> RContractorNegotiationRequest::getOccupancyClasses()
{
    return theOccupancyClasses;
}

void RContractorNegotiationRequest::setOccupancyClasses(QString value)
{
    theOccupancyClasses << value;
}

double RContractorNegotiationRequest::getMedianIncome()
{
    return theMedianIncome;
}

void RContractorNegotiationRequest::setMedianIncome(double value)
{
    theMedianIncome = value;
}


void RContractorNegotiationRequest::clearRepairClassList()
{
    theRepairClasses.clear();
}

void RContractorNegotiationRequest::clearNumberOfStoreysList()
{
    theNumberOfStoreys.clear();
}

void RContractorNegotiationRequest::clearOccupancyClassList()
{
    theOccupancyClasses.clear();
}




