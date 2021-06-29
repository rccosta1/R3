#ifndef RScenarioModel_H
#define RScenarioModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>

class RParameter;
class RResponse;

class RScenarioModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)
    Q_PROPERTY(QObject *StartTime READ getStartTime WRITE setStartTime)
    Q_PROPERTY(QObject *EndTime READ getEndTime WRITE setEndTime)
    Q_PROPERTY(QString RealInterestRate READ getRealInterestRate WRITE setRealInterestRate)
    Q_PROPERTY(QString MaxTimeGap READ getMaxTimeGap WRITE setMaxTimeGap)

public:

    RScenarioModel(QObject *parent, QString name);
    ~RScenarioModel();

    QString getInputParameterList();
    void setInputParameterList(QString value);

    QObject *getStartTime() const;
    void setStartTime(QObject *value);

    QObject *getEndTime() const;
    void setEndTime(QObject *value);

    QString getRealInterestRate();
    double getRealInterestRateValue();
    void setRealInterestRate(QString value);

    QString getMaxTimeGap();
    double getMaxTimeGapValue();
    void setMaxTimeGap(QString value);

    QStringList getActionList();

    int evaluateModel();

public slots:

    void plotModelFlowchart();

private:

    QList<QPointer<RParameter> > theInputParameterList;

    QPointer<RParameter> theStartTime;
    QPointer<RParameter> theEndTime;

    QVector<QVector<RModel *> > originalPathMatrix;

    RResponse *theResponse;

    bool aModelfinishedRepairs;
    bool theEventOccurrence;
    bool finishedRepair;

    int pathMatrixSize;
    int myCounter;

    QPointer<RParameter> theRealInterestRateParameter;
    double theRealInterestRateDouble;

    QPointer<RParameter> theMaxTimeGapParameter = nullptr;
    double theMaxTimeGapDouble = 365.0;

    double overallMaxCost = 0.0;
    int plotCounter = 0;
    int theScenarioCounter = 0;

};

#endif
