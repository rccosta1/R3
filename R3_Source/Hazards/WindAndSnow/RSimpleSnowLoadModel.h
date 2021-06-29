#ifndef RSimpleSnowLoadModel_H
#define RSimpleSnowLoadModel_H

#include "RSnowBaseClass.h"
#include "RParameter.h"

class RResponse;

class RSimpleSnowLoadModel : public RSnowBaseClass
{
	Q_OBJECT

    Q_PROPERTY(QObject *SnowStormMagnitude READ getSnowStormMagnitude WRITE setSnowStormMagnitude)

public:

    RSimpleSnowLoadModel(QObject *parent, QString name);
	~RSimpleSnowLoadModel();

    QObject *getSnowStormMagnitude() const;
    void setSnowStormMagnitude(QObject *value);

    bool isOn();
    int evaluateModel();

private:

    QPointer<RParameter> theSnowStormMagnitude = nullptr;

    RResponse *theSnowPressure = nullptr;
};

#endif
