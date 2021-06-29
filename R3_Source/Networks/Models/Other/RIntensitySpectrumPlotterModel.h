#ifndef RIntensitySpectrumPlotterModel_H
#define RIntensitySpectrumPlotterModel_H

#include "RModel.h"
#include "RLocation.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RIntensitySpectrumPlotterModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QString IntensityList READ getIntensityList WRITE setIntensityList)
    Q_PROPERTY(QObject *BuildingLocation READ getBuildingLocation WRITE setBuildingLocation)

public:

    RIntensitySpectrumPlotterModel(QObject *parent, QString name);
    ~RIntensitySpectrumPlotterModel();

    QString getIntensityList();
    void setIntensityList(QString value);

    QObject *getBuildingLocation() const;
    void setBuildingLocation(QObject *value);

    int evaluateModel(RGradientType theGradientType);

private:
    // Member functions
    void plotIntensities(int graphNumber, QVector<double> xValues, QVector<double> yValues);

    // Data members
    QList<QPointer<RParameter> > theIntensityList;
    QPointer<RParameter> theBuildingLocationParameter;
    QVector<double> theVariablesToPlot;
    QPointer<QCustomPlot>  IntensityCustomPlot;
    RResponse *theResponse;
    int plotCounter;

};

#endif
