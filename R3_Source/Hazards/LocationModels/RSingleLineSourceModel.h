#ifndef RSingleLineSourceModel_H	
#define RSingleLineSourceModel_H	
	
#include "RModel.h"	
	
class RParameter;
class RLocation;
class RLocationResponse;
	
class RSingleLineSourceModel : public RModel	
{	
	Q_OBJECT

    Q_PROPERTY(QObject *Point1Location READ getPoint1Location WRITE setPoint1Location)
    Q_PROPERTY(QObject *Point2Location READ getPoint2Location WRITE setPoint2Location)
	Q_PROPERTY(QObject *Theta READ getTheta WRITE setTheta)
	Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)
	
public:	
	RSingleLineSourceModel(QObject *parent, QString name);
	~RSingleLineSourceModel();

	int evaluateModel();

    QObject *getPoint1Location() const;
    void setPoint1Location(QObject *value);
    QObject *getPoint2Location() const;
    void setPoint2Location(QObject *value);
	QObject *getTheta() const;
	void setTheta(QObject *value);
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
	QPointer<RParameter> theTheta;
	QPointer<RParameter> theDepth;

	// Declare responses
    RLocationResponse *theHypocentreLocation;
};	

#endif	
