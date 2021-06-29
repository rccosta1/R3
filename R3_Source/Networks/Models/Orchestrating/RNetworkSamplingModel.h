#ifndef RNetworkSamplingModel_H
#define RNetworkSamplingModel_H

#include "RModel.h"
#include "RRandomNumberGenerator.h"
#include "RObject.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RProbabilityTransformation.h"
#include "qcustomplot.h"



class RNetworkSamplingModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)
    Q_PROPERTY(QString VariableOfInterest READ getVariableOfInterest WRITE setVariableOfInterest)
    Q_PROPERTY(int MaxSamples READ getMaxSamples WRITE setMaxSamples)
    Q_PROPERTY(int PlotInterval READ getPlotInterval WRITE setPlotInterval)
    Q_PROPERTY(QString SamplingCentre READ getSamplingCentre WRITE setSamplingCentre)
    Q_PROPERTY(QObject *RandomNumberGenerator READ getRandomNumberGenerator WRITE setRandomNumberGenerator)
    Q_PROPERTY(QObject *ProbabilityTransformation READ getProbabilityTransformation WRITE setProbabilityTransformation)
    Q_PROPERTY(RSamplingMultiplierFunction SamplingMultiplierFunction READ getSamplingMultiplierFunction WRITE setSamplingMultiplierFunction)

    Q_ENUMS(RSamplingMultiplierFunction)
public:

    RNetworkSamplingModel(QObject *parent, QString name);
    ~RNetworkSamplingModel();

    enum RSamplingMultiplierFunction{Constant, Linear, None};

    QString getInputParameterList();
    void setInputParameterList(QString value);

    QString getVariableOfInterest();
    void setVariableOfInterest(QString value);

    int getMaxSamples() const;
    void setMaxSamples(int value);

    int getPlotInterval() const;
    void setPlotInterval(int value);

    QString getSamplingCentre();
    void setSamplingCentre(QString value);

    QObject *getRandomNumberGenerator() const;
    void setRandomNumberGenerator(QObject *value);

    QObject *getProbabilityTransformation() const;
    void setProbabilityTransformation(QObject *value);

    RSamplingMultiplierFunction getSamplingMultiplierFunction();
    void setSamplingMultiplierFunction(RSamplingMultiplierFunction value);

    QStringList getActionList();

    double getSamplingMultiplier();

    int evaluateModel();

    void PlotSamplingResults(QVector<double> timeVector, QVector<QVector<double>> valueVector);

private:
    // Members functions
    double evaluateLimitStateFunction();

    // Data members
    int maxSamples;
    int thePlotInterval;
    double theSamplingMultiplier;
    RSamplingMultiplierFunction theSamplingMultiplierFunction;
    QString theVariableOfInterest;
    QString theSamplingCentre;
    QList<QPointer<RParameter>> theInputParameterList;
    QPointer<RRandomNumberGenerator> theRandomNumberGenerator;
    QPointer<RProbabilityTransformation> theProbabilityTransformation;
    QVector<double> theMagnitudes;
    QVector<QVector<double>> theRecoveryTime;
    QCustomPlot *theCustomPlot;


};

#endif
