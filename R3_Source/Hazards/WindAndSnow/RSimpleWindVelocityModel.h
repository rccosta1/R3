#ifndef RSimpleWindVelocityModel_H
#define RSimpleWindVelocityModel_H

#include "RModel.h"
#include "RProbabilityDistributions.h"


class RParameter;
class RResponse;

class RSimpleWindVelocityModel : public RModel
{
	Q_OBJECT

	Q_PROPERTY(QObject *Theta READ getTheta WRITE setTheta)
	Q_PROPERTY(QObject *V10 READ getV10 WRITE setV10)
	Q_PROPERTY(QObject *V50 READ getV50 WRITE setV50)
    Q_PROPERTY(QObject *ProbabilityDistributions READ getProbabilityDistributions WRITE setProbabilityDistributions)


public:
	// Define the constructor and destructor
	RSimpleWindVelocityModel(QObject *parent, QString name);
	~RSimpleWindVelocityModel();

	// Define the mandatory read/write methods for all QProperties
	QObject *getTheta() const;
	void setTheta(QObject *value);
	QObject *getV10() const;
	void setV10(QObject *value);
	QObject *getV50() const;
	void setV50(QObject *value);
    QObject *getProbabilityDistributions() const;
    void setProbabilityDistributions(QObject *value);

	QList<RParameter *> getPhysicalParameterList();

	// Define the method where the actual model is coded
	int evaluateModel(bool evaluateDDM, RObject::RParameterType parameterType);

private:
	QPointer<RParameter> theTheta;
	QPointer<RParameter> theV10;
	QPointer<RParameter> theV50;
    QPointer<RProbabilityDistributions> theProbabilityDistributions;

	
	// Declare responses
	RResponse *theWindSpeed;

};

#endif // RSimpleWindVelocityModel_H
