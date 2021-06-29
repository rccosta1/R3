#ifndef REarthquakeBaseClass_H
#define REarthquakeBaseClass_H

#include "RIntensityModel.h"

class REarthquakeBaseClass : public RIntensityModel
{
	Q_OBJECT

public:
	REarthquakeBaseClass(QObject *parent, QString name);
	virtual ~REarthquakeBaseClass();

public slots:


signals:


protected:



protected slots:


private:
	
};

#endif // REarthquakeBaseClass_H
