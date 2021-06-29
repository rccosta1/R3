#include "RPortStorageFacility.h"
#include "RParameter.h"
#include "RTimeSeries.h"
#include "RIntensityModel.h"
#include "RNormalDistribution.h"
#include "RMainWindow.h"
#include <math.h>

RPortStorageFacility::RPortStorageFacilityModel(QObject *parent, QString name)
    : RFixedLocationNetworkModel(parent, name)
{
    /*
    The fuel facility consists mainly of fuel storage tanks, buildings, pump equipment,
    piping, and, sometimes, backup power.
    */

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    thePacketResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePacketResponse->setModel(this);

    // Fuel facilities - (see Hazus pg. 316/319 for Anchored + back-up power): PGA
    PGADamageMedians << 0.23 << 0.43 << 0.64 << 1.10;
    PGADamageDispersions << 0.50 << 0.45 << 0.60 << 0.60;

    // Fuel facilities - (see Hazus pg. 316/319 for Anchored + no back-up power): PGA
    // PGADamageMedians << 0.12 << 0.27 << 0.64 << 1.10;
    // PGADamageDispersions << 0.55 << 0.50 << 0.60 << 0.60;

    // Fuel facilities - (see Hazus pg. 316/319 for Unnchored + back-up power): PGA
    // PGADamageMedians << 0.10 << 0.23 << 0.48 << 0.80;
    // PGADamageDispersions << 0.55 << 0.50 << 0.60 << 0.60;

    // Fuel facilities - (see Hazus pg. 316/319 for Unnchored + no back-up power): PGA
    // PGADamageMedians << 0.09 << 0.20 << 0.48 << 0.80;
    // PGADamageDispersions << 0.50 << 0.45 << 0.60 << 0.60;

    restorationMeans << 0.9  << 1.5 << 15.0 << 65.0;
    restorationStdvs << 0.05 << 1.5 << 15.0 << 50.0;

    // see Hazus pg. 629
    damageRatios << 0.0 << 0.16 << 0.39 << 0.80 << 1.00;

    // Only buried tanks are susceptible to PGD damage
    // PGDDamageMedians << 4.0 << 8.0 << 24.0 << 24.0;
    // PGDDamageDispersions << 0.50 << 0.50 << 0.50 << 0.50;

    PGDDamageMedians << 1000.0 << 1000.0 << 1000.0 << 1000.0;
    PGDDamageDispersions << 0.50 << 0.50 << 0.50 << 0.50;
}

RPortStorageFacility::~RPortStorageFacility()
{
}



double RPortStorageFacility::getMeanRecordedDamageRatio() const
{
    return theMeanRecordedDamageRatio;
}


void RPortStorageFacility::setMeanRecordedDamageRatio(double value)
{
    theMeanRecordedDamageRatio = value;
}


double RPortStorageFacility::getMeanRecordedTimeUntilRecovery() const
{
    return theMeanRecordedTimeUntilRecovery;
}


void RPortStorageFacility::setMeanRecordedTimeUntilRecovery(double value)
{
    theMeanRecordedTimeUntilRecovery = value;
}


int RPortStorageFacility::evaluateModel(RGradientType theGradientType)
{
    // OUTPUT
    if (theOutputLevel == RDomain::Maximum) {
        //qDebug() << "";
        //qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
        QCoreApplication::processEvents();
    }


    // EARTHQUAKE
    calculateIntensityAndDamage();


    return 1;
}


