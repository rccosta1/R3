#ifndef RRailwayDispatchFacilityModel_H
#define RRailwayDispatchFacilityModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RRailwayDispatchFacilityModel : public RNetworkModel
{
	Q_OBJECT
        Q_PROPERTY(int FacilityType READ getFacilityType WRITE setFacilityType)

public:

    RRailwayDispatchFacilityModel(QObject *parent, QString name);
     ~RRailwayDispatchFacilityModel();

    int getFacilityType() const;
    void setFacilityType(int value);

    void setFragilityCurves(int value);

    int evaluateModel();

protected:
    int theFacilityType;

private:

    RResponse *theResponse;
};

#endif
