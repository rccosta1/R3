#ifndef RBuildingPortfolio_H
#define RBuildingPortfolio_H

#include "RModel.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"

class RParameter;
class RResponse;

class RBuildingPortfolio : public RObject
{ 
    Q_PROPERTY(QString Municipality READ getMunicipality WRITE setMunicipality)
    Q_PROPERTY(double MedianHouseholdIncome READ getMedianHouseholdIncome WRITE setMedianHouseholdIncome)
    Q_PROPERTY(QString BuildingTypes READ getBuildingTypes WRITE setBuildingTypes)
    Q_PROPERTY(QString BuildingOccupancyClasses READ getBuildingOccupancyClasses WRITE setBuildingOccupancyClasses)
    Q_PROPERTY(QString BuildingNumberPerType READ getBuildingNumberPerType WRITE setBuildingNumberPerType)
    Q_PROPERTY(QString BuildingCodeLevel READ getBuildingCodeLevel WRITE setBuildingCodeLevel)
    Q_PROPERTY(QString BuildingAreas READ getBuildingAreas WRITE setBuildingAreas)
    Q_PROPERTY(double BuildingValues READ getBuildingValues WRITE setBuildingValues)
    Q_PROPERTY(QString BuildingDensity READ getBuildingDensity WRITE setBuildingDensity)
    Q_PROPERTY(double RetrofitLevel READ getRetrofitLevel WRITE setRetrofitLevel)
    Q_PROPERTY(QString RoomsDistribution READ getRoomsDistribution WRITE setRoomsDistribution)

    Q_OBJECT

public:

    RBuildingPortfolio(QObject *parent, QString name);
    ~RBuildingPortfolio();

    // Attributes
    double getMedianHouseholdIncome() const;
    void setMedianHouseholdIncome(double value);

    QString getMunicipality() const;
    void setMunicipality(QString value);

    QString getBuildingTypes();
    void setBuildingTypes(QString value);

    QString getBuildingOccupancyClasses();
    void setBuildingOccupancyClasses(QString value);

    QString getBuildingCodeLevel();
    void setBuildingCodeLevel(QString value);

    QString getBuildingNumberPerType();
    void setBuildingNumberPerType(QString value);

    QString getBuildingAreas();
    void setBuildingAreas(QString value);

    double getBuildingValues();
    void setBuildingValues(double value);

    QString getBuildingDensity();
    void setBuildingDensity(QString value);

    double getRetrofitLevel();
    void setRetrofitLevel(double value);

    QString getRoomsDistribution();
    void setRoomsDistribution(QString value);

    int resetTime();

    // Instantiators
    virtual int instantiateBuildings();

    // Extractors
    int getNumberOfBuildings() const;
    QVector<QString> getBuildingArchetypeVector() const;
    QVector<QString> getBuildingTypesVector() const;
    QVector<int> getYearBuiltVector() const;
    QVector<QString> getBuildingCodeLevelVector() const;
    QVector<QString> getBuildingOccupancyClassVector() const;
    QVector<int> getBuildingStoreysVector() const;
    QVector<int> getBuildingDwellingsVector() const;
    QVector<int> getBuildingRoomsVector() const;
    QVector<double> getBuildingAreaVector() const;
    QVector<double> getLandValueVector() const;
    QVector<double> getBuildingValueVector() const;
    QVector<RLocation*> getBuildingLocationVector() const;


protected:

    // Input data members are provided by the user as strings
    RContinuousRandomVariable *theLatitudeRandomVariable;
    QList<RLocation *> theLocationList;

    RParameter* theLocation;
    double theMedianHouseholdIncome;
    double inputPercentageOfPre1940Buildings;
    double inputPercentageOfPre1975Buildings;
    double inputPercentageOfPost1975Buildings;
    double inputBuildingValues;
    QString inputBuildingTypes;
    QString inputBuildingOccupancyClasses;
    QString inputBuildingCodeLevels;
    QString inputNumberPerBuildingType;
    QVector<double> inputBuildingAreas;
    QString inputMunicipality;
    QString inputBuildingDensity;

    double medianMunicipalHouseholdIncome;
    double insuranceTakeUp;
    double theRetrofitLevel;

    // Properties of the buildings
    int theNumberOfBuildings;
    QVector<QString> theBuildingOccupancyClassVector;
    QVector<QString> theBuildingCodeLevelVector;
    QVector<QString> theBuildingTypesVector;
    QVector<int> theBuildingStoreysVector;
    QVector<int> theNumberOfDwellingsPerBuilding;
    QVector<int> theBuildingYearBuiltVector;
    QVector<int> theBuildingNumberOfRoomsVector;
    QVector<double> theBuildingAreasVector;
    QVector<double> theBuildingLandValueVector;
    QVector<double> theBuildingValueVector;
    QVector<double> inputRoomsDistributionVector;
    QVector<QString> theBuildingArchetypeVector;
    QVector<RLocation*> theBuildingLocationVector;
    QVector<QString> theBuildingTenureVector;

private: 


};

#endif
