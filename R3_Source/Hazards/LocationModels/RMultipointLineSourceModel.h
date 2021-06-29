#ifndef RMultipointLineSourceModel_H	
#define RMultipointLineSourceModel_H	
	
#include "RModel.h"	
#include "RContinuousRandomVariable.h"
	
class RParameter;
class RLocation;
class RLocationResponse;
	
class RMultipointLineSourceModel : public RModel	
{	
	Q_OBJECT
    Q_PROPERTY(QString PointLocationList READ getPointLocationList WRITE setPointLocationList)
	Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)
	
public:	
	RMultipointLineSourceModel(QObject *parent, QString name);
	~RMultipointLineSourceModel();

	int evaluateModel();
	
    QString getPointLocationList();
    void setPointLocationList(QString value);
	QObject *getDepth() const;
	void setDepth(QObject *value);

	QList<RParameter *> getPhysicalParameterList();

	QStringList getActionList();
	void reset();

private slots:
	void displayonMap();
	void removefromMap();

private:	
    QList<QPointer<RParameter> > thePointLocationList;
    QPointer<RContinuousRandomVariable> theTheta;
	QPointer<RParameter> theDepth;

	double theFaultLength;
	QVector<double> thePieceLengthVector;

	// Declare responses
    RLocationResponse *theEpicentreLocation;
};	

#endif	
