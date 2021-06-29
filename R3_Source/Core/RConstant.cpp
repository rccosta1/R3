#include "RConstant.h"
#include "RDomain.h"


RConstant::RConstant(QObject *parent, QString name)
    : RParameter(parent, name)
{
}


RConstant::~RConstant()
{

}


QStringList RConstant::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    if(theDecisionSlaveParameter == nullptr)
        actionList << "&decisionVariable";
    else
        actionList << "&Remove the decision variable";
    return actionList;
}


void RConstant::setUpstreamParameterList(QList<RParameter *> parameterList)
{
    theUpstreamParameterList = parameterList;
}

QList<RParameter *> RConstant::getUpstreamParameterList()
{
    return theUpstreamParameterList;
}
