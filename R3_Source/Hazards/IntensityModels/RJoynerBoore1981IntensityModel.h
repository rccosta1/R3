#ifndef RJoynerBoore1981IntensityModel_H	
#define RJoynerBoore1981IntensityModel_H	
	
#include "RModel.h"	
#include "REarthquakeBaseClass.h"
	
class RParameter;	
class RResponse;
class RLocation;
	
class RJoynerBoore1981IntensityModel : public REarthquakeBaseClass
{	
	Q_OBJECT
	Q_PROPERTY(QObject *Magnitude READ getMagnitude WRITE setMagnitude)
	Q_PROPERTY(QObject *Alpha READ getAlpha WRITE setAlpha)
	Q_PROPERTY(QObject *Beta READ getBeta WRITE setBeta)
	Q_PROPERTY(QObject *B READ getB WRITE setB)
	Q_PROPERTY(QObject *Epsilon READ getEpsilon WRITE setEpsilon)
    Q_PROPERTY(QObject *HypocentreLocation READ getHypocentreLocation WRITE setHypocentreLocation)
	
public:	
	RJoynerBoore1981IntensityModel(QObject *parent, QString name);
	~RJoynerBoore1981IntensityModel();
	
	QObject *getMagnitude() const;
	void setMagnitude(QObject *value);

	QObject *getAlpha() const;
	void setAlpha(QObject *value);

	QObject *getBeta() const;
	void setBeta(QObject *value);

	QObject *getB() const;
	void setB(QObject *value);

	QObject *getEpsilon() const;
	void setEpsilon(QObject *value);

    QObject *getHypocentreLocation() const;
    void setHypocentreLocation(QObject *value);	
	
    bool isOn();
    int evaluateModel();

private:

	QPointer<RParameter> theMagnitude;
	QPointer<RParameter> theAlpha;
	QPointer<RParameter> theBeta;
    QPointer<RParameter> theB;
	QPointer<RParameter> theEpsilon;
    QPointer<RParameter> theHypocentreLocation;

    RResponse *theResponse;
};
	
#endif
