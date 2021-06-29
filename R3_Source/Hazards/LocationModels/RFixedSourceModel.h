#ifndef RFixedSourceModel_H	
#define RFixedSourceModel_H	
	
#include "RModel.h"	
	
class RParameter;
class RLocation;
class RLocationResponse;
	
class RFixedSourceModel : public RModel	
{	
	Q_OBJECT

    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)
    Q_PROPERTY(QObject *Depth READ getDepth WRITE setDepth)

public:	
	RFixedSourceModel(QObject *parent, QString name);
	~RFixedSourceModel();

	int evaluateModel();

    QObject *getLocation() const;
    void setLocation(QObject *value);

    QObject *getDepth() const;
    void setDepth(QObject *value);

	QStringList getActionList();

private slots:

	void displayonMap();
	void removefromMap();

private:	

    QPointer<RParameter> theLocation;
    RLocationResponse *theHypocentreLocation;
    QPointer<RParameter> theDepth;
};

#endif	
