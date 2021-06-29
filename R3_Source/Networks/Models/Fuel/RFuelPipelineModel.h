#ifndef RFuelPipelineModel_H
#define RFuelPipelineModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RFuelPipelineModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RPipelineType PipelineType READ getPipelineType WRITE setPipelineType)
    Q_PROPERTY(double Diameter READ getDiameter WRITE setDiameter)
    Q_PROPERTY(double PipelineLength READ getPipelineLength WRITE setPipelineLength)
    Q_PROPERTY(double Workforce READ getWorkforce WRITE setWorkforce)
    Q_PROPERTY(QString FuelSupplierList READ getFuelSupplierList WRITE setFuelSupplierList)


    Q_ENUMS(RPipelineType)
public:

    RFuelPipelineModel(QObject *parent, QString name);
    ~RFuelPipelineModel();

    int resetTime();

    enum RPipelineType{NoDamage, Ductile, Brittle};

    RPipelineType getPipelineType() const;
    void setPipelineType(RPipelineType value);

    double getDiameter() const;
    void setDiameter(double value);

    double getPipelineLength() const;
    void setPipelineLength(double value);

    int getWorkforce() const;
    void setWorkforce(int value);

    QString getFuelSupplierList();
    void setFuelSupplierList(QString value);

    int evaluateModel();

protected:
    RPipelineType thePipelineType;

private:

    // Data members
    QList<QPointer<RParameter> > theFuelSupplierList;

    bool theAvailabilityOfFuel;
    int theWorkforce;
    int numberOfLeaks;
    int numberOfBreaks;
    double theVulnerabilityFactor;
    double theDiameter;
    double thePipelineLength;


    RResponse *theResponse;
};

#endif
