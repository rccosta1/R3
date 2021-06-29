#include "RHazusBuildingModel.h"

#include "RParameter.h"
#include "RResponse.h"
#include "RNormalDistribution.h"

#include <QtMath>
#include <math.h>

RHazusBuildingModel::RHazusBuildingModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    // Initializing the pointer data members
    theSa1 = 0;
    theSa2 = 0;
    theMagnitude = nullptr;
    theBuildingType = RHazusBuildingModel::W1;
    theOccupancyClass = RHazusBuildingModel::RES1;
    theCodeDesignLevel = QString("High");


    // Instantiating the output response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theDamageFactor = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theDamageFactor->setModel(this);
    theDamageFactor->setCurrentValue(0.0);
}

RHazusBuildingModel::~RHazusBuildingModel()
{

}


double RHazusBuildingModel::getSa1() const
{	
    return theSa1;
}	

void RHazusBuildingModel::setSa1(double value)
{	
    theSa1 = value;
}	


double RHazusBuildingModel::getSa2() const
{	
    return theSa2;
}	

void RHazusBuildingModel::setSa2(double value)
{	
    theSa2 = value;
}

void RHazusBuildingModel::setPGA(double value)
{
    thePGA = value;
}

QString RHazusBuildingModel::getBuildingType() const
{	
    return theBuildingType;
}	

void RHazusBuildingModel::setBuildingType(QString value)
{	
    theBuildingType = value;
}	

QString RHazusBuildingModel::getOccupancyClass() const
{	
    return theOccupancyClass;
}	

void RHazusBuildingModel::setOccupancyClass(QString value)
{	
    theOccupancyClass = value;
}	

QString RHazusBuildingModel::getCodeDesignLevel() const
{	
    return theCodeDesignLevel;
}	

void RHazusBuildingModel::setCodeDesignLevel(QString value)
{	
    theCodeDesignLevel = value;
}

void RHazusBuildingModel::setSoilSusceptibilityLevel(QString value)
{
    theSoilSusceptibilityLevel = value;
}


QObject * RHazusBuildingModel::getMagnitude() const	
{	
    return theMagnitude;
}	

void RHazusBuildingModel::setMagnitude(QObject *value)	
{	
    theMagnitude = qobject_cast<RParameter *>(value);
}	


int RHazusBuildingModel::getStrDS()
{
    return theStrDamageState;
}

int RHazusBuildingModel::getNSDDS()
{
    return theNSDDamageState;
}

int RHazusBuildingModel::getNSADS()
{
    return theNSADamageState;
}

double RHazusBuildingModel::getLossRatio()
{
    return theLossRatio;
}

double RHazusBuildingModel::getContentLossRatio()
{
    return theContentLossRatio;
}

double RHazusBuildingModel::getRepairTime()
{
    return theRepairTime;
}

double RHazusBuildingModel::getReoccupancyTime()
{
    return theReoccupancyTime;
}


int RHazusBuildingModel::evaluateModel()
{	
    double magnitude = theMagnitude->getCurrentValue();
    double Sa1 = theSa1;
    double Sa0_3 = theSa2;

    theStrDamageState = 0;
    theNSDDamageState = 0;
    theNSADamageState = 0;
    theEffectiveDamageRatio = 0;
    theStructuralLossRatio = 0;
    theDriftLossRatio = 0;
    theAccelerationLossRatio = 0;
    theLossRatio = 0;
    theContentLossRatio = 0;
    theRepairTime = 0;

    if (Sa1 < 0.001 && Sa0_3 < 0.001){
        theDamageFactor->setCurrentValue(0.0);
        return 0;
    }

    double kappa = 0.0;
    double Dy = 0.0;
    double Ay = 0.0;
    double Du= 0.0;
    double Au= 0.0;
    double betaE = 0.0;
    double period = 0.0;

    QVector<double> SDBarStrVector;
    QVector<double> SDStdvStrVector;
    QVector<double> SDBarNonstrDriftVector;
    QVector<double> SDStdvNonstrDriftVector;
    QVector<double> SABarNonstrAccVector;
    QVector<double> SAStdvNonstrAccVector;
    QVector<double> PGABarVector;
    QVector<double> PGAStdvVector;

    if (theBuildingType == QString("W1")) {
        betaE = 0.125;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            // According to FEMA-NIBS document:
            //		-- Before 1940: Pre
            //		-- Between 1940-1973: Moderate
            //		-- After 1973: High

            Dy = 0.48;
            Ay = 0.4;
            Du = 11.51;
            Au = 1.2;

            SDBarStrVector << 0.5 << 1.51 << 5.04 << 12.6;
            SDStdvStrVector << 0.8 << 0.81 << 0.85 << 0.97;

            SDBarNonstrDriftVector << 0.5 << 1.01 << 3.15 << 6.3;
            SDStdvNonstrDriftVector << 0.85 << 0.88 << 0.88 << 0.94;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4;
            SAStdvNonstrAccVector << 0.73 << 0.68 << 0.68 << 0.68;

            PGABarVector << 0.26 << 0.55 << 1.28 << 2.01;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 1;
            } else if (magnitude <= 7.5) {
                kappa = 0.8;
            } else {
                kappa = 0.5;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.36;
            Ay = 0.3;
            Du = 6.48;
            Au = 0.9;

            SDBarStrVector << 0.5 << 1.25 << 3.86 << 9.45;
            SDStdvStrVector << 0.84 << 0.86 << 0.89 << 1.04;

            SDBarNonstrDriftVector << 0.5 << 1.01 << 3.15 << 6.3 ;
            SDStdvNonstrDriftVector << 0.89 << 0.91 << 0.9 << 1.04 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.73 << 0.68 << 0.67 << 0.64 ;

            PGABarVector << 0.24 << 0.43 << 0.91 << 1.34;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.24;
            Ay = 0.2;
            Du = 4.32;
            Au = 0.6;

            SDBarStrVector << 0.5 << 1.25 << 3.86 << 9.45;
            SDStdvStrVector << 0.93 << 0.98 << 1.02 << 0.99;

            SDBarNonstrDriftVector << 0.5 << 1.01 << 3.15 << 6.3 ;
            SDStdvNonstrDriftVector << 0.98 << 0.99 << 1.02 << 1.09 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.71 << 0.68 << 0.66 << 0.66 ;

            PGABarVector << 0.20 << 0.34 << 0.61 << 0.95;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.24;
            Ay = 0.2;
            Du = 4.32;
            Au = 0.6;

            SDBarStrVector << 0.4 << 1 << 3.09 << 7.56;
            SDStdvStrVector << 1.01 << 1.05 << 1.07 << 1.06;

            SDBarNonstrDriftVector << 0.5 << 1.01 << 3.15 << 6.3 ;
            SDStdvNonstrDriftVector << 1.07 << 1.11 << 1.11 << 1.14 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.72 << 0.7 << 0.67 << 0.67 ;

            PGABarVector << 0.18 << 0.29 << 0.51 << 0.77;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        } else {
            qCritical() << this->objectName() << "could not identify code level for" << theCodeDesignLevel;
        }
    }

    else if (theBuildingType == QString("W2")) {
        betaE = 0.125;
        period = 0.4;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.63;
            Ay = 0.4;
            Du = 12.53;
            Au = 1;

            SDBarStrVector << 0.86 << 2.59 << 8.64 << 21.6;
            SDStdvStrVector << 0.81 << 0.88 << 0.9 << 0.83;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.87 << 0.89 << 0.96 << 0.94 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.7 << 0.67 << 0.67 << 0.68 ;

            PGABarVector << 0.26 << 0.56 << 1.15 << 2.08;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.31;
            Ay = 0.2;
            Du = 4.7;
            Au = 0.5;

            SDBarStrVector << 0.86 << 2.14 << 6.62 << 16.2;
            SDStdvStrVector << 0.89 << 0.95 << 0.95 << 0.92;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.94 << 0.99 << 1 << 0.9 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.68 << 0.67 << 0.68 << 0.68 ;

            PGABarVector << 0.20 << 0.35 << 0.64 << 1.13;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.16;
            Ay = 0.1;
            Du = 2.35;
            Au = 0.25;

            SDBarStrVector << 0.86 << 2.14 << 6.62 << 16.2;
            SDStdvStrVector << 0.97 << 0.9 << 0.89 << 0.99;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1.01 << 0.97 << 0.93 << 1.03 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.67 << 0.67 << 0.7 << 0.7 ;

            PGABarVector << 0.14 << 0.23 << 0.48 << 0.75;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.16;
            Ay = 0.1;
            Du = 2.35;
            Au = 0.25;

            SDBarStrVector << 0.69 << 1.71 << 5.29 << 12.96;
            SDStdvStrVector << 1.04 << 0.97 << 0.9 << 0.99;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1.06 << 1 << 0.93 << 1.01 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.67 << 0.65 << 0.65 ;

            PGABarVector << 0.12 << 0.19 << 0.37 << 0.60;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S1L")) {
        betaE = 0.06;
        period = 0.5;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.61;
            Ay = 0.25;
            Du = 14.67;
            Au = 0.749;

            SDBarStrVector << 1.3 << 2.59 << 6.48 << 17.28;
            SDStdvStrVector << 0.8 << 0.76 << 0.69 << 0.72;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.81 << 0.85 << 0.77 << 0.77 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.67 << 0.67 << 0.68 << 0.67 ;

            PGABarVector << 0.19 << 0.31 << 0.64 << 1.49;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.31;
            Ay = 0.125;
            Du = 5.5;
            Au = 0.375;

            SDBarStrVector << 1.3 << 2.24 << 5.08 << 12.96;
            SDStdvStrVector << 0.8 << 0.75 << 0.74 << 0.88;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.84 << 0.83 << 0.79 << 0.87 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.67 << 0.66 << 0.67 << 0.67 ;

            PGABarVector << 0.15 << 0.22 << 0.42 << 0.80;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;


            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.15;
            Ay = 0.062;
            Du = 2.29;
            Au = 0.187;

            SDBarStrVector << 1.3 << 2.07 << 4.38 << 10.8;
            SDStdvStrVector << 0.77 << 0.78 << 0.78 << 0.96;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.86 << 0.84 << 0.88 << 1 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.12 << 0.17 << 0.30 << 0.48;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.15;
            Ay = 0.062;
            Du = 2.75;
            Au = 0.187;

            SDBarStrVector << 1.04 << 1.65 << 3.5 << 8.64;
            SDStdvStrVector << 0.85 << 0.82 << 0.8 << 0.95;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.9 << 0.87 << 0.91 << 1.02 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.09 << 0.13 << 0.22 << 0.38;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S1M")) {
        betaE = 0.06;
        period = 1.08;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 1.78;
            Ay = 0.156;
            Du = 28.4;
            Au = 0.468;

            SDBarStrVector << 2.16 << 4.32 << 10.8 << 28.8;
            SDStdvStrVector << 0.65 << 0.66 << 0.67 << 0.74;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.67 << 0.68 << 0.67 << 0.67 ;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.71 << 0.72 << 0.72 << 0.8 ;

            PGABarVector << 0.14 << 0.26 << 0.62 << 1.43;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.89;
            Ay = 0.078;
            Du = 10.65;
            Au = 0.234;

            SDBarStrVector << 2.16 << 3.74 << 8.46 << 21.6;
            SDStdvStrVector << 0.65 << 0.68 << 0.69 << 0.87;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.71 << 0.74 << 0.85 << 0.95 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.13 << 0.21 << 0.44 << 0.82;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.44;
            Ay = 0.039;
            Du = 4.44;
            Au = 0.117;

            SDBarStrVector << 2.16 << 3.44 << 7.3 << 18;
            SDStdvStrVector << 0.68 << 0.78 << 0.85 << 0.98;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.74 << 0.89 << 0.99 << 1.05 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.12 << 0.18 << 0.29 << 0.49;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.44;
            Ay = 0.039;
            Du = 5.33;
            Au = 0.117;

            SDBarStrVector << 1.73 << 2.76 << 5.84 << 14.4;
            SDStdvStrVector << 0.7 << 0.75 << 0.81 << 0.98;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.8 << 0.92 << 0.99 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.09 << 0.14 << 0.23 << 0.39;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S1H")) {
        betaE = 0.06;
        period = 2.21;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 4.66;
            Ay = 0.098;
            Du = 55.88;
            Au = 0.293;

            SDBarStrVector << 3.37 << 6.74 << 16.85 << 44.93;
            SDStdvStrVector << 0.64 << 0.64 << 0.65 << 0.67;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.72 << 0.71 << 0.74 << 0.77 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.10 << 0.21 << 0.52 << 1.31;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 2.33;
            Ay = 0.049;
            Du = 20.96;
            Au = 0.147;

            SDBarStrVector << 3.37 << 5.83 << 13.21 << 33.7;
            SDStdvStrVector << 0.64 << 0.64 << 0.71 << 0.83;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.71 << 0.74 << 0.84 << 0.95 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.10 << 0.18 << 0.39 << 0.78;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 1.16;
            Ay = 0.024;
            Du = 8.73;
            Au = 0.073;

            SDBarStrVector << 3.37 << 5.37 << 11.38 << 28.08;
            SDStdvStrVector << 0.66 << 0.7 << 0.76 << 0.92;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.75 << 0.87 << 0.97 << 1.04 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.67 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.10 << 0.15 << 0.28 << 0.48;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 1.16;
            Ay = 0.024;
            Du = 10.48;
            Au = 0.073;

            SDBarStrVector << 2.7 << 4.3 << 9.11 << 22.46;
            SDStdvStrVector << 0.69 << 0.71 << 0.85 << 0.93;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.79 << 0.89 << 1 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.68 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.08 << 0.12 << 0.22 << 0.38;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S2L")) {
        betaE = 0.06;
        period = 0.4;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.63;
            Ay = 0.4;
            Du = 10.02;
            Au = 0.8;

            SDBarStrVector << 1.08 << 2.16 << 6.48 << 17.28;
            SDStdvStrVector << 0.81 << 0.89 << 0.94 << 0.83;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.84 << 0.9 << 0.97 << 0.92 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.67 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.24 << 0.41 << 0.76 << 1.46;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.31;
            Ay = 0.2;
            Du = 3.76;
            Au = 0.4;

            SDBarStrVector << 1.08 << 1.87 << 5.04 << 12.96;
            SDStdvStrVector << 0.93 << 0.92 << 0.93 << 0.93;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.93 << 0.99 << 0.96 << 0.92 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.66 << 0.68 << 0.68 ;

            PGABarVector << 0.20 << 0.26 << 0.46 << 0.84;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.16;
            Ay = 0.1;
            Du = 1.57;
            Au = 0.2;

            SDBarStrVector << 1.08 << 1.73 << 4.32 << 10.8;
            SDStdvStrVector << 0.96 << 0.89 << 0.86 << 0.98;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1.01 << 0.94 << 0.94 << 1.03 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.13 << 0.17 << 0.30 << 0.50;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.16;
            Ay = 0.1;
            Du = 1.88;
            Au = 0.2;

            SDBarStrVector << 0.86 << 1.38 << 3.46 << 8.64;
            SDStdvStrVector << 1.01 << 0.96 << 0.88 << 0.98;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1.06 << 0.97 << 0.96 << 1.04 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.11 << 0.14 << 0.23 << 0.39;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S2M")) {
        betaE = 0.06;
        period = 0.86;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 2.43;
            Ay = 0.333;
            Du = 25.88;
            Au = 0.667;

            SDBarStrVector << 1.8 << 3.6 << 10.8 << 28.8;
            SDStdvStrVector << 0.67 << 0.67 << 0.68 << 0.79;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.71 << 0.74 << 0.74 << 0.84 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.69 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.14 << 0.27 << 0.73 << 1.62;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 1.21;
            Ay = 0.167;
            Du = 9.7;
            Au = 0.333;

            SDBarStrVector << 1.8 << 3.12 << 8.4 << 21.6;
            SDStdvStrVector << 0.7 << 0.69 << 0.69 << 0.89;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.74 << 0.74 << 0.85 << 0.96 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.14 << 0.22 << 0.53 << 0.97;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.61;
            Ay = 0.083;
            Du = 4.04;
            Au = 0.167;

            SDBarStrVector << 1.8 << 2.88 << 7.2 << 18;
            SDStdvStrVector << 0.7 << 0.73 << 0.85 << 0.98;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.77 << 0.87 << 0.99 << 1.05 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.12 << 0.18 << 0.35 << 0.58;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.61;
            Ay = 0.083;
            Du = 4.85;
            Au = 0.167;

            SDBarStrVector << 1.44 << 2.3 << 5.76 << 14.4;
            SDStdvStrVector << 0.73 << 0.75 << 0.8 << 0.98;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.8 << 0.9 << 1.02 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.10 << 0.14 << 0.28 << 0.47;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S2H")) {
        betaE = 0.06;
        period = 1.77;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 7.75;
            Ay = 0.254;
            Du = 61.97;
            Au = 0.508;

            SDBarStrVector << 2.81 << 5.62 << 16.85 << 44.93;
            SDStdvStrVector << 0.63 << 0.63 << 0.64 << 0.71;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.71 << 0.71 << 0.72 << 0.78 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.11 << 0.22 << 0.65 << 1.60;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 3.87;
            Ay = 0.127;
            Du = 23.24;
            Au = 0.254;

            SDBarStrVector << 2.81 << 4.87 << 13.1 << 33.7;
            SDStdvStrVector << 0.66 << 0.64 << 0.69 << 0.8;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.72 << 0.73 << 0.8 << 0.94 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.65 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.11 << 0.19 << 0.49 << 1.02;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 1.94;
            Ay = 0.063;
            Du = 9.68;
            Au = 0.127;

            SDBarStrVector << 2.81 << 4.49 << 11.23 << 28.08;
            SDStdvStrVector << 0.66 << 0.67 << 0.74 << 0.92;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.74 << 0.86 << 0.97 << 1.05 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.11 << 0.17 << 0.36 << 0.63;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 1.94;
            Ay = 0.063;
            Du = 11.62;
            Au = 0.127;

            SDBarStrVector << 2.25 << 3.59 << 8.99 << 22.46;
            SDStdvStrVector << 0.7 << 0.7 << 0.84 << 0.91;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.79 << 0.89 << 0.99 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.09 << 0.13 << 0.29 << 0.50;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S3") | theBuildingType == QString("S3L")) {
        betaE = 0.06;
        period = 0.4;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.63;
            Ay = 0.4;
            Du = 10.02;
            Au = 0.8;

            SDBarStrVector << 0.54 << 1.08 << 3.24 << 9.45;
            SDStdvStrVector << 0.81 << 0.82 << 0.91 << 0.9;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 0.86 << 0.88 << 0.98 << 0.98 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.15 << 0.26 << 0.54 << 1.00;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.31;
            Ay = 0.2;
            Du = 3.76;
            Au = 0.4;

            SDBarStrVector << 0.54 << 0.94 << 2.52 << 7.09;
            SDStdvStrVector << 0.88 << 0.92 << 0.97 << 0.89;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 0.93 << 0.98 << 1.01 << 0.94 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.67 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.13 << 0.19 << 0.33 << 0.60;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.16;
            Ay = 0.1;
            Du = 1.57;
            Au = 0.2;

            SDBarStrVector << 0.54 << 0.87 << 2.17 << 5.91;
            SDStdvStrVector << 0.98 << 0.99 << 1.01 << 0.9;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 1.03 << 1.02 << 0.96 << 0.99 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.10 << 0.13 << 0.20 << 0.38;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.16;
            Ay = 0.1;
            Du = 1.88;
            Au = 0.2;

            SDBarStrVector << 0.43 << 0.69 << 1.73 << 4.73;
            SDStdvStrVector << 1.06 << 1.03 << 1.07 << 0.89;


            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 1.11 << 1.05 << 0.96 << 1 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;



            PGABarVector << 0.08 << 0.10 << 0.16 << 0.30;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S4L")) {
        betaE = 0.06;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.38;
            Ay = 0.32;
            Du = 6.91;
            Au = 0.72;

            SDBarStrVector << 0.86 << 1.73 << 5.18 << 15.12;
            SDStdvStrVector << 0.89 << 0.89 << 0.98 << 0.87;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 0.93 << 0.94 << 1.01 << 0.99 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.68 << 0.67 << 0.67 ;

            PGABarVector << 0.24 << 0.39 << 0.71 << 1.33;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.19;
            Ay = 0.16;
            Du = 2.59;
            Au = 0.36;

            SDBarStrVector << 0.86 << 1.5 << 4.04 << 11.34;
            SDStdvStrVector << 0.96 << 1 << 1.03 << 0.92;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1 << 1.06 << 0.99 << 0.96 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.19 << 0.26 << 0.41 << 0.78;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.1;
            Ay = 0.08;
            Du = 1.08;
            Au = 0.18;

            SDBarStrVector << 0.86 << 1.38 << 3.47 << 9.45;
            SDStdvStrVector << 1.05 << 0.98 << 0.89 << 0.98;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1.09 << 0.99 << 0.96 << 1.03 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.13 << 0.16 << 0.26 << 0.46;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.1;
            Ay = 0.08;
            Du = 1.3;
            Au = 0.18;

            SDBarStrVector << 0.69 << 1.11 << 2.77 << 7.56;
            SDStdvStrVector << 1.11 << 1.03 << 0.99 << 0.98;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1.12 << 1 << 0.99 << 1.05 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.68 << 0.68 << 0.68 ;


            PGABarVector << 0.10 << 0.13 << 0.20 << 0.36;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S4M")) {
        betaE = 0.06;
        period = 0.65;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 1.09;
            Ay = 0.267;
            Du = 13.1;
            Au = 0.6;

            SDBarStrVector << 1.44 << 2.88 << 8.64 << 25.2;
            SDStdvStrVector << 0.77 << 0.72 << 0.7 << 0.89;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.8 << 0.76 << 0.76 << 0.93 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.67 << 0.65 << 0.66 << 0.66 ;

            PGABarVector << 0.16 << 0.28 << 0.73 << 1.56;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.55;
            Ay = 0.133;
            Du = 4.91;
            Au = 0.3;

            SDBarStrVector << 1.44 << 2.5 << 6.73 << 18.9;
            SDStdvStrVector << 0.75 << 0.72 << 0.72 << 0.94;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.77 << 0.8 << 0.95 << 1.04 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.65 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.14 << 0.22 << 0.51 << 0.92;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.27;
            Ay = 0.067;
            Du = 2.05;
            Au = 0.15;

            SDBarStrVector << 1.44 << 2.31 << 5.78 << 15.75;
            SDStdvStrVector << 0.76 << 0.78 << 0.9 << 0.99;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.83 << 0.95 << 1.04 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.12 << 0.17 << 0.31 << 0.54;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.27;
            Ay = 0.067;
            Du = 2.46;
            Au = 0.15;

            SDBarStrVector << 1.15 << 1.85 << 4.62 << 12.6;
            SDStdvStrVector << 0.81 << 0.8 << 0.94 << 1;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.86 << 0.99 << 1.06 << 1.1 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.09 << 0.13 << 0.25 << 0.43;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S4H")) {
        betaE = 0.06;
        period = 1.32;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 3.49;
            Ay = 0.203;
            Du = 31.37;
            Au = 0.457;

            SDBarStrVector << 2.25 << 4.49 << 13.48 << 39.31;
            SDStdvStrVector << 0.64 << 0.66 << 0.69 << 0.77;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.72 << 0.72 << 0.79 << 0.91 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.67 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.13 << 0.25 << 0.69 << 1.63;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 1.74;
            Ay = 0.102;
            Du = 11.76;
            Au = 0.228;

            SDBarStrVector << 2.25 << 3.9 << 10.5 << 29.48;
            SDStdvStrVector << 0.66 << 0.67 << 0.7 << 0.9;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.73 << 0.82 << 0.93 << 1.01 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.65 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.12 << 0.21 << 0.51 << 0.97;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.87;
            Ay = 0.051;
            Du = 4.9;
            Au = 0.114;

            SDBarStrVector << 2.25 << 3.6 << 9.01 << 24.57;
            SDStdvStrVector << 0.7 << 0.75 << 0.9 << 0.98;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.84 << 0.95 << 1.05 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.12 << 0.17 << 0.33 << 0.59;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;


            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.87;
            Ay = 0.051;
            Du = 5.88;
            Au = 0.114;

            SDBarStrVector << 1.8 << 2.88 << 7.21 << 19.66;
            SDStdvStrVector << 0.73 << 0.75 << 0.9 << 0.97;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.88 << 0.99 << 1.07 << 1.09 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.09 << 0.14 << 0.27 << 0.47;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S5L")) {
        betaE = 0.06;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.12;
            Ay = 0.1;
            Du = 1.2;
            Au = 0.2;

            SDBarStrVector << 0.65 << 1.3 << 3.24 << 7.56;
            SDStdvStrVector << 1.11 << 1.04 << 0.99 << 0.95;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1.14 << 1.04 << 0.98 << 1.01 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.67 << 0.67 ;

            PGABarVector << 0.13 << 0.17 << 0.28 << 0.45;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.12;
            Ay = 0.1;
            Du = 1.2;
            Au = 0.2;

            SDBarStrVector << 0.52 << 1.04 << 2.59 << 6.05;
            SDStdvStrVector << 1.2 << 1.11 << 1.08 << 0.95;

            SDBarNonstrDriftVector << 0.86 << 1.73 << 5.4 << 10.8 ;
            SDStdvNonstrDriftVector << 1.18 << 1.06 << 0.98 << 1.03 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.11 << 0.14 << 0.22 << 0.37;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S5M")) {
        betaE = 0.06;
        period = 0.65;
        if (theCodeDesignLevel == QString("High")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.34;
            Ay = 0.083;
            Du = 2.27;
            Au = 0.167;

            SDBarStrVector << 1.08 << 2.16 << 5.4 << 12.6;
            SDStdvStrVector << 0.77 << 0.79 << 0.87 << 0.98;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.84 << 0.95 << 1.03 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.68 << 0.67 << 0.67 ;

            PGABarVector << 0.11 << 0.18 << 0.34 << 0.53;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.34;
            Ay = 0.083;
            Du = 2.27;
            Au = 0.167;

            SDBarStrVector << 0.86 << 1.73 << 4.32 << 10.08;
            SDStdvStrVector << 0.85 << 0.83 << 0.94 << 0.99;

            SDBarNonstrDriftVector << 2.16 << 4.32 << 13.5 << 27 ;
            SDStdvNonstrDriftVector << 0.86 << 0.99 << 1.05 << 1.09 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.09 << 0.14 << 0.28 << 0.43;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("S5H")) {
        betaE = 0.06;
        period = 1.32;
        if (theCodeDesignLevel == QString("High")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 1.09;
            Ay = 0.063;
            Du = 5.45;
            Au = 0.127;

            SDBarStrVector << 1.68 << 3.37 << 8.42 << 19.66;
            SDStdvStrVector << 0.7 << 0.73 << 0.89 << 0.97;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.84 << 0.95 << 1.03 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.10 << 0.18 << 0.35 << 0.58;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 1.09;
            Ay = 0.063;
            Du = 5.45;
            Au = 0.127;

            SDBarStrVector << 1.35 << 2.7 << 6.74 << 15.72;
            SDStdvStrVector << 0.72 << 0.75 << 0.92 << 0.96;

            SDBarNonstrDriftVector << 4.49 << 8.99 << 28.08 << 56.16 ;
            SDStdvNonstrDriftVector << 0.88 << 0.91 << 1.05 << 1.09 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.08 << 0.14 << 0.29 << 0.46;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C1L")) {
        betaE = 0.07;
        period = 0.4;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.39;
            Ay = 0.25;
            Du = 9.39;
            Au = 0.749;

            SDBarStrVector << 0.9 << 1.8 << 5.4 << 14.4;
            SDStdvStrVector << 0.81 << 0.84 << 0.86 << 0.81;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.84 << 0.88 << 0.9 << 0.88 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.68 << 0.67 << 0.67 ;

            PGABarVector << 0.21 << 0.35 << 0.70 << 1.37;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.2;
            Ay = 0.125;
            Du = 3.52;
            Au = 0.375;

            SDBarStrVector << 0.9 << 1.56 << 4.2 << 10.8;
            SDStdvStrVector << 0.89 << 0.9 << 0.9 << 0.89;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.93 << 0.96 << 0.94 << 0.88 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.67 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.16 << 0.23 << 0.41 << 0.77;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.1;
            Ay = 0.062;
            Du = 1.47;
            Au = 0.187;

            SDBarStrVector << 0.9 << 1.44 << 3.6 << 9;
            SDStdvStrVector << 0.95 << 0.91 << 0.85 << 0.97;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.99 << 0.96 << 0.9 << 1.01 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.12 << 0.15 << 0.27 << 0.45;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.1;
            Ay = 0.062;
            Du = 1.76;
            Au = 0.187;

            SDBarStrVector << 0.72 << 1.15 << 2.88 << 7.2;
            SDStdvStrVector << 0.98 << 0.94 << 0.9 << 0.97;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.02 << 0.98 << 0.93 << 1.03 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.10 << 0.12 << 0.21 << 0.36;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C1M")) {
        betaE = 0.07;
        period = 0.75;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 1.15;
            Ay = 0.208;
            Du = 18.44;
            Au = 0.624;

            SDBarStrVector << 1.5 << 3 << 9 << 24;
            SDStdvStrVector << 0.68 << 0.67 << 0.68 << 0.81;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.72 << 0.73 << 0.74 << 0.84 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.68 << 0.66 << 0.66 ;

            PGABarVector << 0.15 << 0.27 << 0.73 << 1.61;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.58;
            Ay = 0.104;
            Du = 6.91;
            Au = 0.312;

            SDBarStrVector << 1.5 << 2.6 << 7 << 18;
            SDStdvStrVector << 0.7 << 0.7 << 0.7 << 0.89;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.77 << 0.76 << 0.87 << 0.98 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.65 << 0.63 << 0.63 ;

            PGABarVector << 0.13 << 0.21 << 0.49 << 0.89;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.29;
            Ay = 0.052;
            Du = 2.88;
            Au = 0.156;

            SDBarStrVector << 1.5 << 2.4 << 6 << 15;
            SDStdvStrVector << 0.7 << 0.74 << 0.86 << 0.98;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.79 << 0.88 << 0.99 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.12 << 0.17 << 0.32 << 0.54;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.29;
            Ay = 0.052;
            Du = 3.46;
            Au = 0.156;

            SDBarStrVector << 1.2 << 1.92 << 4.8 << 12;
            SDStdvStrVector << 0.73 << 0.77 << 0.83 << 0.98;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.81 << 0.91 << 1.02 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.09 << 0.13 << 0.26 << 0.43;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C1H")) {
        betaE = 0.07;
        period = 1.45;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 2.01;
            Ay = 0.098;
            Du = 24.13;
            Au = 0.293;

            SDBarStrVector << 2.16 << 4.32 << 12.96 << 34.56;
            SDStdvStrVector << 0.66 << 0.64 << 0.67 << 0.78;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.71 << 0.71 << 0.78 << 0.88 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.66 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.11 << 0.22 << 0.62 << 1.35;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 1.01;
            Ay = 0.049;
            Du = 9.05;
            Au = 0.147;

            SDBarStrVector << 2.16 << 3.74 << 10.08 << 25.92;
            SDStdvStrVector << 0.66 << 0.66 << 0.76 << 0.91;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.74 << 0.8 << 0.94 << 1.03 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.11 << 0.18 << 0.41 << 0.74;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.5;
            Ay = 0.024;
            Du = 3.77;
            Au = 0.073;

            SDBarStrVector << 2.16 << 3.46 << 8.64 << 21.6;
            SDStdvStrVector << 0.7 << 0.81 << 0.89 << 0.98;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.87 << 0.96 << 1.02 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.67 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.10 << 0.15 << 0.27 << 0.44;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.5;
            Ay = 0.024;
            Du = 4.52;
            Au = 0.073;

            SDBarStrVector << 1.73 << 2.76 << 6.91 << 17.28;
            SDStdvStrVector << 0.71 << 0.8 << 0.94 << 1.01;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.9 << 0.99 << 1.05 << 1.1 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.68 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.08 << 0.12 << 0.21 << 0.35;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C2L")) {
        betaE = 0.07;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.48;
            Ay = 0.4;
            Du = 9.59;
            Au = 1;

            SDBarStrVector << 0.72 << 1.8 << 5.4 << 14.4;
            SDStdvStrVector << 0.81 << 0.84 << 0.93 << 0.92;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.87 << 0.88 << 0.97 << 0.99 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.69 << 0.67 << 0.66 << 0.64 ;

            PGABarVector << 0.24 << 0.45 << 0.90 << 1.55;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.24;
            Ay = 0.2;
            Du = 3.6;
            Au = 0.5;

            SDBarStrVector << 0.72 << 1.52 << 4.17 << 10.8;
            SDStdvStrVector << 0.91 << 0.97 << 1.03 << 0.87;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.96 << 1 << 1.06 << 0.95 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.68 << 0.66 << 0.68 << 0.68 ;

            PGABarVector << 0.18 << 0.30 << 0.49 << 0.87;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.12;
            Ay = 0.1;
            Du = 1.5;
            Au = 0.25;

            SDBarStrVector << 0.72 << 1.37 << 3.55 << 9;
            SDStdvStrVector << 1.04 << 1.02 << 0.99 << 0.95;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.08 << 1.05 << 0.95 << 0.99 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.67 << 0.66 << 0.66 ;

            PGABarVector << 0.14 << 0.19 << 0.30 << 0.52;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.12;
            Ay = 0.1;
            Du = 1.8;
            Au = 0.25;

            SDBarStrVector << 0.58 << 1.1 << 2.84 << 7.2;
            SDStdvStrVector << 1.11 << 1.09 << 1.07 << 0.93;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.14 << 1.08 << 0.97 << 1 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.11 << 0.15 << 0.24 << 0.42;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C2M")) {
        betaE = 0.07;
        period = 0.56;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 1.04;
            Ay = 0.333;
            Du = 13.84;
            Au = 0.833;

            SDBarStrVector << 1.2 << 3 << 9 << 24;
            SDStdvStrVector << 0.74 << 0.77 << 0.68 << 0.77;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.84 << 0.82 << 0.74 << 0.81 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.7 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.17 << 0.36 << 0.87 << 1.95;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.52;
            Ay = 0.167;
            Du = 5.19;
            Au = 0.417;

            SDBarStrVector << 1.2 << 2.53 << 6.95 << 18;
            SDStdvStrVector << 0.81 << 0.77 << 0.73 << 0.91;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.84 << 0.81 << 0.83 << 0.98 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.67 << 0.64 << 0.67 << 0.67 ;

            PGABarVector << 0.15 << 0.26 << 0.55 << 1.02;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.26;
            Ay = 0.083;
            Du = 2.16;
            Au = 0.208;

            SDBarStrVector << 1.2 << 2.29 << 5.92 << 15;
            SDStdvStrVector << 0.82 << 0.81 << 0.81 << 0.99;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.84 << 0.87 << 1 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.12 << 0.19 << 0.38 << 0.63;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.26;
            Ay = 0.083;
            Du = 2.6;
            Au = 0.208;

            SDBarStrVector << 0.96 << 1.83 << 4.74 << 12;
            SDStdvStrVector << 0.86 << 0.83 << 0.8 << 0.98;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.88 << 0.9 << 1.03 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.10 << 0.15 << 0.30 << 0.50;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C2H")) {
        betaE = 0.07;
        period = 1.09;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 2.94;
            Ay = 0.254;
            Du = 29.39;
            Au = 0.635;

            SDBarStrVector << 1.73 << 4.32 << 12.96 << 34.56;
            SDStdvStrVector << 0.68 << 0.65 << 0.66 << 0.75;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.71 << 0.72 << 0.74 << 0.85 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.12 << 0.29 << 0.82 << 1.87;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 1.47;
            Ay = 0.127;
            Du = 11.02;
            Au = 0.317;

            SDBarStrVector << 1.73 << 3.64 << 10 << 25.92;
            SDStdvStrVector << 0.66 << 0.68 << 0.7 << 0.87;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.73 << 0.76 << 0.89 << 0.99 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.12 << 0.23 << 0.57 << 1.07;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.74;
            Ay = 0.063;
            Du = 4.59;
            Au = 0.159;

            SDBarStrVector << 1.73 << 3.3 << 8.53 << 21.6;
            SDStdvStrVector << 0.68 << 0.73 << 0.84 << 0.95;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.79 << 0.93 << 0.99 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.11 << 0.19 << 0.38 << 0.65;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.74;
            Ay = 0.063;
            Du = 5.51;
            Au = 0.159;

            SDBarStrVector << 1.38 << 2.64 << 6.82 << 17.28;
            SDStdvStrVector << 0.73 << 0.75 << 0.92 << 0.97;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.83 << 0.97 << 1.05 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.09 << 0.15 << 0.31 << 0.52;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C3L")) {
        betaE = 0.07;
        period = 0.35;

        if (theCodeDesignLevel == QString("High")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.12;
            Ay = 0.1;
            Du = 1.35;
            Au = 0.225;

            SDBarStrVector << 0.54 << 1.08 << 2.7 << 6.3;
            SDStdvStrVector << 1.09 << 1.07 << 1.08 << 0.91;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.13 << 1.08 << 0.95 << 1 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.66 << 0.66 ;

            PGABarVector << 0.12 << 0.17 << 0.26 << 0.44;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.12;
            Ay = 0.1;
            Du = 1.35;
            Au = 0.225;

            SDBarStrVector << 0.43 << 0.86 << 2.16 << 5.04;
            SDStdvStrVector << 1.19 << 1.15 << 1.15 << 0.92;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.19 << 1.11 << 0.99 << 1.02 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.10 << 0.14 << 0.21 << 0.35;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C3M")) {
        betaE = 0.07;
        period = 0.56;
        if (theCodeDesignLevel == QString("High")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.26;
            Ay = 0.083;
            Du = 1.95;
            Au = 0.188;

            SDBarStrVector << 0.9 << 1.8 << 4.5 << 10.5;
            SDStdvStrVector << 0.85 << 0.83 << 0.79 << 0.98;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.88 << 0.92 << 1 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.67 << 0.66 << 0.66 ;

            PGABarVector << 0.11 << 0.17 << 0.32 << 0.51;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.26;
            Ay = 0.083;
            Du = 1.95;
            Au = 0.188;

            SDBarStrVector << 0.72 << 1.44 << 3.6 << 8.4;
            SDStdvStrVector << 0.9 << 0.86 << 0.9 << 0.96;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.92 << 0.95 << 1.03 << 1.09 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.09 << 0.14 << 0.25 << 0.41;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("C3H")) {
        betaE = 0.07;
        period = 1.09;
        if (theCodeDesignLevel == QString("High")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.74;
            Ay = 0.063;
            Du = 4.13;
            Au = 0.143;

            SDBarStrVector << 1.3 << 2.59 << 6.48 << 15.12;
            SDStdvStrVector << 0.71 << 0.74 << 0.9 << 0.97;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.83 << 0.96 << 1.02 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.09 << 0.16 << 0.33 << 0.53;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.74;
            Ay = 0.063;
            Du = 4.13;
            Au = 0.143;

            SDBarStrVector << 1.04 << 2.07 << 5.18 << 12.1;
            SDStdvStrVector << 0.73 << 0.75 << 0.9 << 0.95;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.86 << 0.9 << 1.04 << 1.09 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.08 << 0.13 << 0.27 << 0.43;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("PC1")) {
        betaE = 0.07;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.72;
            Ay = 0.6;
            Du = 11.51;
            Au = 1.2;

            SDBarStrVector << 0.54 << 1.08 << 3.24 << 9.45;
            SDStdvStrVector << 0.76 << 0.86 << 0.88 << 0.99;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 0.82 << 0.91 << 0.95 << 1.03 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.74 << 0.67 << 0.67 << 0.64 ;

            PGABarVector << 0.20 << 0.35 << 0.72 << 1.25;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.36;
            Ay = 0.3;
            Du = 4.32;
            Au = 0.6;

            SDBarStrVector << 0.54 << 0.94 << 2.52 << 7.09;
            SDStdvStrVector << 0.89 << 0.92 << 0.97 << 1.04;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 0.94 << 0.99 << 1.05 << 1.08 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.68 << 0.67 << 0.66 << 0.66 ;

            PGABarVector << 0.18 << 0.24 << 0.44 << 0.71;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.18;
            Ay = 0.15;
            Du = 1.8;
            Au = 0.3;

            SDBarStrVector << 0.54 << 0.87 << 2.17 << 5.91;
            SDStdvStrVector << 1 << 1.05 << 1.12 << 0.89;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 1.05 << 1.1 << 1.1 << 0.93 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.13 << 0.17 << 0.25 << 0.45;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.18;
            Ay = 0.15;
            Du = 2.16;
            Au = 0.3;

            SDBarStrVector << 0.43 << 0.69 << 1.73 << 4.73;
            SDStdvStrVector << 1.14 << 1.14 << 1.17 << 0.98;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 1.18 << 1.16 << 1.12 << 0.95 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.67 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.11 << 0.14 << 0.21 << 0.35;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("PC2L")) {
        betaE = 0.07;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.48;
            Ay = 0.4;
            Du = 7.67;
            Au = 0.8;

            SDBarStrVector << 0.72 << 1.44 << 4.32 << 12.6;
            SDStdvStrVector << 0.84 << 0.88 << 0.98 << 0.94;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.89 << 0.93 << 1.03 << 1.04 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.24 << 0.36 << 0.69 << 1.23;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.24;
            Ay = 0.2;
            Du = 2.88;
            Au = 0.4;

            SDBarStrVector << 0.72 << 1.25 << 3.37 << 9.45;
            SDStdvStrVector << 0.96 << 1 << 1.03 << 0.88;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1 << 1.06 << 1.07 << 0.93 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.18 << 0.25 << 0.40 << 0.74;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.12;
            Ay = 0.1;
            Du = 1.2;
            Au = 0.2;

            SDBarStrVector << 0.72 << 1.15 << 2.89 << 7.88;
            SDStdvStrVector << 1.08 << 1.03 << 0.98 << 0.96;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.12 << 1.04 << 0.93 << 1.02 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.13 << 0.15 << 0.24 << 0.44;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.12;
            Ay = 0.1;
            Du = 1.44;
            Au = 0.2;

            SDBarStrVector << 0.58 << 0.92 << 2.31 << 6.3;
            SDStdvStrVector << 1.14 << 1.1 << 1.1 << 0.93;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.16 << 1.06 << 0.96 << 1.02 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.10 << 0.13 << 0.19 << 0.35;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("PC2M")) {
        betaE = 0.07;
        period = 0.56;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 1.04;
            Ay = 0.333;
            Du = 11.07;
            Au = 0.667;

            SDBarStrVector << 1.2 << 2.4 << 7.2 << 21;
            SDStdvStrVector << 0.77 << 0.81 << 0.7 << 0.82;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.87 << 0.83 << 0.77 << 0.89 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.68 << 0.65 << 0.66 << 0.66 ;

            PGABarVector << 0.17 << 0.29 << 0.67 << 1.51;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.52;
            Ay = 0.167;
            Du = 4.15;
            Au = 0.333;

            SDBarStrVector << 1.2 << 2.08 << 5.61 << 15.75;
            SDStdvStrVector << 0.82 << 0.79 << 0.75 << 0.93;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.85 << 0.83 << 0.92 << 1 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.65 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.15 << 0.21 << 0.45 << 0.86;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.26;
            Ay = 0.083;
            Du = 1.73;
            Au = 0.167;

            SDBarStrVector << 1.2 << 1.92 << 4.81 << 13.12;
            SDStdvStrVector << 0.81 << 0.79 << 0.84 << 0.99;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.86 << 0.93 << 1.02 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.11 << 0.16 << 0.31 << 0.52;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.26;
            Ay = 0.083;
            Du = 2.08;
            Au = 0.167;

            SDBarStrVector << 0.96 << 1.54 << 3.85 << 10.5;
            SDStdvStrVector << 0.87 << 0.83 << 0.91 << 1;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.87 << 0.95 << 1.04 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.68 << 0.68 << 0.68 ;

            PGABarVector << 0.09 << 0.13 << 0.24 << 0.42;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("PC2H")) {
        betaE = 0.07;
        period = 1.09;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 2.94;
            Ay = 0.254;
            Du = 23.52;
            Au = 0.508;

            SDBarStrVector << 1.73 << 3.46 << 10.37 << 30.24;
            SDStdvStrVector << 0.64 << 0.66 << 0.68 << 0.81;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.73 << 0.73 << 0.77 << 0.89 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.67 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.12 << 0.23 << 0.63 << 1.49;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.7;
            } else if (magnitude <= 7.5) {
                kappa = 0.5;
            } else {
                kappa = 0.3;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 1.47;
            Ay = 0.127;
            Du = 8.82;
            Au = 0.254;

            SDBarStrVector << 1.73 << 3 << 8.08 << 22.68;
            SDStdvStrVector << 0.68 << 0.69 << 0.77 << 0.89;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.74 << 0.79 << 0.93 << 1.02 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.64 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.12 << 0.19 << 0.46 << 0.90;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.74;
            Ay = 0.063;
            Du = 3.67;
            Au = 0.127;

            SDBarStrVector << 1.73 << 2.77 << 6.93 << 18.9;
            SDStdvStrVector << 0.71 << 0.75 << 0.89 << 0.98;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.83 << 0.94 << 1.04 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.11 << 0.16 << 0.31 << 0.55;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.74;
            Ay = 0.063;
            Du = 4.41;
            Au = 0.127;

            SDBarStrVector << 1.38 << 2.21 << 5.55 << 15.12;
            SDStdvStrVector << 0.74 << 0.75 << 0.91 << 0.96;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.87 << 0.99 << 1.06 << 1.08 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.09 << 0.13 << 0.25 << 0.43;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("RM1L")) {
        betaE = 0.085;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.64;
            Ay = 0.533;
            Du = 10.23;
            Au = 1.066;

            SDBarStrVector << 0.72 << 1.44 << 4.32 << 12.6;
            SDStdvStrVector << 0.84 << 0.86 << 0.92 << 1.01;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.89 << 0.91 << 0.97 << 1.06 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.7 << 0.67 << 0.67 << 0.63 ;

            PGABarVector << 0.30 << 0.46 << 0.93 << 1.57;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.32;
            Ay = 0.267;
            Du = 3.84;
            Au = 0.533;

            SDBarStrVector << 0.72 << 1.25 << 3.37 << 9.45;
            SDStdvStrVector << 0.96 << 0.99 << 1.05 << 0.94;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1 << 1.06 << 1.12 << 1.01 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.68 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.22 << 0.30 << 0.50 << 0.85;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.16;
            Ay = 0.133;
            Du = 1.6;
            Au = 0.267;

            SDBarStrVector << 0.72 << 1.15 << 2.89 << 7.88;
            SDStdvStrVector << 1.11 << 1.1 << 1.1 << 0.92;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.15 << 1.12 << 1.03 << 0.99 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.67 << 0.64 << 0.64 ;

            PGABarVector << 0.16 << 0.20 << 0.29 << 0.54;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.16;
            Ay = 0.133;
            Du = 1.92;
            Au = 0.267;

            SDBarStrVector << 0.58 << 0.92 << 2.31 << 6.3;
            SDStdvStrVector << 1.2 << 1.17 << 1.17 << 0.94;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.22 << 1.14 << 1.03 << 0.99 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.67 << 0.66 << 0.66 ;

            PGABarVector << 0.13 << 0.16 << 0.24 << 0.43;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("RM1M")) {
        betaE = 0.085;
        period = 0.56;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 1.38;
            Ay = 0.444;
            Du = 14.76;
            Au = 0.889;

            SDBarStrVector << 1.2 << 2.4 << 7.2 << 21;
            SDStdvStrVector << 0.71 << 0.81 << 0.76 << 0.75;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.81 << 0.86 << 0.8 << 0.81 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.72 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.20 << 0.37 << 0.81 << 1.90;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.69;
            Ay = 0.222;
            Du = 5.54;
            Au = 0.444;

            SDBarStrVector << 1.2 << 2.08 << 5.61 << 15.75;
            SDStdvStrVector << 0.81 << 0.82 << 0.8 << 0.89;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.88 << 0.85 << 0.84 << 0.98 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.67 << 0.64 << 0.67 << 0.67 ;

            PGABarVector << 0.18 << 0.26 << 0.51 << 1.03;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.35;
            Ay = 0.111;
            Du = 2.31;
            Au = 0.222;

            SDBarStrVector << 1.2 << 1.92 << 4.81 << 13.12;
            SDStdvStrVector << 0.87 << 0.84 << 0.79 << 0.96;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.89 << 0.89 << 1 << 1.05 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.64 << 0.64 ;

            PGABarVector << 0.14 << 0.19 << 0.35 << 0.63;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.35;
            Ay = 0.111;
            Du = 2.77;
            Au = 0.222;

            SDBarStrVector << 0.96 << 1.54 << 3.85 << 10.5;
            SDStdvStrVector << 0.91 << 0.89 << 0.89 << 0.96;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.93 << 0.92 << 1.02 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.11 << 0.15 << 0.28 << 0.50;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("RM2L")) {
        betaE = 0.085;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.64;
            Ay = 0.533;
            Du = 10.23;
            Au = 1.066;

            SDBarStrVector << 0.72 << 1.44 << 4.32 << 12.6;
            SDStdvStrVector << 0.8 << 0.81 << 0.91 << 0.98;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.85 << 0.87 << 0.95 << 1.03 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.7 << 0.66 << 0.67 << 0.64 ;

            PGABarVector << 0.26 << 0.42 << 0.87 << 1.49;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.32;
            Ay = 0.267;
            Du = 3.84;
            Au = 0.533;

            SDBarStrVector << 0.72 << 1.25 << 3.37 << 9.45;
            SDStdvStrVector << 0.91 << 0.96 << 1.02 << 0.93;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 0.96 << 1.02 << 1.1 << 0.99 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.68 << 0.66 << 0.67 << 0.67 ;

            PGABarVector << 0.20 << 0.28 << 0.47 << 0.81;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.16;
            Ay = 0.133;
            Du = 1.6;
            Au = 0.267;

            SDBarStrVector << 0.72 << 1.15 << 2.89 << 7.88;
            SDStdvStrVector << 1.05 << 1.07 << 1.09 << 0.91;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.09 << 1.08 << 1.01 << 0.99 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.67 << 0.64 << 0.64 ;

            PGABarVector << 0.14 << 0.18 << 0.28 << 0.51;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.16;
            Ay = 0.133;
            Du = 1.92;
            Au = 0.267;

            SDBarStrVector << 0.58 << 0.92 << 2.31 << 6.3;
            SDStdvStrVector << 1.14 << 1.1 << 1.15 << 0.92;

            SDBarNonstrDriftVector << 0.72 << 1.44 << 4.5 << 9 ;
            SDStdvNonstrDriftVector << 1.17 << 1.12 << 1.01 << 0.99 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.66 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.12 << 0.15 << 0.22 << 0.41;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("RM2M")) {
        betaE = 0.085;
        period = 0.56;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 1.38;
            Ay = 0.444;
            Du = 14.76;
            Au = 0.889;

            SDBarStrVector << 1.2 << 2.4 << 7.2 << 21;
            SDStdvStrVector << 0.71 << 0.79 << 0.7 << 0.73;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.82 << 0.84 << 0.76 << 0.8 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.72 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.17 << 0.33 << 0.75 << 1.83;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.69;
            Ay = 0.222;
            Du = 5.54;
            Au = 0.444;

            SDBarStrVector << 1.2 << 2.08 << 5.61 << 15.75;
            SDStdvStrVector << 0.81 << 0.8 << 0.75 << 0.88;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.88 << 0.83 << 0.81 << 0.98 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.67 << 0.64 << 0.67 << 0.67 ;

            PGABarVector << 0.16 << 0.23 << 0.48 << 0.99;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.35;
            Ay = 0.111;
            Du = 2.31;
            Au = 0.222;

            SDBarStrVector << 1.2 << 1.92 << 4.81 << 13.12;
            SDStdvStrVector << 0.84 << 0.81 << 0.77 << 0.96;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.85 << 0.86 << 0.99 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.65 << 0.65 ;

            PGABarVector << 0.12 << 0.17 << 0.34 << 0.60;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.35;
            Ay = 0.111;
            Du = 2.77;
            Au = 0.222;

            SDBarStrVector << 0.96 << 1.54 << 3.85 << 10.5;
            SDStdvStrVector << 0.89 << 0.87 << 0.87 << 0.96;

            SDBarNonstrDriftVector << 1.8 << 3.6 << 11.25 << 22.5 ;
            SDStdvNonstrDriftVector << 0.89 << 0.9 << 1.01 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.10 << 0.14 << 0.26 << 0.47;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("RM2H")) {
        betaE = 0.085;
        period = 1.09;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 3.92;
            Ay = 0.338;
            Du = 31.35;
            Au = 0.677;

            SDBarStrVector << 1.73 << 3.46 << 10.37 << 30.24;
            SDStdvStrVector << 0.66 << 0.65 << 0.66 << 0.72;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.71 << 0.73 << 0.73 << 0.85 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.7 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.12 << 0.24 << 0.67 << 1.78;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.9;
            } else if (magnitude <= 7.5) {
                kappa = 0.6;
            } else {
                kappa = 0.4;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 1.96;
            Ay = 0.169;
            Du = 11.76;
            Au = 0.338;

            SDBarStrVector << 1.73 << 3 << 8.08 << 22.68;
            SDStdvStrVector << 0.67 << 0.69 << 0.7 << 0.86;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.73 << 0.76 << 0.88 << 0.99 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.66 << 0.64 << 0.64 << 0.64 ;

            PGABarVector << 0.12 << 0.20 << 0.48 << 1.01;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.98;
            Ay = 0.085;
            Du = 4.9;
            Au = 0.169;

            SDBarStrVector << 1.73 << 2.77 << 6.93 << 18.9;
            SDStdvStrVector << 0.69 << 0.72 << 0.87 << 0.96;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.79 << 0.92 << 0.99 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.11 << 0.17 << 0.35 << 0.62;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.98;
            Ay = 0.085;
            Du = 5.88;
            Au = 0.169;

            SDBarStrVector << 1.38 << 2.21 << 5.55 << 15.12;
            SDStdvStrVector << 0.75 << 0.75 << 0.84 << 0.94;

            SDBarNonstrDriftVector << 3.46 << 6.91 << 21.6 << 43.2 ;
            SDStdvNonstrDriftVector << 0.82 << 0.96 << 1.04 << 1.07 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.09 << 0.13 << 0.27 << 0.50;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("URML")) {
        betaE = 0.1;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Low") || theCodeDesignLevel == QString("LS")) {
            Dy = 0.24;
            Ay = 0.2;
            Du = 2.4;
            Au = 0.4;

            SDBarStrVector << 0.41 << 0.81 << 2.03 << 4.73;
            SDStdvStrVector << 0.99 << 1.05 << 1.1 << 1.08;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 1.07 << 1.13 << 1.16 << 1.01 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.68 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.14 << 0.20 << 0.32 << 0.46;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.24;
            Ay = 0.2;
            Du = 2.4;
            Au = 0.4;

            SDBarStrVector << 0.32 << 0.65 << 1.62 << 3.78;
            SDStdvStrVector << 1.15 << 1.19 << 1.2 << 1.18;

            SDBarNonstrDriftVector << 0.54 << 1.08 << 3.38 << 6.75 ;
            SDStdvNonstrDriftVector << 1.21 << 1.23 << 1.23 << 1.03 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.69 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.13 << 0.17 << 0.26 << 0.37;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("URMM")) {
        betaE = 0.1;
        period = 0.5;
        if (theCodeDesignLevel == QString("High")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            qCritical() << "Error in object" << objectName() << ": The building type" << theBuildingType << "does not include the code design level" << theCodeDesignLevel <<".";
            return -1;
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.27;
            Ay = 0.111;
            Du = 1.81;
            Au = 0.222;

            SDBarStrVector << 0.63 << 1.26 << 3.15 << 7.35;
            SDStdvStrVector << 0.91 << 0.92 << 0.87 << 0.91;

            SDBarNonstrDriftVector << 1.26 << 2.52 << 7.88 << 15.75 ;
            SDStdvNonstrDriftVector << 0.97 << 0.91 << 0.98 << 1.04 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.10 << 0.16 << 0.27<< 0.46;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.5;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.27;
            Ay = 0.111;
            Du = 1.81;
            Au = 0.222;

            SDBarStrVector << 0.5 << 1.01 << 2.52 << 5.88;
            SDStdvStrVector << 0.99 << 0.97 << 0.9 << 0.88;

            SDBarNonstrDriftVector << 1.26 << 2.52 << 7.88 << 15.75 ;
            SDStdvNonstrDriftVector << 0.99 << 0.95 << 0.99 << 1.06 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.64 << 0.66 << 0.66 << 0.66 ;

            PGABarVector << 0.09 << 0.13 << 0.21 << 0.38;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.4;
            } else if (magnitude <= 7.5) {
                kappa = 0.2;
            } else {
                kappa = 0;
            }
        }
    }

    else if (theBuildingType == QString("MH")) {
        betaE = 0.05;
        period = 0.35;
        if (theCodeDesignLevel == QString("High")) {
            Dy = 0.18;
            Ay = 0.15;
            Du = 2.16;
            Au = 0.3;

            SDBarStrVector << 0.48 << 0.96 << 2.88 << 8.4;
            SDStdvStrVector << 0.91 << 1 << 1.03 << 0.92;

            SDBarNonstrDriftVector << 0.48 << 0.96 << 3 << 6 ;
            SDStdvNonstrDriftVector << 0.96 << 1.05 << 1.07 << 0.93 ;

            SABarNonstrAccVector << 0.3 << 0.6 << 1.2 << 2.4 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.11 << 0.18 << 0.31 << 0.60;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Moderate")) {
            Dy = 0.18;
            Ay = 0.15;
            Du = 2.16;
            Au = 0.3;

            SDBarStrVector << 0.48 << 0.96 << 2.88 << 8.4;
            SDStdvStrVector << 0.91 << 1 << 1.03 << 0.92;

            SDBarNonstrDriftVector << 0.48 << 0.96 << 3 << 6 ;
            SDStdvNonstrDriftVector << 0.96 << 1.05 << 1.07 << 0.93 ;

            SABarNonstrAccVector << 0.25 << 0.5 << 1 << 2 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.11 << 0.18 << 0.31 << 0.60;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Low")) {
            Dy = 0.18;
            Ay = 0.15;
            Du = 2.16;
            Au = 0.3;

            SDBarStrVector << 0.48 << 0.96 << 2.88 << 8.4;
            SDStdvStrVector << 0.91 << 1 << 1.03 << 0.92;

            SDBarNonstrDriftVector << 0.48 << 0.96 << 3 << 6 ;
            SDStdvNonstrDriftVector << 0.96 << 1.05 << 1.07 << 0.93 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.65 << 0.67 << 0.67 << 0.67 ;

            PGABarVector << 0.11 << 0.18 << 0.31 << 0.60;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.8;
            } else if (magnitude <= 7.5) {
                kappa = 0.4;
            } else {
                kappa = 0.2;
            }
        }
        else if (theCodeDesignLevel == QString("Pre")) {
            Dy = 0.18;
            Ay = 0.15;
            Du = 2.16;
            Au = 0.3;

            SDBarStrVector << 0.38 << 0.77 << 2.3 << 6.72;
            SDStdvStrVector << 1.11 << 1.1 << 0.95 << 0.97;

            SDBarNonstrDriftVector << 0.48 << 0.96 << 3 << 6 ;
            SDStdvNonstrDriftVector << 1.15 << 1.09 << 0.93 << 0.99 ;

            SABarNonstrAccVector << 0.2 << 0.4 << 0.8 << 1.6 ;
            SAStdvNonstrAccVector << 0.67 << 0.65 << 0.65 << 0.65 ;

            PGABarVector << 0.08 << 0.11 << 0.18 << 0.34;
            PGAStdvVector << 0.64 << 0.64 << 0.64 << 0.64;

            if (magnitude <= 5.5) {
                kappa = 0.6;
            } else if (magnitude <= 7.5) {
                kappa = 0.3;
            } else {
                kappa = 0.1;
            }
        }
    } else {
        qCritical() << this->objectName() << "could not identify the building type for" << theBuildingType;
    }


    // Enter the central damage factors
    QVector<double> theCDFStrVector;
    QVector<double> theCDFNonstrAccVector;
    QVector<double> theCDFNonstrDriftVector;
    QVector<double> ContentDamageVector;
    QVector<double> RepairTimeVector;

    theCDFStrVector << 0.0;
    theCDFNonstrAccVector << 0.0;
    theCDFNonstrDriftVector << 0.0;
    ContentDamageVector << 0.0;
    RepairTimeVector << 0.0;

    double cost;


    // Occupancy class information: The data below are from the chapter 15 of the FEMA-NIBS document
    if (theOccupancyClass == QString("RES1")) {
        theCDFStrVector << 0.5 << 2.3 << 11.7 << 23.4;
        theCDFNonstrAccVector << 0.5 << 2.7 << 8.0 << 26.6;
        theCDFNonstrDriftVector << 1.0 << 5.5 << 25.0 << 50.0;
        ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
        RepairTimeVector << 2.0 << 30.0 << 90.0 << 180.0;
        cost = 90.85;	// Per square foot

    } else if (theOccupancyClass == QString("RES2")) {
        theCDFStrVector << 0.4 << 2.4 << 7.3 << 24.4;
        theCDFNonstrAccVector << 0.8 << 3.8 << 11.3 << 37.8;
        theCDFNonstrDriftVector << 0.8 << 3.8 << 18.9 << 37.8;
        ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
        RepairTimeVector << 2.0 << 10.0 << 30.0 << 60.0;
        cost = 30.90;	// Per square foot

    } else if (theOccupancyClass == QString("RES3A")) {
        theCDFStrVector << 0.3 << 1.4 << 6.9 << 13.8 ;
        theCDFNonstrAccVector << 0.8 << 4.3 << 13.1 << 43.7 ;
        theCDFNonstrDriftVector << 0.9 << 4.3 << 21.3 << 42.5 ;
        ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
        RepairTimeVector << 2.0 << 30.0 << 120.0 << 240.0;
        cost = 100.77;

    } else if (theOccupancyClass == QString("RES3B")) {
        theCDFStrVector << 0.3 << 1.4 << 6.9 << 13.8 ;
        theCDFNonstrAccVector  << 0.8 << 4.3 << 13.1 << 43.7 ;
        theCDFNonstrDriftVector   << 0.9 << 4.3 << 21.3 << 42.5 ;
        ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
        RepairTimeVector << 2.0 << 30.0 << 120.0 << 240.0;
        cost = 100.77;

    } else if (theOccupancyClass == QString("RES3C")) {
        theCDFStrVector << 0.3 << 1.4 << 6.9 << 13.8 ;
        theCDFNonstrAccVector   << 0.8 << 4.3 << 13.1 << 43.7 ;
        theCDFNonstrDriftVector  << 0.9 << 4.3 << 21.3 << 42.5 ;
        ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
        RepairTimeVector << 2.0 << 30.0 << 120.0 << 240.0;
        cost = 100.77;

    } else if (theOccupancyClass == QString("RES3D")) {
        theCDFStrVector << 0.3 << 1.4 << 6.9 << 13.8 ;
        theCDFNonstrAccVector  << 0.8 << 4.3 << 13.1 << 43.7 ;
        theCDFNonstrDriftVector  << 0.9 << 4.3 << 21.3 << 42.5 ;
        ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
        RepairTimeVector << 2.0 << 30.0 << 120.0 << 240.0;
        cost = 100.77;

    } else if (theOccupancyClass == QString("RES3E")) {
        theCDFStrVector << 0.3 << 1.4 << 6.9 << 13.8 ;
        theCDFNonstrAccVector  << 0.8 << 4.3 << 13.1 << 43.7 ;
        theCDFNonstrDriftVector << 0.9 << 4.3 << 21.3 << 42.5 ;
        ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
        RepairTimeVector << 2.0 << 30.0 << 120.0 << 240.0;
        cost = 100.77;

    } else if (theOccupancyClass == QString("RES3F")) {
        theCDFStrVector << 0.3 << 1.4 << 6.9 << 13.8 ;
        theCDFNonstrAccVector << 0.8 << 4.3 << 13.1 << 43.7 ;
        theCDFNonstrDriftVector << 0.9 << 4.3 << 21.3 << 42.5 ;
        ContentDamageVector << 1.0 << 5.0 << 25.0 << 50.0;
        RepairTimeVector << 2.0 << 30.0 << 120.0 << 240.0;
        cost = 100.77;

    } else if (theOccupancyClass == QString("RES4")) {
        theCDFStrVector << 0.2 << 1.4 << 6.8 << 13.6 ;
        theCDFNonstrAccVector << 0.9 << 4.3 << 13 << 43.2 ;
        theCDFNonstrDriftVector << 0.9 << 4.3 << 21.6 << 43.2 ;
        cost = 100.57;

    } else if (theOccupancyClass == QString("RES5")) {
        theCDFStrVector << 0.4 << 1.9 << 9.4 << 18.8 ;
        theCDFNonstrAccVector << 0.8 << 4.1 << 12.4 << 41.2 ;
        theCDFNonstrDriftVector << 0.8 << 4 << 20 << 40 ;
        cost = 110.54;

    } else if (theOccupancyClass == QString("RES6")) {
        theCDFStrVector << 0.4 << 1.8 << 9.2 << 18.4 ;
        theCDFNonstrAccVector << 0.8 << 4.1 << 12.2 << 40.8 ;
        theCDFNonstrDriftVector << 0.8 << 4.1 << 20.4 << 40.8 ;
        cost = 104.62;

    } else if (theOccupancyClass == QString("COM1")) {
        theCDFStrVector << 0.6 << 2.9 << 14.7 << 29.4 ;
        theCDFNonstrAccVector << 0.8 << 4.4 << 12.9 << 43.1 ;
        theCDFNonstrDriftVector << 0.6 << 2.7 << 13.8 << 27.5 ;
        cost = 77.17;

    } else if (theOccupancyClass == QString("COM2")) {
        theCDFStrVector << 0.6 << 3.2 << 16.2 << 32.4 ;
        theCDFNonstrAccVector << 0.8 << 4.2 << 12.4 << 41.1 ;
        theCDFNonstrDriftVector << 0.6 << 2.6 << 13.2 << 26.5 ;
        cost = 59.25;

    } else if (theOccupancyClass == QString("COM3")) {
        theCDFStrVector << 0.3 << 1.6 << 8.1 << 16.2 ;
        theCDFNonstrAccVector << 1 << 5 << 15 << 50 ;
        theCDFNonstrDriftVector << 0.7 << 3.4 << 16.9 << 33.8 ;
        cost = 100.89;

    } else if (theOccupancyClass == QString("COM4")) {
        theCDFStrVector << 0.4 << 1.9 << 9.6 << 19.2 ;
        theCDFNonstrAccVector << 0.9 << 4.8 << 14.4 << 47.9 ;
        theCDFNonstrDriftVector << 0.7 << 3.3 << 16.4 << 32.9 ;
        cost = 133.31;

    } else if (theOccupancyClass == QString("COM5")) {
        theCDFStrVector << 0.3 << 1.4 << 6.9 << 13.8 ;
        theCDFNonstrAccVector << 1 << 5.2 << 15.5 << 51.7 ;
        theCDFNonstrDriftVector << 0.7 << 3.4 << 17.2 << 34.5 ;
        cost = 88.21;

    } else if (theOccupancyClass == QString("COM6")) {
        theCDFStrVector << 0.2 << 1.4 << 7 << 14 ;
        theCDFNonstrAccVector << 1 << 5.1 << 15.4 << 51.3 ;
        theCDFNonstrDriftVector << 0.8 << 3.5 << 17.4 << 34.7 ;
        cost = 149.29;

    } else if (theOccupancyClass == QString("COM7")) {
        theCDFStrVector << 0.3 << 1.4 << 7.2 << 14.4 ;
        theCDFNonstrAccVector << 1 << 5.2 << 15.3 << 51.2 ;
        theCDFNonstrDriftVector << 0.7 << 3.4 << 17.2 << 34.4 ;
        cost = 127.21;

    } else if (theOccupancyClass == QString("COM8")) {
        theCDFStrVector << 0.2 << 1 << 5 << 10 ;
        theCDFNonstrAccVector << 1.1 << 5.4 << 16.3 << 54.4 ;
        theCDFNonstrDriftVector << 0.7 << 3.6 << 17.8 << 35.6 ;
        cost = 113.23;

    } else if (theOccupancyClass == QString("COM9")) {
        theCDFStrVector << 0.3 << 1.2 << 6.1 << 12.2 ;
        theCDFNonstrAccVector << 1 << 5.3 << 15.8 << 52.7 ;
        theCDFNonstrDriftVector << 0.7 << 3.5 << 17.6 << 35.1 ;
        cost = 105.98;

    } else if (theOccupancyClass == QString("COM10")) {
        theCDFStrVector << 1.3 << 6.1 << 30.4 << 60.9 ;
        theCDFNonstrAccVector << 0.3 << 2.2 << 6.5 << 21.7 ;
        theCDFNonstrDriftVector << 0.4 << 1.7 << 8.7 << 17.4 ;
        cost = 41.99;

    } else if (theOccupancyClass == QString("IND1")) {
        theCDFStrVector << 0.4 << 1.6 << 7.8 << 15.7 ;
        theCDFNonstrAccVector << 1.4 << 7.2 << 21.8 << 72.5 ;
        theCDFNonstrDriftVector << 0.2 << 1.2 << 5.9 << 11.8 ;
        cost = 76.22;

    } else if (theOccupancyClass == QString("IND2")) {
        theCDFStrVector << 0.4 << 1.6 << 7.8 << 15.7 ;
        theCDFNonstrAccVector << 1.4 << 7.2 << 21.8 << 72.5 ;
        theCDFNonstrDriftVector << 0.2 << 1.2 << 5.9 << 11.8 ;
        cost = 71.45;

    } else if (theOccupancyClass == QString("IND3")) {
        theCDFStrVector << 0.4 << 1.6 << 7.8 << 15.7 ;
        theCDFNonstrAccVector << 1.4 << 7.2 << 21.8 << 72.5 ;
        theCDFNonstrDriftVector << 0.2 << 1.2 << 5.9 << 11.8 ;
        cost = 90.65;

    } else if (theOccupancyClass == QString("IND4")) {
        theCDFStrVector << 0.4 << 1.6 << 7.8 << 15.7 ;
        theCDFNonstrAccVector << 1.4 << 7.2 << 21.8 << 72.5 ;
        theCDFNonstrDriftVector << 0.2 << 1.2 << 5.9 << 11.8 ;
        cost = 90.65;

    } else if (theOccupancyClass == QString("IND5")) {
        theCDFStrVector << 0.4 << 1.6 << 7.8 << 15.7 ;
        theCDFNonstrAccVector << 1.4 << 7.2 << 21.8 << 72.5 ;
        theCDFNonstrDriftVector << 0.2 << 1.2 << 5.9 << 11.8 ;
        cost = 90.65;

    } else if (theOccupancyClass == QString("IND6")) {
        theCDFStrVector << 0.4 << 1.6 << 7.8 << 15.7 ;
        theCDFNonstrAccVector << 1.4 << 7.2 << 21.8 << 72.5 ;
        theCDFNonstrDriftVector << 0.2 << 1.2 << 5.9 << 11.8 ;
        cost = 62.97;

    } else if (theOccupancyClass == QString("AGR1")) {
        theCDFStrVector << 0.8 << 4.6 << 23.1 << 46.2 ;
        theCDFNonstrAccVector << 0.8 << 4.6 << 13.8 << 46.1 ;
        theCDFNonstrDriftVector << 0 << 0.8 << 3.8 << 7.7 ;
        cost = 62.97;

    } else if (theOccupancyClass == QString("REL1")) {
        theCDFStrVector << 0.3 << 2 << 9.9 << 19.8 ;
        theCDFNonstrAccVector << 0.9 << 4.7 << 14.3 << 47.6 ;
        theCDFNonstrDriftVector << 0.8 << 3.3 << 16.3 << 32.6 ;
        cost = 114.08;

    } else if (theOccupancyClass == QString("GOV1")) {
        theCDFStrVector << 0.3 << 1.8 << 9 << 17.9 ;
        theCDFNonstrAccVector << 1 << 4.9 << 14.8 << 49.3 ;
        theCDFNonstrDriftVector << 0.7 << 3.3 << 16.4 << 32.8 ;
        cost = 117.69;

    } else if (theOccupancyClass == QString("GOV2")) {
        theCDFStrVector << 0.3 << 1.5 << 7.7 << 15.3 ;
        theCDFNonstrAccVector << 1 << 5.1 << 15.1 << 50.5 ;
        theCDFNonstrDriftVector << 0.7 << 3.4 << 17.1 << 34.2 ;
        cost = 109.70;

    } else if (theOccupancyClass == QString("EDU1")) {
        theCDFStrVector << 0.4 << 1.9 << 9.5 << 18.9 ;
        theCDFNonstrAccVector << 0.7 << 3.2 << 9.7 << 32.4 ;
        theCDFNonstrDriftVector << 0.9 << 4.9 << 24.3 << 48.7 ;
        cost = 98.87;

    } else if  (theOccupancyClass == QString("EDU2")) {
        theCDFStrVector << 0.2 << 1.1 << 5.5 << 11 ;
        theCDFNonstrAccVector << 0.6 << 2.9 << 8.7 << 29 ;
        theCDFNonstrDriftVector << 1.2 << 6 << 30 << 60 ;
        cost = 109.38;
    } else {
        qCritical() << this->objectName() << "could not identify the occupancy for" << theOccupancyClass;
    }

    // Threshold period of displacement preserevd region
    double Tvd = 10;// exp((magnitude - 5.0) / 2.0 * log(10.0));

    double SD;
    double SA;

    // Thresholds
    double cc = 32.2 * 12 / 4.0 / 3.14159265359 / 3.14159265359; // = 9.8: Sd = 9.8 * Sa * T * T when Sa is in g and Sd in inch
    double SDav = cc * Sa1 * Sa1 / Sa0_3;
    double SDvd = cc * Sa1 * Tvd;

    //Sd = Sa/(omega*omega) = Sa * T * T / (4 * pi * pi )

    // Determine SDp and SAp
    double SDp;
    double SAp;
    // State 1:
    SDp = Sa0_3 * Dy / Ay;
    // State 2:
    if (!(SDp <= SDav)) {
        SDp = sqrt(cc) * Sa1 * sqrt(Dy / Ay);
        // State 3:
        if (!((SDp > SDav) && (SDp <= SDvd))) {
            SDp = SDvd;
        }
    }
    if (SDp <= Dy) {
        SAp = Ay / Dy * SDp;
    } else if ((SDp > Dy) && (SDp <= Du)) {
        SAp = Ay + (Au - Ay) / (Du - Dy) * (SDp - Dy);
    } else {
        SAp =  Au;
    }

    // Calculate the total damping ratio
    //double betaEff = betaE + kappa * (Ay * Dy / 2.0 + (Au + Ay) * (Du - Dy) / 2.0) * 2.0 / (2 * 3.14159265359 * Du * Au);
    //double betaEff = betaE + kappa * 4.0 * (Ay * SDp - Dy * SAp)  / (2 * 3.14159265359 * SDp * SAp);
    double betaEff = betaE + kappa * 4.0 * SAp * (SDp - SAp/(Ay/Dy))  / (2 * 3.14159265359 * SDp * SAp);

    // Scale the spectral acceleration demand according to the new damping
    double Ra = 2.12 / (3.21 - 0.68 * log(betaEff * 100));
    double Rv = 1.65 / (2.31 - 0.41 * log(betaEff * 100));
    double Say1 = Sa1 / Rv;
    double Say0_3 = Sa0_3 / Ra;

    // Thresholds
    double SDyav = cc * Say1 * Say1 / Say0_3;
    double SDyvd = cc * Say1 * Tvd;

    // Starting to find the SD
    bool state123 = true;
    // State 1:
    SD = Say0_3 * Dy / Ay;
    SA = Say0_3; // or: SA = SD * Ay / Dy;

    // State 2:
    if (!((SD <= SDyav) && (SD <= Dy))) {
        SD = sqrt(cc) * Say1 * sqrt(Dy / Ay);
        SA = SD * Ay / Dy;

        // State 3:
        if (!((SD > SDyav) && (SD <= SDyvd) && (SD <= Dy))) {
            SD = SDyvd;
            SA = SD * Ay / Dy;

            // State 4:
            if (!(SD <= Dy)) {
                SD = Dy + (Du - Dy) / (Au - Ay) * (Say0_3 - Ay);
                SA = Say0_3; // or: SA = Ay + (Au - Ay) / (Du - Dy) * (SD - Dy);
                state123 = false;

                // State 5:
                if (!((SD <= SDyav) && (SD > Dy) && (SD <= Du))) {
                    double AA = (Au - Ay) / (Du - Dy);
                    double BB = Ay - AA * Dy;
                    BB /= 2.0;
                    double CC = -cc * Say1 * Say1;
                    SD = (-BB + sqrt(BB * BB - AA * CC)) / AA;
                    SA = Ay + (Au - Ay) / (Du - Dy) * (SD - Dy);

                    // State 6:
                    if (!((SD > SDyav) && (SD <= SDyvd) && (SD > Dy) && (SD <= Du))) {
                        SD = SDyvd;
                        SA = Ay + (Au - Ay) / (Du - Dy) * (SD - Dy);

                        // State 7
                        if (!((SD > Dy) && (SD <= Du))) {
                            SD = cc * Say1 * Say1 / Au;
                            SA = Au;

                            // State 8
                            if (!((SD > Du) && (SD > SDyav) && (SD <= SDyvd))) {
                                SD = SDyvd;
                                SA = Au;
                                //qCritical() << "Warning: A too large deformation is calculated equal to" << SD << "inch in the model" << objectName() << ".";

                            }
                        }
                    }
                }
            }
        }
    }

    // In case od state 1 or 2 or 3 (intersection in elastic capacity range), it should be ckecked whether the yield point falls between the elastic and inelastic demand curve:
    if (state123) {
        double SDe;
        double SAe;

        // State 1:
        SDe = Sa0_3 * Dy / Ay;
        SAe = Sa0_3; // or: SAe = SDe * Ay / Dy;

        // State 2:
        if (!((SDe <= SDav) && (SDe <= Dy))) {
            SDe = 3.14159265359 * Sa1 * sqrt(Dy / Ay);
            SAe = SDe * Ay / Dy;

            // State 3:
            if (!((SDe > SDav) && (SDe <= SDvd) && (SDe <= Dy))) {
                SDe = SDvd;
                SAe = SDe * Ay / Dy;

                // State 4,5,6,7:
                if (!(SDe <= Dy)) {
                    SD = Dy;
                    SA = Ay;
                } else {
                    SD = SDe;
                    SA = SAe;
                }
            } else {
                SD = SDe;
                SA = SAe;
            }
        } else {
            SD = SDe;
            SA = SAe;
        }
    }

    if (SA > Au) {
        SA = Au;
    }


    // Evaluating the model expression of the structural damage model
    RNormalDistribution theNormalDistribution(theDomain);


    // THIS MODEL CALCULATES PGD CAUSED BY LIQUEFACTION
    // This model was double checked against the algorithm presented in the latest Hazus manual.
    // It suffers from the problem of not having a map of liquefaction susceptibility built-in R3.
    // It is assumed that ports are at high susceptibility of liquefaction due to being close to the water.
    // All other models are considered at LOW susceptibility of liquefaction.
    // This will need to be improved eventually.

    double theKm = 0.0;
    double theKw = 0.0;
    double theGroundwaterDepth = 5.0; //in feet (the factors were obtained for GWD of 5 feet - page 140 Hazus 2.1)
    double theDisplacementCorrectionFactor = 0.0;
    double theThresholdPGA = 0.0;
    double theProportionOfTheMap = 0.0;
    double theLiquefactionSusceptibility;
    double theVerticalPGD;

    // 1 - Define if the fixed structure is or is not on a zone susceptible to liquefaction
    // This is not being considered

    // 2 - Conditional probability of liquefaction for a susceptibility category (this is the Prob. of Liquefaction for a given set of conditions)
    // The vertical PGD is a simple function of the susceptibility level, therefore it is calculated here too
    // See Hazus 2.1 item 4.2.2.1.3 on page 138 of the pdf file
    // See Table 4.13 Hazus report - a large number here denotes a soil with no susceptibility to liquefaction.
    if (theSoilSusceptibilityLevel == QString("None")){
        theThresholdPGA = 1000.0;
        theVerticalPGD = 0.0;
        theLiquefactionSusceptibility = 0.0;
        theProportionOfTheMap = 0.0;

    } else if (theSoilSusceptibilityLevel == QString("VeryLow")){
        theThresholdPGA = 0.26;
        theVerticalPGD = 0.0;
        theLiquefactionSusceptibility = 4.16 * thePGA - 1.08;
        theProportionOfTheMap = 0.02;

    } else if (theSoilSusceptibilityLevel == QString("Low")){
        theThresholdPGA = 0.21;
        theVerticalPGD = 1.0;
        theLiquefactionSusceptibility = 5.57 * thePGA - 1.18;
        theProportionOfTheMap = 0.05;

    } else if (theSoilSusceptibilityLevel == QString("Moderate")){
        theThresholdPGA = 0.15;
        theVerticalPGD = 2.0;
        theLiquefactionSusceptibility = 6.67 * thePGA - 1.0;
        theProportionOfTheMap = 0.10;

    } else if (theSoilSusceptibilityLevel == QString("High")){
        theThresholdPGA = 0.12;
        theVerticalPGD = 6.0;
        theLiquefactionSusceptibility = 7.67 * thePGA - 0.92;
        theProportionOfTheMap = 0.20;

    } else if (theSoilSusceptibilityLevel == QString("VeryHigh")){
        theThresholdPGA = 0.09;
        theVerticalPGD = 12.0;
        theLiquefactionSusceptibility = 9.09 * thePGA - 0.82;
        theProportionOfTheMap = 0.25;

    } else {
//        qCritical() << this->objectName() << "could not identify the Soil Susceptibility Level!";
        theThresholdPGA = 0.0;
        theVerticalPGD = 0.0;
        theLiquefactionSusceptibility = 0.0;
        theProportionOfTheMap = 0.0;
    }

    // 2.5 - Uncertainty in the ground displacement estimates
    // The uncertainty associated with vertical settlement values is assumed to have a uniform probability distribution within bounds of one-half to two times the respective value.
    // The following expression guarantees a number between 0.5 and 2.0 the PGDs
    theVerticalPGD = theVerticalPGD - 0.5 * theVerticalPGD + 1.5 * theVerticalPGD * rand() / static_cast<double>(RAND_MAX);


    // Bounds for the conditional probability of liquefaction
    if (theLiquefactionSusceptibility < 0.0){
        theLiquefactionSusceptibility = 0.0;

    } else if (theLiquefactionSusceptibility > 1.0) {
        theLiquefactionSusceptibility = 1.0;
    }


    // 3 - Expected PGD
    // get the earthquake magnitude
    double M = theMagnitude->getCurrentValue();

    // Calculating the probability of soil liquefaction
    theKm = 0.0027 * M*M*M - 0.0267 * M*M - 0.2055 * M + 2.9188;
    theKw = 0.022 * theGroundwaterDepth + 0.93;

    // For lateral spreading Eq 4.24 Hazus 2.1
    theDisplacementCorrectionFactor = 0.0086 * M*M*M - 0.0914 * M*M + 0.4698 * M - 0.9835;

    double theLateralPGD = 0.0;
    // Depending on the ratio between PGA and threshold PGA, calculate the Lateral Spreading in inches
    if (thePGA/theThresholdPGA < 1.0){
        theLateralPGD = 0.0;

        // See Figure 4.9 Hazus 2.1 page 143 of the pdf file
    } else if ( (thePGA/theThresholdPGA) > 1.0 && (thePGA/theThresholdPGA) < 2.0 ) {
        theLateralPGD = (12.0*(thePGA / theThresholdPGA) - 12.0) * theDisplacementCorrectionFactor;

    } else if ( (thePGA/theThresholdPGA) > 2.0 && (thePGA/theThresholdPGA) < 3.0) {
        theLateralPGD = (18.0*(thePGA / theThresholdPGA) - 24.0) * theDisplacementCorrectionFactor;

    } else if ( (thePGA/theThresholdPGA) > 3.0 && (thePGA/theThresholdPGA) < 4.0){
        theLateralPGD = (70.0*(thePGA / theThresholdPGA) - 180.0) * theDisplacementCorrectionFactor;

        // Defining an forced upper bound for lateral spreading
    } else if (thePGA / theThresholdPGA > 4.0){
        theLateralPGD = (70.0*4.0 - 180.0) * theDisplacementCorrectionFactor;

    } else {
        qCritical() << this->objectName() << "The ratio of PGA to PGA threshold is out of bounds!";
        theLateralPGD = 0.0;
    }


    double theProbOfGFDS3 = 0.0;
    double theProbOfGFDS4 = 0.0;

    double m = 0.0;
    double v = 1.2 * 1.2;

    if ((theLateralPGD > 12.0 && theLateralPGD < 60.0) || (theVerticalPGD > 2.0 && theVerticalPGD < 10.0)){
        m = 0.1;
        theProbOfGFDS3 = 0.8;
        theProbOfGFDS4 = 0.2;
    } else if (theLateralPGD >= 60.0 || theVerticalPGD >= 10.0){
        m = 0.5;
        theProbOfGFDS3 = 0.8;
        theProbOfGFDS4 = 0.2;
    }

    if (m > 0.0){
        double phi = qSqrt(v + m * m);
        double mu = qLn(m * m / phi);
        double sigma = qSqrt(qLn((phi*phi) / (m*m)));
        double theUx = rand()/static_cast<double>(RAND_MAX);
        theProbOfGFDS3 *= exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);
        theProbOfGFDS4 *= exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);
    }

    // ==========================================


    theStrDamageState = 0;
    theNSDDamageState = 0;
    theNSADamageState = 0;

    // Calculate the probability of each structural damage state
    QVector<double> probabilityVector;
    double prob = 1.0;
    int count = SDBarStrVector.count();
    for (int m = 0; m < count; m++) {
        double oldProb = prob;

        // Prob of DS=dsi from ground failure
        double probOfDSiFromGF = theProbOfGFDS3;
        if (m == 3){ // position 3 equal DS 4
            probOfDSiFromGF = theProbOfGFDS4;
        }

        // Prob of DS=dsi from ground shaking
        double probOfDSiFromGS = theNormalDistribution.getNormalCDF(1.0 / SDStdvStrVector[m] * log(SD / SDBarStrVector[m]), 1);

        // Combined Prob. see HAZUS manual page 246 of the pdf file
        prob = probOfDSiFromGF + probOfDSiFromGS - probOfDSiFromGF * probOfDSiFromGS;
        probabilityVector << oldProb - prob;
    }
    probabilityVector << prob - 0.0;

    double rngStrDS = rand()/static_cast<double>(RAND_MAX);

    if (rngStrDS < probabilityVector.at(0)){
        theStrDamageState = 0;
    } else if (rngStrDS >= probabilityVector.at(0) && rngStrDS < probabilityVector.at(0) + probabilityVector.at(1)) {
        theStrDamageState = 1;
    } else if (rngStrDS >= probabilityVector.at(0) + probabilityVector.at(1) && rngStrDS < probabilityVector.at(0) + probabilityVector.at(1) + probabilityVector.at(2)) {
        theStrDamageState = 2;
    } else if (rngStrDS >= probabilityVector.at(0) + probabilityVector.at(1) + probabilityVector.at(2) && rngStrDS < probabilityVector.at(0) + probabilityVector.at(1) + probabilityVector.at(2) + probabilityVector.at(3)) {
        theStrDamageState = 3;
    } else if (rngStrDS >= probabilityVector.at(0) + probabilityVector.at(1) + probabilityVector.at(2) + probabilityVector.at(3)) {
        theStrDamageState = 4;
    }


    // Calculate the probability of each non-structural drift-sensitive damage state
    QVector<double> probabilityNDVector;
    double probND = 1.0;
    int countND = SDBarNonstrDriftVector.count();
    for (int m = 0; m < countND; m++) {
        double oldProb = probND;
        probND = theNormalDistribution.getNormalCDF(1.0 / SDStdvNonstrDriftVector[m] * log(SD / SDBarNonstrDriftVector[m]), 1);
        probabilityNDVector << oldProb - probND;
    }
    probabilityNDVector << probND - 0.0;

    double rngNSDDS = rand()/static_cast<double>(RAND_MAX);

    if (rngNSDDS < probabilityNDVector.at(0)){
        theNSDDamageState = 0;
    } else if (rngNSDDS >= probabilityNDVector.at(0) && rngNSDDS < probabilityNDVector.at(0) + probabilityNDVector.at(1)) {
        theNSDDamageState = 1;
    } else if (rngNSDDS >= probabilityNDVector.at(0) + probabilityNDVector.at(1) && rngNSDDS < probabilityNDVector.at(0) + probabilityNDVector.at(1) + probabilityNDVector.at(2)) {
        theNSDDamageState = 2;
    } else if (rngNSDDS >= probabilityNDVector.at(0) + probabilityNDVector.at(1) + probabilityNDVector.at(2) && rngNSDDS < probabilityNDVector.at(0) + probabilityNDVector.at(1) + probabilityNDVector.at(2) + probabilityNDVector.at(3)) {
        theNSDDamageState = 3;
    } else if (rngNSDDS >= probabilityNDVector.at(0) + probabilityNDVector.at(1) + probabilityNDVector.at(2) + probabilityNDVector.at(3)) {
        theNSDDamageState = 4;
    }

    // Calculate the probability of each non-structural acc-sensitive damage state
    QVector<double> probabilityNAVector;
    double probNA = 1.0;
    int countNA = SABarNonstrAccVector.count();
    for (int m = 0; m < countNA; m++) {
        double oldProb = probNA;
        probNA = theNormalDistribution.getNormalCDF(1.0 / SAStdvNonstrAccVector[m] * log(SA / SABarNonstrAccVector[m]), 1);
        probabilityNAVector << oldProb - probNA;
    }
    probabilityNAVector << probNA - 0.0;

    double rngNSADS = rand()/static_cast<double>(RAND_MAX);

    if (rngNSADS < probabilityNAVector.at(0)){
        theNSADamageState = 0;
    } else if (rngNSADS >= probabilityNAVector.at(0) && rngNSADS < probabilityNAVector.at(0) + probabilityNAVector.at(1)) {
        theNSADamageState = 1;
    } else if (rngNSADS >= probabilityNAVector.at(0) + probabilityNAVector.at(1) && rngNSADS < probabilityNAVector.at(0) + probabilityNAVector.at(1) + probabilityNAVector.at(2)) {
        theNSADamageState = 2;
    } else if (rngNSADS >= probabilityNAVector.at(0) + probabilityNAVector.at(1) + probabilityNAVector.at(2) && rngNSADS < probabilityNAVector.at(0) + probabilityNAVector.at(1) + probabilityNAVector.at(2) + probabilityNAVector.at(3)) {
        theNSADamageState = 3;
    } else if (rngNSADS >= probabilityNAVector.at(0) + probabilityNAVector.at(1) + probabilityNAVector.at(2) + probabilityNAVector.at(3)) {
        theNSADamageState = 4;
    }


    // Evaluate immediate losses and repair times
    theEffectiveDamageRatio = (theCDFStrVector.at(theStrDamageState) + theCDFNonstrDriftVector.at(theNSDDamageState) + theCDFNonstrAccVector.at(theNSADamageState)) / 100.0;
    theStructuralLossRatio = (theCDFStrVector.at(theStrDamageState)) / 100.0;
    theDriftLossRatio = (theCDFNonstrDriftVector.at(theNSDDamageState)) / 100.0;
    theAccelerationLossRatio = (theCDFNonstrAccVector.at(theNSADamageState)) / 100.0;
    theLossRatio = (theStructuralLossRatio + theDriftLossRatio + theAccelerationLossRatio);
    theContentLossRatio = ContentDamageVector.at(theNSADamageState);
    theRepairTime = RepairTimeVector.at(theStrDamageState);

    //qCritical() << "This is a" << theBuildingType << "with code" << theCodeDesignLevel << "with loss" << theStructuralLossRatio << theDriftLossRatio << theAccelerationLossRatio;
    // If too much loss
    if (theLossRatio > 0.50 && theBuildingType.contains(QString("W")) == false){
        theLossRatio = 1.25;
        //theRepairTime = 1.25 * RepairTimeVector.at(RepairTimeVector.size() - 1);
        theContentLossRatio = ContentDamageVector.at(ContentDamageVector.size() - 1);
    }
    // If old building
    else if (theLossRatio > 0.40 && (theCodeDesignLevel == QString("Pre") || theCodeDesignLevel == QString("Low"))){
        theLossRatio = 1.25;
        //theRepairTime = 1.25 * RepairTimeVector.at(RepairTimeVector.size() - 1);
        theContentLossRatio = ContentDamageVector.at(ContentDamageVector.size() - 1);
    }

    // Reoccupancy time
    double theServiceInterruptionMultiplier = 0.0;
    if (theStrDamageState >= 3 || theLossRatio > 1.0){
        theServiceInterruptionMultiplier = 1.0;

    } else {
        theServiceInterruptionMultiplier = 0.5;
    }

    theReoccupancyTime = theServiceInterruptionMultiplier * theRepairTime;

    // Updating the response current value
    theDamageFactor->setCurrentValue(0.0);

    return 0;
}





