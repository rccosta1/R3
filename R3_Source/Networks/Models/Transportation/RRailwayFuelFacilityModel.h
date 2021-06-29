#ifndef RRailwayFuelFacilityModel_H
#define RRailwayFuelFacilityModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RRailwayFuelFacilityModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(int FacilityType READ getFacilityType WRITE setFacilityType)

public:

    RRailwayFuelFacilityModel(QObject *parent, QString name);
    ~RRailwayFuelFacilityModel();

    int getFacilityType() const;
    void setFacilityType(int value);

    int evaluateModel();

protected:
    int theFacilityType;

private:

    double theTimeUnderRepairs;
    RResponse *theResponse;
};

#endif
