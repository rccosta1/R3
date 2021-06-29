#ifndef RLocation_H
#define RLocation_H

#include "RParameter.h"

#include <math.h>

#include <QPointer>

class QNetworkReply;
class RMainWindow;

const double EarthRadius = 6372.796924;

class RLocation : public RParameter
{
	Q_OBJECT	

	Q_PROPERTY(double Latitude READ getLatitude WRITE setLatitude)
	Q_PROPERTY(double Longitude READ getLongitude WRITE setLongitude)
	Q_PROPERTY(double Altitude READ getAltitude WRITE setAltitude)
    Q_PROPERTY(QString PGAIntensity READ getPGAIntensity WRITE setPGAIntensity)
    Q_PROPERTY(QString Sa1Intensity READ getSa1Intensity WRITE setSa1Intensity)
    Q_PROPERTY(QString Sa0_3Intensity READ getSa0_3Intensity WRITE setSa0_3Intensity)
    Q_PROPERTY(QString ShearWaveVelocity READ getShearWaveVelocity WRITE setShearWaveVelocity)

public:
    RLocation(QObject *parent, QString name);
    virtual ~RLocation();
	
	double getLatitude() const;
	void setLatitude(double value);

    double getLongitude() const;
	void setLongitude(double value);

    double getAltitude() const;
	void setAltitude(double value);

    QString getPGAIntensity();
    void setPGAIntensity(QString value);

    QString getSa1Intensity() const;
    void setSa1Intensity(QString value);

    QString getSa0_3Intensity() const;
    void setSa0_3Intensity(QString value);

    QString getShearWaveVelocity();
    double getShearWaveVelocityValue();
    void setShearWaveVelocity(QString value);

	// Computes the surface distance to another location
    double computeSurfaceDistance(const RLocation *destination);

	// Computes the bearing (angle with north direction)
    double computeBearing(const RLocation *destination);

	// Computes the destination location given the distance and bearing
    void computeDestination(RLocation *destination, double distance, double bearing);

	// Computes the coordinates of the an intermediate point given a ratio of the distance
    void computeIntermediatePoint(RLocation *endPoint, RLocation *intermediatePoint, double distanceRatioFromStartPoint);

	// Computes the derivative of the latitude and longitude of the intermediate point wrt. the ratio of the distance by DDM
    void computeIntermediatePointGradient(RLocation *endPoint, double distanceRatioFromStartPoint ,double *dLat_dDistanceRatio, double *dLng_dDistanceRatio);

	// Computes the coordinates of the intersection with a given latitude
    int computeIntersectionWithLatitude(double latitude, RLocation *endPoint, double *intersectionLongitude);

	// Returns a list of "right-click" menu items
	QStringList getActionList();

public slots:

private:

    double degreeToRadian(double degree);
	double radianToDegree(double radian);

	double theLatitude;
	double theLongitude;
	double theAltitude;
    QString thePGAIntensity;
    QString theSa1Intensity;
    QString theSa0_3Intensity;
	QString theAddress;
    QPointer<RParameter> theShearWaveVelocityParameter;
    double theShearWaveVelocityDouble;

	RMainWindow *theMainWindow;	
};

#endif
