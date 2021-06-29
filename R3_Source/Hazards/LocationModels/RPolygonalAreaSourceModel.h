#ifndef RPolygonalAreaSourceModel_H	
#define RPolygonalAreaSourceModel_H	
	
#include "RModel.h"	
#include "RContinuousRandomVariable.h"
	
class RParameter;	
class RLocationResponse;
class RLocation;
class RContinuousRandomVariable;
	
class RPolygonalAreaSourceModel : public RModel	
{	
	Q_OBJECT
    Q_PROPERTY(QString BoundaryLocationList READ getBoundaryLocationList WRITE setBoundaryLocationList)
	Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)

public:	
	RPolygonalAreaSourceModel(QObject *parent, QString name);
	~RPolygonalAreaSourceModel();

	int evaluateModel();
	
    QString getBoundaryLocationList();
    void setBoundaryLocationList(QString value);

	QObject *getDepth() const;
	void setDepth(QObject *value);

	QList<RParameter *> getPhysicalParameterList();
	void reset();

	QStringList getActionList();

private slots:
	void displayonMap();
	void removefromMap();
	
private:	
    QList<QPointer<RParameter> > theBoundaryLocationList;
    QPointer<RContinuousRandomVariable> theTheta1;
    QPointer<RContinuousRandomVariable> theTheta2;
	QPointer<RParameter> theDepth;

	// Declare responses
    RLocationResponse *theHypocentreLocation;

	// Internal data members
	RContinuousRandomVariable *theLatitudeRandomVariable;

};	

#endif	
