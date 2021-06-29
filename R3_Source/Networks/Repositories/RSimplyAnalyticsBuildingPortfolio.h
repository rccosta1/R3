#ifndef RSimplyAnalyticsBuildingPortfolio_H
#define RSimplyAnalyticsBuildingPortfolio_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RRequest.h"
#include "RNetworkModel.h"
#include "RBuildingPortfolio.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RSimplyAnalyticsBuildingPortfolio : public RBuildingPortfolio
{ 

    Q_OBJECT

public:

    RSimplyAnalyticsBuildingPortfolio(QObject *parent, QString name);
    ~RSimplyAnalyticsBuildingPortfolio();

    int resetTime();

    // Instantiators
    int instantiateBuildings();


protected:


private: 

    // DATA MEMBERS
    QPair<double,double>  instantiateBuildingLocation();

};

#endif
