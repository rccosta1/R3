#include "RFinancingRequest.h"
#include "RModel.h"
#include "RDomain.h"


RFinancingRequest::RFinancingRequest(QObject *parent, QString name)
    : RRequest(parent, name)
{

}

RFinancingRequest::~RFinancingRequest()
{

}


QVector<double> RFinancingRequest::getRequesterBuildingValue()
{
    return theRequesterBuildingValue;
}


void RFinancingRequest::setRequesterBuildingValue(double value)
{
    theRequesterBuildingValue << value;
}


QVector<QString> RFinancingRequest::getRequesterOccupancyType()
{
    return theRequesterOccupancyType;
}

void RFinancingRequest::setRequesterOccupancyType(QString value)
{
    theRequesterOccupancyType << value;
}

QVector<QString> RFinancingRequest::getRequesterIncome()
{
    return theRequesterIncome;
}

void RFinancingRequest::setRequesterIncome(QString value)
{
    theRequesterIncome << value;
}

void RFinancingRequest::clearBuildingValueList()
{
    if (theRequesterBuildingValue.size() > 0){
        theRequesterBuildingValue.clear();
    }
}


void RFinancingRequest::clearOccupancyTypeList()
{
    if (theRequesterBuildingValue.size() > 0){
        theRequesterBuildingValue.clear();
    }
}

void RFinancingRequest::clearIncomeList(){
    if (theRequesterIncome.size() > 0){
        theRequesterIncome.clear();
    }
}

