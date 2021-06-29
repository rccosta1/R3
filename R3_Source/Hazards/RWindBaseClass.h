#ifndef RWindBaseClass_H
#define RWindBaseClass_H

#include "RIntensityModel.h"

class RWindBaseClass : public RIntensityModel
{
	Q_OBJECT

public:
	RWindBaseClass(QObject *parent, QString name);
	virtual ~RWindBaseClass();

public slots:


signals:


protected:



protected slots:


private:
	
};

#endif // RWindBaseClass_H
