#ifndef RSimCenterBuildingPortfolio_H
#define RSimCenterBuildingPortfolio_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RRequest.h"
#include "RNetworkModel.h"
#include "RBuildingPortfolio.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RSimCenterBuildingPortfolio : public RBuildingPortfolio
{ 
    Q_PROPERTY(QString Municipality READ getMunicipality WRITE setMunicipality)
    Q_PROPERTY(QString Latitudes READ getLatitudes WRITE setLatitudes)
    Q_PROPERTY(QString Longitudes READ getLongitudes WRITE setLongitudes)
    Q_PROPERTY(QString NumberOfStories READ getNumberOfStories WRITE setNumberOfStories)
    Q_PROPERTY(QString YearBuilt READ getYearBuilt WRITE setYearBuilt)
    Q_PROPERTY(QString BuildingType READ getBuildingType WRITE setBuildingType)
    Q_PROPERTY(QString BuildingOccupancy READ getBuildingOccupancy WRITE setBuildingOccupancy)
    Q_PROPERTY(QString BuildingArea READ getBuildingArea WRITE setBuildingArea)
    Q_PROPERTY(double RetrofitLevel READ getRetrofitLevel WRITE setRetrofitLevel)

    Q_OBJECT

public:

    RSimCenterBuildingPortfolio(QObject *parent, QString name);
    ~RSimCenterBuildingPortfolio();

    int resetTime();

    // Attributes
    QString getLatitudes();
    void setLatitudes(QString value);

    QString getLongitudes();
    void setLongitudes(QString value);

    QString getNumberOfStories();
    void setNumberOfStories(QString value);

    QString getYearBuilt();
    void setYearBuilt(QString value);

    QString getMunicipality();
    void setMunicipality(QString value);

    QString getBuildingType();
    void setBuildingType(QString value);

    QString getBuildingOccupancy();
    void setBuildingOccupancy(QString value);

    QString getBuildingArea();
    void setBuildingArea(QString value);

    double getRetrofitLevel();
    void setRetrofitLevel(double value);

    // Instantiators
    int instantiateBuildings();

//    int getNumberOfBuildings() const;
//    QVector<QString> getBuildingArchetypeVector() const;
//    QVector<QString> getBuildingTypesVector() const;
//    QVector<QString> getBuildingCodeLevelVector() const;
//    QVector<QString> getBuildingOccupancyClassVector() const;
//    QVector<int> getBuildingStoreysVector() const;
//    QVector<int> getBuildingDwellingsVector() const;
//    QVector<double> getBuildingAreaVector() const;
//    QVector<double> getBuildingValueVector() const;
//    QVector<RLocation*> getBuildingLocationVector() const;

protected:


private: 

    // DATA MEMBERS

    // Census data
    QString inputMunicipality;


    // Input data members are provided by the user as strings
    QVector<double> inputLatitudes;
    QVector<double> inputLongitudes;
    QVector<int> inputYearBuilt;
    QString inputBuildingTypes;
    QString inputBuildingOccupancyClasses;
    QVector<int> inputNumberOfStories;
    QVector<double> inputBuildingAreas;

    double theRetrofitLevel;
};

#endif
