#ifndef RBridgeModel_H
#define RBridgeModel_H

#include "RModel.h"
#include "RProbabilityDistributions.h"
#include "RContinuousRandomVariable.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RBridgeModel : public RNetworkModel
{
	Q_OBJECT

	// Define a QProperty for each input (parameter) data member. The type of the QProperty is "QObject *"
    Q_PROPERTY(RAnalysisType AnalysisType READ getAnalysisType WRITE setAnalysisType)
	Q_PROPERTY(RBridgeType BridgeType READ getBridgeType WRITE setBridgeType)
    Q_PROPERTY(QString SaIntensityList READ getSaIntensityList WRITE setSaIntensityList)
    Q_PROPERTY(QString PGDIntensityList READ getPGDIntensityList WRITE setPGDIntensityList)
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)
    Q_PROPERTY(int NumberOfSpans READ getNumberOfSpans WRITE setNumberOfSpans)
	Q_PROPERTY(double AngleOfSkew READ getAngleOfSkew WRITE setAngleOfSkew)
	Q_PROPERTY(double BridgeLength READ getBridgeLength WRITE setBridgeLength)
	Q_PROPERTY(double SpanWidth READ getSpanWidth WRITE setSpanWidth)


	Q_ENUMS(RBridgeType)
	Q_ENUMS(RAnalysisType)

public:
	// Define the constructor and destructor
    RBridgeModel(QObject *parent, QString name);
    ~RBridgeModel();

    int resetTime();

	enum RBridgeType{HWB1, HWB2, HWB3, HWB4, HWB5, HWB6, HWB7, HWB8, HWB9, HWB10, HWB11, HWB12, HWB13, HWB14, HWB15, HWB16, HWB17, HWB18, HWB19, HWB20, HWB21, HWB22, HWB23, HWB24, HWB25, HWB26, HWB27, HWB28};
	enum RAnalysisType{GroundShaking, GroundFailure, Both};

    RAnalysisType getAnalysisType() const;
	void setAnalysisType(RAnalysisType value);

    RBridgeType getBridgeType() const;
	void setBridgeType(RBridgeType value);

    QString getSaIntensityList();
    void setSaIntensityList(QString value);

    QString getPGDIntensityList();
    void setPGDIntensityList(QString value);

    QString getInputParameterList();
    void setInputParameterList(QString value);

    int getNumberOfSpans() const;
	void setNumberOfSpans(int value);

    double getAngleOfSkew() const;
	void setAngleOfSkew(double value);

    double getBridgeLength() const;
	void setBridgeLength(double value);

    double getSpanWidth() const;
	void setSpanWidth(double value);

    QObject *getProbabilityDistributions() const;
    void setProbabilityDistributions(QObject *value);

	// Define the method where the actual model is coded
	int evaluateModel();


private:

    // Data members
    RAnalysisType theAnalysisType;
	RBridgeType theBridgeType;
    QList<QPointer<RParameter> > theSaIntensityList;
    QList<QPointer<RParameter> > thePGDIntensityList;
    QList<QPointer<RParameter> > theInputParameterList;
    QVector<double> timePacketsCurrentlyOnboardWereReceived;
    int theNumberOfSpans;
	double theAngleOfSkew;
	double theBridgeLength;
	double theSpanWidth;
    double theTimeUnderRepairs;
    QPointer<RContinuousRandomVariable> theModelUncertainty;


	// Declare responses
	RResponse *thePercentFunctional;
    RResponse *theDamageFactor;

    double calculateTimeUntilGivenRestorationRatio(double ratio);
    QVector<double> probabilityVector;
    int tempCounter;
    double Sa1 = 0.0;
    double Sa0_3 = 0.0;
    double PGD = 0.0;

    RResponse *theResponse;
};

#endif // RBridgeModel_H
