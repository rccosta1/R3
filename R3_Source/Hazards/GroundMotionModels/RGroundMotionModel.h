#ifndef RGroundMotionModel_H	
#define RGroundMotionModel_H	
	
#include "RModel.h"	
	
class RParameter;	
class RResponse;
class RLocation;
	
class RGroundMotionModel : public RModel	
{	
	Q_OBJECT

public:	

    RGroundMotionModel(QObject *parent, QString name);
	~RGroundMotionModel();

	int evaluateModel();


    QStringList getActionList();

private slots:

    void plotAcceleration();
    void plotVelocity();
    void plotDisplacement();


private:	
	
};	
	
#endif // RGroundMotionModel_H	
