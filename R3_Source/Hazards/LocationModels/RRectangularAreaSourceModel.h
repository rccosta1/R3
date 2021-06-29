#ifndef RRectangularAreaSourceModel_H	
#define RRectangularAreaSourceModel_H	
	
#include "RModel.h"	

class RParameter;	
class RLocationResponse;
class RLocation;
	
class RRectangularAreaSourceModel : public RModel	
{	
	Q_OBJECT
    Q_PROPERTY(QObject *Point1Location READ getPoint1Location WRITE setPoint1Location)
    Q_PROPERTY(QObject *Point2Location READ getPoint2Location WRITE setPoint2Location)
	Q_PROPERTY(QObject *Theta1 READ getTheta1 WRITE setTheta1)
	Q_PROPERTY(QObject *Theta2 READ getTheta2 WRITE setTheta2)
	Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)

public:	
	RRectangularAreaSourceModel(QObject *parent, QString name);
	~RRectangularAreaSourceModel();

	int evaluateModel();

    QObject *getPoint1Location() const;
    void setPoint1Location(QObject *value);
    QObject *getPoint2Location() const;
    void setPoint2Location(QObject *value);
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
    QPointer<RParameter> thePoint1Location;
    QPointer<RParameter> thePoint2Location;
	QPointer<RParameter> theTheta1;
	QPointer<RParameter> theTheta2;
	QPointer<RParameter> theDepth;

	// Declare responses
    RLocationResponse *theHypocentreLocation;

};	

#endif	
