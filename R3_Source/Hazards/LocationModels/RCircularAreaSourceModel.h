#ifndef RCircularAreaSourceModel_H	
#define RCircularAreaSourceModel_H	
	
#include "RModel.h"	
	
class RParameter;	
class RLocationResponse;
class RLocation;
	
class RCircularAreaSourceModel : public RModel	
{	
	Q_OBJECT
    Q_PROPERTY(QObject *CentreLocation READ getCentreLocation WRITE setCentreLocation)
	Q_PROPERTY(double MinimumRadius READ getMinimumRadius WRITE setMinimumRadius)
	Q_PROPERTY(double MaximumRadius READ getMaximumRadius WRITE setMaximumRadius)
	Q_PROPERTY(QObject *Theta1 READ getTheta1 WRITE setTheta1)
	Q_PROPERTY(QObject *Theta2 READ getTheta2 WRITE setTheta2)
	Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)

public:	
	RCircularAreaSourceModel(QObject *parent, QString name);
	~RCircularAreaSourceModel();

	int evaluateModel();

    QObject *getCentreLocation() const;
    void setCentreLocation(QObject *value);
	double getMinimumRadius() const;
	void setMinimumRadius(double value);
	double getMaximumRadius() const;
	void setMaximumRadius(double value);
	QObject *getTheta1() const;
	void setTheta1(QObject *value);
	QObject *getTheta2() const;
	void setTheta2(QObject *value);
	QObject *getDepth() const;
	void setDepth(QObject *value);

	QList<RParameter *> getPhysicalParameterList();

	QStringList getActionList();

private slots:
	void displayonMap();
	void removefromMap();
	
private:	
    QPointer<RParameter> theCentreLocation;
	double theMinimumRadius;
	double theMaximumRadius;
	QPointer<RParameter> theTheta1;
	QPointer<RParameter> theTheta2;
	QPointer<RParameter> theDepth;

	// Declare responses
    RLocationResponse *theHypocentreLocation;

};	

#endif	
