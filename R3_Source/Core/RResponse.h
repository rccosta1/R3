#ifndef RResponse_H
#define RResponse_H

#include "RParameter.h"
#include "RDomain.h"

class RModel;

class RResponse : public RParameter
{
	Q_OBJECT

	// Current realization of the response
	Q_PROPERTY(double CurrentValue READ getCurrentValue WRITE setCurrentValue)

	// Sets the model object for which the response is an output
	Q_PROPERTY(QObject *Model READ getModel WRITE setModel)

	// Read-only property that shows the total stdv of the response including contributions from ALL Aleatory and Epistemic random variables that implicitly or explicitly affect the response
	Q_PROPERTY(double TotalStandardDeviation READ getTotalStandardDeviation)

	// Read-only property that shows the derivative of the reliability index wrt. the total stdv of the response including contributions from ALL Aleatory and Epistemic random variables that implicitly or explicitly affect the response
	Q_PROPERTY(double BetaSensitivityWrtTotalStandardDeviation READ getBetaSensitivityWrtTotalStandardDeviation)

	// Read-only property that shows the total stdv of the response including contributions from ALL Epistemic random variables that implicitly or explicitly affect the response
	Q_PROPERTY(double TotalEpistemicStandardDeviation READ getTotalEpistemicStandardDeviation)

	// Read-only property that shows the derivative of the reliability index wrt. the total stdv of the response including contributions from ALL Epistemic random variables that implicitly or explicitly affect the response
	Q_PROPERTY(double BetaSensitivityWrtTotalEpistemicStandardDeviation READ getBetaSensitivityWrtTotalEpistemicStandardDeviation)

	// Read-only property that shows the total stdv of the response including contributions from only those Aleatory and Epistemic random variables that explicitly affect the response
	Q_PROPERTY(double ModelStandardDeviation READ getModelStandardDeviation)

	// Read-only property that shows the derivative of the reliability index wrt. the total stdv of the response including contributions from only those Aleatory and Epistemic random variables that explicitly affect the response
	Q_PROPERTY(double BetaSensitivityWrtModelStandardDeviation READ getBetaSensitivityWrtModelStandardDeviation)

	// Read-only property that shows the total stdv of the response including contributions from only those Epistemic random variables that explicitly affect the response
	Q_PROPERTY(double ModelEpistemicStandardDeviation READ getModelEpistemicStandardDeviation)

	// Read-only property that shows the derivative of the reliability index wrt. the total stdv of the response including contributions from only those Epistemic random variables that explicitly affect the response
	Q_PROPERTY(double BetaSensitivityWrtModelEpistemicStandardDeviation READ getBetaSensitivityWrtModelEpistemicStandardDeviation)

public:
	RResponse(QObject *parent, QString name);
	virtual ~RResponse();
	
	// Specific implementation of setCurrentValue for RResponse that sets the isAnalyzed flag to true
	void setCurrentValue(double value);

	// getter/setter methods for meta-properties
	QObject *getModel() const;
	void setModel(QObject *value);
	double getTotalStandardDeviation() const;
	void setTotalStandardDeviation(double value);
	double getBetaSensitivityWrtTotalStandardDeviation() const;
	void setBetaSensitivityWrtTotalStandardDeviation(double value);
	double getTotalEpistemicStandardDeviation() const;
	void setTotalEpistemicStandardDeviation(double value);
	double getBetaSensitivityWrtTotalEpistemicStandardDeviation() const;
	void setBetaSensitivityWrtTotalEpistemicStandardDeviation(double value);
	double getModelStandardDeviation() const;
	void setModelStandardDeviation(double value);
	double getBetaSensitivityWrtModelStandardDeviation() const;
	void setBetaSensitivityWrtModelStandardDeviation(double value);
	double getModelEpistemicStandardDeviation() const;
	void setModelEpistemicStandardDeviation(double value);
	double getBetaSensitivityWrtModelEpistemicStandardDeviation() const;
	void setBetaSensitivityWrtModelEpistemicStandardDeviation(double value);

	// Sets the isAnalyzed flag
	bool isAnalyzed();
	void setAnalyzed(bool analyzed);

	// Backs up the current value(s) of the response
	virtual void backup();

	// Restores the current value(s) of the response from the backup
	virtual void restore();

	// Returns the vector of response values, used in DDM computations; In case of ordinary responses, such as generic, file, or command response, the vector has only one element, which is the CurrentValue; In case of a location response, it has three elements: latitude, longitude, and altitude
	virtual QVector<double> getValueVector();

	// Sets the vector of response values as explained above
	virtual void setValueVector(QVector<double> valueVector);

	// Returns the backup vector of response values as explained above
	virtual QVector<double> getBackupVector();

	// Invokes the operation of updating the response value by, e.g., reading files for a file response, or issuing commands to the external software application for a command response; It does nothing for a generic response
	virtual int updateValue();

private:
	// Data member for meta-properties
	QPointer<RModel> theModel;
	double theTotalStandardDeviation;
	double theBetaSensitivityWrtTotalStandardDeviation;
	double theTotalEpistemicStandardDeviation;
	double theBetaSensitivityWrtTotalEpistemicStandardDeviation;
	double theModelStandardDeviation;
	double theBetaSensitivityWrtModelStandardDeviation;
	double theModelEpistemicStandardDeviation;
	double theBetaSensitivityWrtModelEpistemicStandardDeviation;

	// Data member that shows if the model for this response needs to be analyzed
	bool isAnalyzedFlag;

	// Backup of the CurrentValue
	double backupValue;
	
	// A QMap object that stores the DDM response sensitivities
	QMap<RParameter *, double> theGradientMap;

    // A QMap object that stores the second order DDM response sensitivities
    QMap<RParameter *, double> theSecondOrderGradientMap;

    // A QMap object that stores the second order DDM response sensitivities
    QMap<RParameter *, double> theThirdOrderGradientMap;
	
};

#endif // RResponse_H
