#ifndef RSnowBaseClass_H
#define RSnowBaseClass_H

#include "RIntensityModel.h"

class RSnowBaseClass : public RIntensityModel
{
	Q_OBJECT

public:
    RSnowBaseClass(QObject *parent, QString name);
    virtual ~RSnowBaseClass();

    //virtual int evaluateModel(RGradientType theGradientType) = 0;


public slots:


signals:


protected:



protected slots:


private:
	
};

#endif // RSnowBaseClass_H
