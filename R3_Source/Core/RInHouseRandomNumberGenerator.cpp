#include "RInHouseRandomNumberGenerator.h"
#include "RProbabilityTransformation.h"
#include "RRandomVariable.h"
#include "RDomain.h"
#include "RNormalDistribution.h"
#include <stdlib.h>


RInHouseRandomNumberGenerator::RInHouseRandomNumberGenerator(QObject *parent, QString name)
	: RRandomNumberGenerator(parent, name)
{
    theSeed = 0;
    theGeneratedNumbers = 0;
}


RInHouseRandomNumberGenerator::~RInHouseRandomNumberGenerator()
{
    if (theGeneratedNumbers) {
        delete theGeneratedNumbers;
        theGeneratedNumbers = 0;
    }
}




int RInHouseRandomNumberGenerator::getSeed()
{
    return theSeed;
}

void RInHouseRandomNumberGenerator::setSeed(int value)
{
    theSeed = value;
}



int RInHouseRandomNumberGenerator::generateStandardNormalNumbers(QVector<double> *u)
{
    // Check that a proper vector came in
    if (u==0) {
        qCritical() << "The default GSL random number generator did not get a proper vector to fill with numbers.";
        return 0;
    }


    // Check the size of the vector
    double count = u->size();


    // Create a normal distribution tool
    RNormalDistribution theNormalDistribution(theDomain);


    // Initial declarations
    int randomNumberBetween0AndRAND_MAX;
    double randomNumberBetween0And1;


    // Set seed
    if (theSeed != 0) {
        srand(theSeed);
    }

    // Iterate "count" times
    for (int i=0; i<count; i++) {



        // Generate a number between 0 and RAND_MAX
        randomNumberBetween0AndRAND_MAX = rand();

        // Modify it so that the value lies between 0 and 1
        randomNumberBetween0And1 = (double)randomNumberBetween0AndRAND_MAX/RAND_MAX;


        // Treat two special cases
        if (randomNumberBetween0And1 == 0.0) {
            randomNumberBetween0And1 = 0.0000001;
        }
        if (randomNumberBetween0And1 == 1.0) {
            randomNumberBetween0And1 = 0.9999999;
        }


        // Transform that number into a standard normal variable
        //    Phi(z) = F(x)
        //    z = invPhi( F(x) )
        //       where F(x) for the uniform distribution
        //       from 0 to 1 in fact is equal to x itself.
        (*u)[i] = theNormalDistribution.getInverseNormalCDF(randomNumberBetween0And1, 1.0);
    }


    // Reset seed (from Paolo?)
    //seed = randomNumberBetween0AndRAND_MAX;

    return 0;
}



void RInHouseRandomNumberGenerator::reset()
{
}


