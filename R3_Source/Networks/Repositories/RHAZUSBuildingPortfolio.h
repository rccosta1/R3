#ifndef RHAZUSBuildingPortfolio_H
#define RHAZUSBuildingPortfolio_H

#include "RModel.h"
#include "RResponse.h"
#include "RBuildingPortfolio.h"

class RParameter;
class RResponse;

class RHAZUSBuildingPortfolio : public RBuildingPortfolio
{ 

    Q_OBJECT

public:

    RHAZUSBuildingPortfolio(QObject *parent, QString name);
    ~RHAZUSBuildingPortfolio();

    int resetTime();

    // Instantiators
    int instantiateBuildings();

    // Extractors
//    int getNumberOfBuildings() const;
//    int getNumberOfSingleFamilyBuildings() const;
//    int getNumberOfMultiFamilyBuildings() const;

//    QVector<QString> getBuildingArchetypeVector() const;
//    QVector<QString> getBuildingTypesVector() const;
//    QVector<QString> getBuildingCodeLevelVector() const;
//    QVector<QString> getBuildingOccupancyClassVector() const;
//    QVector<int> getBuildingStoreysVector() const;
//    QVector<int> getBuildingDwellingsVector() const;
//    QVector<double> getBuildingAreaVector() const;
//    QVector<double> getBuildingValueVector() const;

protected:


private: 

    // DATA MEMBERS
    QPair<double,double>  instantiateBuildingLocation();

};

#endif
