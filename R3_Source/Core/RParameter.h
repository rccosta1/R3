#ifndef RParameter_H
#define RParameter_H

#include "RObject.h"

#include <functional>

class RParameter : public RObject
{
	Q_OBJECT

public:
	RParameter(QObject *parent, QString name);
	virtual ~RParameter();
	
    virtual double  getCurrentValue(RParameter* theDDMParameter = nullptr, RParameter* theDDMParameter2 = nullptr) const;
    virtual void    setCurrentValue(double value);
    virtual void    setCurrentValue(std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2)> LambdaValue);

    // Is this a nodal Parameter?
    void setNodalParameterFlag(bool flag);  //SG add
    bool isNodalParameter();  //SG add

protected:

    double theCurrentValue;

private:

    std::function<double(RParameter* theDDMParameter, RParameter* theDDMParameter2 )> theCurrentValueLambda;

    bool NodalParameterFlag; //SG add

};

#endif
