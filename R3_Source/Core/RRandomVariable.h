#ifndef RRandomVariable_H
#define RRandomVariable_H

#include "RParameter.h"

class RRandomVariable : public RParameter
{
	Q_OBJECT

	// Current realization of the random variable
	Q_PROPERTY(double CurrentValue READ getCurrentValue WRITE setCurrentValue)

public:
	RRandomVariable(QObject *parent, QString name);
	virtual ~RRandomVariable();

	// Returns the mean value of the random variable as a double
	virtual double getMeanValue() = 0;

	// Returns the stdv of the random variable as a double
	virtual double getStandardDeviationValue() = 0;

	// Returns the PDF at a given realization of the random variable
	virtual double getPDFValue(double rvValue) = 0;

	// Returns the CDF at a given realization of the random variable
	virtual double getCDFValue(double rvValue) = 0;

	// Returns the inverse CDF at a given probability value
	virtual double getInverseCDFValue(double probValue) = 0;

	// Returns a list of parameters that are input to the random variable as mean, stdv, etc.
	virtual QList<RParameter *> getParameterList() = 0;

    virtual int updateParameters();

	
private:

};

#endif
