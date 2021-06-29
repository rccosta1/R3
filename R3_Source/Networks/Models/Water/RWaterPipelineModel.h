#ifndef RWaterPipelineModel_H
#define RWaterPipelineModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "RTimeSeriesResponse.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RWaterPipelineModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *Magnitude READ getMagnitude WRITE setMagnitude)
    Q_PROPERTY(QObject *FromLocation READ getFromLocation WRITE setFromLocation)
    Q_PROPERTY(QObject *ToLocation READ getToLocation WRITE setToLocation)
    Q_PROPERTY(double DuctilePipelineLength READ getDuctilePipelineLength WRITE setDuctilePipelineLength)
    Q_PROPERTY(double BritlePipelineLength READ getBritlePipelineLength WRITE setBritlePipelineLength)
    Q_PROPERTY(double Diameter READ getDiameter WRITE setDiameter)
    Q_PROPERTY(double Workforce READ getWorkforce WRITE setWorkforce)
    Q_PROPERTY(double PercentageOfLiquefiableSoil READ getPercentageOfLiquefiableSoil WRITE setPercentageOfLiquefiableSoil)
    Q_PROPERTY(QString WaterSupplierList READ getWaterSupplierList WRITE setWaterSupplierList)


    Q_ENUMS(RPipelineType)
public:

    RWaterPipelineModel(QObject *parent, QString name);
    ~RWaterPipelineModel();

    int resetTime();

    QObject * getMagnitude();
    void setMagnitude(QObject *value);

    QObject *getFromLocation() const;
    void setFromLocation(QObject *value);

    QObject *getToLocation() const;
    void setToLocation(QObject *value);

    double getDuctilePipelineLength() const;
    void setDuctilePipelineLength(double value);

    double getBritlePipelineLength() const;
    void setBritlePipelineLength(double value);

    enum RPipelineType{NoDamage, Ductile, Brittle};

    double getDiameter() const;
    void setDiameter(double value);

    double getWorkforce() const;
    void setWorkforce(double value);

    double getPercentageOfLiquefiableSoil() const;
    void setPercentageOfLiquefiableSoil(double value);

    QString getWaterSupplierList();
    void setWaterSupplierList(QString value);

    double calculatePGDIntensity(double thePGA);

    int evaluateModel();

protected:


private:

    // Data members
    QPointer<RParameter> theMagnitude;
    QPointer<RParameter> theFromLocation;
    QPointer<RParameter> theToLocation;
    RPipelineType thePipelineType;
    bool theAvailabilityOfWater;

    double theWorkforce;
    double theFractionOnLiquefiableSoil;
    int numberOfLeaks;
    int numberOfBreaks;

    double theVulnerabilityFactor;
    double theDiameter;
    double theDuctilePipelineLength;
    double theBritlePipelineLength;
    double theTimeUnderRepairs;
    double theLateralPGD;
    double theProbabilityOfLiquefaction;

    RResponse *theResponse;

    QList<QPointer<RParameter> > theWaterSupplierList;


};

#endif
