#ifndef RScaledGroundMotionModel_H
#define RScaledGroundMotionModel_H

#include "REarthquakeBaseClass.h"

class RTimeSeries;

class RScaledGroundMotionModel : public REarthquakeBaseClass
{
    Q_OBJECT

    Q_PROPERTY(QString InputFile READ getInputFile WRITE setInputFile)


public:
    RScaledGroundMotionModel(QObject *parent, QString name);
    ~RScaledGroundMotionModel();

    QString getInputFile() const;
    void setInputFile(QString value);

    QStringList getActionList();

    bool isOn();

    int evaluateModel();

    RResponse* getMagnitudeResponse();

private:

    QString theInputFile;
    RResponse *theGroundMotionResponse;
    RTimeSeries *theInputTimeSeries;

};

#endif
