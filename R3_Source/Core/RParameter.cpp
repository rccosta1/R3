#include "RParameter.h"

RParameter::RParameter(QObject *parent, QString name)
    : RObject(parent, name)
{
    theCurrentValue = 0.0;
    NodalParameterFlag = false;


    //the default current value lambda
    theCurrentValueLambda = [&](RParameter* theDDMParameter, RParameter* theDDMParameter2)
    {
        if(theDDMParameter != nullptr || theDDMParameter2 != nullptr)
        {
            if(theDDMParameter == this)
                return 1.0;
            else
                return 0.0;
        }

        return theCurrentValue;
    };

}

RParameter::~RParameter()
{

}

double RParameter::getCurrentValue(RParameter* theDDMParameter, RParameter* theDDMParameter2)  const
{   
    return theCurrentValueLambda(theDDMParameter, theDDMParameter2);
}


//Warning:: if the theCurrentValueLambda gets replaced with a "custom" lambda, this will no longer work, since the default current value lambda will be overwritten
void RParameter::setCurrentValue(double value)
{
    theCurrentValue = value;
}

void RParameter::setCurrentValue(std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> LambdaValue)
{
    theCurrentValueLambda = std::move(LambdaValue);  //Transfer ownership to the RParameter of lambda
}


void RParameter::setNodalParameterFlag(bool flag)  //SG add
{

    NodalParameterFlag = flag;

}


bool RParameter::isNodalParameter()  //SG add
{

    return NodalParameterFlag;

}
