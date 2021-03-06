#ifndef RSimpleWindPressureModel_H
#define RSimpleWindPressureModel_H

#include "RWindBaseClass.h"

class RParameter;
class RResponse;

class RSimpleWindPressureModel : public RWindBaseClass
{
	Q_OBJECT

	Q_PROPERTY(QObject *Velocity READ getVelocity WRITE setVelocity)
	Q_PROPERTY(QObject *AirDensity READ getAirDensity WRITE setAirDensity)
	Q_PROPERTY(QObject *ExposureFactor READ getExposureFactor WRITE setExposureFactor)
	Q_PROPERTY(QObject *ExternalPressureFactor READ getExternalPressureFactor WRITE setExternalPressureFactor)
	Q_PROPERTY(QObject *GustFactor READ getGustFactor WRITE setGustFactor)
	Q_PROPERTY(QObject *Epsilon READ getEpsilon WRITE setEpsilon)
    Q_PROPERTY(QObject *Occurrence READ getOccurrence WRITE setOccurrence)

public:
	// Define the constructor and destructor
	RSimpleWindPressureModel(QObject *parent, QString name);
	~RSimpleWindPressureModel();

	// Define the mandatory read/write methods for all QProperties
	QObject *getVelocity() const;
	void setVelocity(QObject *value);
	QObject *getAirDensity() const;
	void setAirDensity(QObject *value);
	QObject *getExposureFactor() const;
	void setExposureFactor(QObject *value);
	QObject *getExternalPressureFactor() const;
	void setExternalPressureFactor(QObject *value);
	QObject *getGustFactor() const;
	void setGustFactor(QObject *value);
	QObject *getEpsilon() const;
	void setEpsilon(QObject *value);

    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);

	// Define the method where the actual model is coded
    int evaluateModel();

private:
	QPointer<RParameter> theVelocity;
	QPointer<RParameter> theAirDensity;
	QPointer<RParameter> theExposureFactor;
	QPointer<RParameter> theExternalPressureFactor;
	QPointer<RParameter> theGustFactor;
	QPointer<RParameter> theEpsilon;

    QPointer<RParameter> theOccurrence;
	
	// Declare responses
	RResponse *theWindPressure;

};

#endif
