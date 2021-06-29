#ifndef RSyntheticGroundMotionModel_H
#define RSyntheticGroundMotionModel_H

#include "RModel.h"
#include "RTimeSeriesResponse.h"
#include "RContinuousRandomVariable.h"
#include "RParameter.h"
#include "RRandomNumberGenerator.h"



class RSyntheticGroundMotionModel : public RModel
{
    Q_OBJECT
    
    Q_PROPERTY(QString GammaModulatingFunctionParameterAList READ getGammaModulatingFunctionParameterAList WRITE setGammaModulatingFunctionParameterAList)
    Q_PROPERTY(QString GammaModulatingFunctionParameterBList READ getGammaModulatingFunctionParameterBList WRITE setGammaModulatingFunctionParameterBList)
    Q_PROPERTY(QString GammaModulatingFunctionParameterCList READ getGammaModulatingFunctionParameterCList WRITE setGammaModulatingFunctionParameterCList)

    Q_PROPERTY(QString FilterDampingList READ getFilterDampingList WRITE setFilterDampingList)
    Q_PROPERTY(QString FilterFrequencyList READ getFilterFrequencyList WRITE setFilterFrequencyList)

    Q_PROPERTY(double NumberOfRandomVariables READ getNumberOfRandomVariables WRITE setNumberOfRandomVariables)
    Q_PROPERTY(double StartTime READ getStartTime WRITE setStartTime)
    Q_PROPERTY(double EndTime READ getEndTime WRITE setEndTime)


public:
    RSyntheticGroundMotionModel(QObject *parent, QString name);
    ~RSyntheticGroundMotionModel();

    QString getGammaModulatingFunctionParameterAList();
    void setGammaModulatingFunctionParameterAList(QString value);


    QString getGammaModulatingFunctionParameterBList();
    void setGammaModulatingFunctionParameterBList( QString value);

    QString getGammaModulatingFunctionParameterCList();
    void setGammaModulatingFunctionParameterCList( QString value);


    QString getFilterDampingList();
    void setFilterDampingList( QString value);

    QString getFilterFrequencyList();
    void setFilterFrequencyList( QString value);

    double getNumberOfRandomVariables() const;
    void setNumberOfRandomVariables(double value);

    double getStartTime() const;
    void setStartTime(double value);

    double getEndTime() const;
    void setEndTime(double value);

    QList<RParameter *> getAllParametersList(bool *ok);

    QStringList getActionList();


    int evaluateModel();

private:

    RResponse *theGroundMotionResponse;

    QList<QPointer<RParameter> > theGammaModulatingFunctionParameterAList;
    QList<QPointer<RParameter> > theGammaModulatingFunctionParameterBList;
    QList<QPointer<RParameter> > theGammaModulatingFunctionParameterCList;

    QList<QPointer<RParameter> > theFilterDampingList;
    QList<QPointer<RParameter> > theFilterFrequencyList;

    double theNumberOfRandomVariables;
    double theStartTime;
    double theEndTime;

    QList<RContinuousRandomVariable*> *theRVlist;
};

#endif
