#include "RRandomVariable.h"

RRandomVariable::RRandomVariable(QObject *parent, QString name)
	: RParameter(parent, name)
{

}

RRandomVariable::~RRandomVariable()
{

}


int RRandomVariable::updateParameters()
{
    return 0;
}
