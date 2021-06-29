#ifndef RHazusBuildingModel_H
#define RHazusBuildingModel_H

#include "RModel.h"

class RParameter;
class RResponse;

class RHazusBuildingModel : public RModel
{
	Q_OBJECT

	// Define a QProperty for each input (parameter) data member. The type of the QProperty is "QObject *"
    Q_PROPERTY(QString BuildingType READ getBuildingType WRITE setBuildingType)
    Q_PROPERTY(QString OccupancyClass READ getOccupancyClass WRITE setOccupancyClass)
    Q_PROPERTY(QString CodeDesignLevel READ getCodeDesignLevel WRITE setCodeDesignLevel)
	Q_PROPERTY(QObject *Magnitude READ getMagnitude WRITE setMagnitude)


	Q_ENUMS(RBuildingType)
	Q_ENUMS(ROccupancyClass)
	Q_ENUMS(RCodeDesignLevel)

public:
	// Define the constructor and destructor
	RHazusBuildingModel(QObject *parent, QString name);
	~RHazusBuildingModel();

	enum RBuildingType{W1, W2, S1L, S1M, S1H, S2L, S2M, S2H, S3, S4L, S4M, S4H, S5L, S5M, S5H, C1L, C1M, C1H, C2L, C2M, C2H, C3L, C3M, C3H, PC1, PC2L, PC2M, PC2H, RM1L, RM1M, RM2L, RM2M, RM2H, URML, URMM, MH};
	enum ROccupancyClass{RES1, RES2, RES3A, RES3B, RES3C, RES3D, RES3E, RES3F, RES4, RES5, RES6, COM1, COM2, COM3, COM4, COM5, COM6, COM7, COM8, COM9, COM10, IND1, IND2, IND3, IND4, IND5, IND6, AGR1, REL1, GOV1, GOV2, EDU1, EDU2};
	enum RCodeDesignLevel{High, Moderate, Low, Pre};

	// Define the mandatory read/write methods for all QProperties
	QObject *getMagnitude() const;
	void setMagnitude(QObject *value);

    double getSa1() const;
    void setSa1(double value);

    double getSa2() const;
    void setSa2(double value);

    void setPGA(double value);

    QString getBuildingType() const;
    void setBuildingType(QString value);

    QString getOccupancyClass() const;
    void setOccupancyClass(QString value);

    QString getCodeDesignLevel() const;
    void setCodeDesignLevel(QString value);

    void setSoilSusceptibilityLevel(QString value);

	// Define the method where the actual model is coded
	int evaluateModel();

    int getStrDS();
    int getNSDDS();
    int getNSADS();
    double getLossRatio();
    double getContentLossRatio();
    double getRepairTime();
    double getReoccupancyTime();

private:
	// Declare input data members
    double theSa1;
    double theSa2;
    double thePGA;
	QPointer<RParameter> theMagnitude;
    QString theBuildingType;
    QString theOccupancyClass;
    QString theCodeDesignLevel;
    QString theSoilSusceptibilityLevel;
	
	// Declare responses
	RResponse *theDamageFactor;
    int theStrDamageState;
    int theNSDDamageState;
    int theNSADamageState;
    double theEffectiveDamageRatio;
    double theStructuralLossRatio;
    double theDriftLossRatio;
    double theAccelerationLossRatio;
    double theLossRatio;
    double theContentLossRatio;
    double theRepairTime;
    double theReoccupancyTime;

};

#endif // RHazusBuildingModel_H
