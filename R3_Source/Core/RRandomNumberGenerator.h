#ifndef RRandomNumberGenerator_H
#define RRandomNumberGenerator_H

#include "RMethod.h"


class RRandomVariable;

class RRandomNumberGenerator : public RMethod
{
	Q_OBJECT

public:
	RRandomNumberGenerator(QObject *parent, QString name);
	virtual ~RRandomNumberGenerator();

    virtual int    generateStandardNormalNumbers(QVector<double> *u) = 0;
    virtual void   reset() = 0;

private:

};

#endif
