#include "REngineeringAssessmentRequest.h"
#include "RModel.h"
#include "RDomain.h"

REngineeringAssessmentRequest::REngineeringAssessmentRequest(QObject *parent, QString name)
    : RRequest(parent, name)
{

}

REngineeringAssessmentRequest::~REngineeringAssessmentRequest()
{

}


QVector<int> REngineeringAssessmentRequest::getRepairClasses()
{
    return theRepairClasses;
}

void REngineeringAssessmentRequest::setRepairClasses(int value)
{
    theRepairClasses << value;
}


QVector<QString> REngineeringAssessmentRequest::getOccupancyClasses()
{
    return theOccupancyClasses;
}

void REngineeringAssessmentRequest::setOccupancyClasses(QString value)
{
    theOccupancyClasses << value;
}


QVector<double> REngineeringAssessmentRequest::getFundsAvailability()
{
    return theFundsAvailability;
}

void REngineeringAssessmentRequest::setFundsAvailability(double value)
{
    theFundsAvailability << value;
}

QVector<double> REngineeringAssessmentRequest::getDelayInTakingAction()
{
    return theDelayInTakingAction;
}

void REngineeringAssessmentRequest::setDelayInTakingAction(double value)
{
    theDelayInTakingAction << value;
}


void REngineeringAssessmentRequest::clearRepairClassList()
{
    theRepairClasses.clear();
}

void REngineeringAssessmentRequest::clearOccupancyClassList()
{
    theOccupancyClasses.clear();
}

void REngineeringAssessmentRequest::clearFundsAvailabilityList()
{
    theFundsAvailability.clear();
}

void REngineeringAssessmentRequest::clearDelayInTakingActionList()
{
    theDelayInTakingAction.clear();
}
