#ifndef RLocationResponse_H
#define RLocationResponse_H

#include "RResponse.h"

class RLocation;

class RLocationResponse : public RResponse
{
	Q_OBJECT

    // Sets the RLocation object that the response "carries"
    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)

public:
    RLocationResponse(QObject *parent, QString name);
    ~RLocationResponse();
	
	// getter/setter methods for meta-properties
    QObject *getLocation() const;
    void setLocation(QObject *value);

    // Specific implementation of the setObjectName, which sets the object name for the response and its auto-generated RLocation object
	void setObjectName(const QString &name);

	// Backs up the current values of the response (latitude, longitude, and altitude)
	void backup();

	// Backs up the current values of the response (latitude, longitude, and altitude)
	void restore();

	// Returns the vector of response values, used in DDM computations; It has three elements: latitude, longitude, and altitude
	QVector<double> getValueVector();

	// Sets the vector of response values as explained above
	void setValueVector(QVector<double> valueVector);

	// Returns the backup vector of response values as explained above
	QVector<double> getBackupVector();


    QStringList getActionList();


private:
	// Data member for meta-properties
    RLocation *theLocation;

	// Data member for the backup vector of response values as explained above
	QVector<double> backupVector;
	
};

#endif // RLocationResponse_H
