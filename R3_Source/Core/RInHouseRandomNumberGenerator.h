#ifndef RInHouseRandomNumberGenerator_H
#define RInHouseRandomNumberGenerator_H

#include "RRandomNumberGenerator.h"


class RProbabilityTransformation;

class RInHouseRandomNumberGenerator : public RRandomNumberGenerator
{
	Q_OBJECT

    Q_PROPERTY(int Seed READ getSeed WRITE setSeed)

public:
	RInHouseRandomNumberGenerator(QObject *parent, QString name);
	~RInHouseRandomNumberGenerator();

    int getSeed();
    void setSeed(int value);

    int    generateStandardNormalNumbers(QVector<double> *u);
    void   reset();

private:

    int theSeed;
    QVector<double> *theGeneratedNumbers;

};

#endif
