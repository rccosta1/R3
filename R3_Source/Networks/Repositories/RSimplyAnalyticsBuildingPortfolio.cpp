#include "RSimplyAnalyticsBuildingPortfolio.h"
#include "RParameter.h"
#include "RMainWindow.h"
#include "RNormalDistribution.h"
#include "RPolygonalAreaSourceModel.h"
#include "RLocation.h"
#include <math.h>


RSimplyAnalyticsBuildingPortfolio::RSimplyAnalyticsBuildingPortfolio(QObject *parent, QString name)
    : RBuildingPortfolio(parent, name)
{
    theLatitudeRandomVariable = nullptr;
    theNumberOfBuildings = 0;
    theRetrofitLevel = 0.0;
    
    this->setMunicipality(QString("SanFrancisco"));
}


RSimplyAnalyticsBuildingPortfolio::~RSimplyAnalyticsBuildingPortfolio()
{
    
}


int RSimplyAnalyticsBuildingPortfolio::resetTime()
{
    if (theLatitudeRandomVariable) {
        delete theLatitudeRandomVariable;
    }
    theLatitudeRandomVariable = nullptr;



    
    return 0;
}





int RSimplyAnalyticsBuildingPortfolio::instantiateBuildings()
{
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }
    


    return 1;
}

