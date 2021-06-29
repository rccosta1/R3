#ifndef RCommunityModel_H
#define RCommunityModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RCommunityModel : public RNetworkModel
{
	Q_OBJECT

    Q_PROPERTY(double DailyDemand READ getDailyDemand WRITE setDailyDemand)
    Q_PROPERTY(QString Contracts READ getContracts WRITE setContracts)
    Q_PROPERTY(QString FuelSupplierList READ getFuelSupplierList WRITE setFuelSupplierList)

public:

    RCommunityModel(QObject *parent, QString name);
	~RCommunityModel();

    int resetTime();

    QString getContracts();
    void setContracts(QString value);

    double getDailyDemand() const;
    void setDailyDemand(double value);

    QString getFuelSupplierList();
    void setFuelSupplierList(QString value);

	int evaluateModel();

private:

    double theDailyDemand;
    double lastEvaluationTime;

    QList<QPointer<RParameter> > theContracts;
    QList<QPointer<RParameter> > theFuelSupplierList;

    RResponse *theResponse;
};

#endif
