#include "RSingleFamilyContractorRequest.h"
#include "RModel.h"
#include "RDomain.h"


RSingleFamilyContractorRequest::RSingleFamilyContractorRequest(QObject *parent, QString name)
    : RRequest(parent, name)
{

}

RSingleFamilyContractorRequest::~RSingleFamilyContractorRequest()
{

}


QVector<int> RSingleFamilyContractorRequest::getRepairClasses()
{
    return theRepairClasses;
}

void RSingleFamilyContractorRequest::setRepairClasses(int value)
{
    theRepairClasses << value;
}


QVector<int> RSingleFamilyContractorRequest::getNumberOfStoreys()
{
    return theNumberOfStoreys;
}

void RSingleFamilyContractorRequest::setNumberOfStoreys(int value)
{
    theNumberOfStoreys << value;
}


QVector<QString> RSingleFamilyContractorRequest::getOccupancyClasses()
{
    return theOccupancyClasses;
}

void RSingleFamilyContractorRequest::setOccupancyClasses(QString value)
{
    theOccupancyClasses << value;
}

double RSingleFamilyContractorRequest::getMedianIncome()
{
    return theMedianIncome;
}

void RSingleFamilyContractorRequest::setMedianIncome(double value)
{
    theMedianIncome = value;
}

QVector<double> RSingleFamilyContractorRequest::getBid()
{
    return theBidList;
}

void RSingleFamilyContractorRequest::setBid(double value)
{
    theBidList << value;
}



void RSingleFamilyContractorRequest::clearRepairClassList()
{
    theRepairClasses.clear();
}

void RSingleFamilyContractorRequest::clearNumberOfStoreysList()
{
    theNumberOfStoreys.clear();
}

void RSingleFamilyContractorRequest::clearOccupancyClassList()
{
    theOccupancyClasses.clear();
}

void RSingleFamilyContractorRequest::clearBidList()
{
    theBidList.clear();
}




