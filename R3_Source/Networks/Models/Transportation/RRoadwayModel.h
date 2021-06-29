#ifndef RRoadwayModel_H
#define RRoadwayModel_H

#include "RModel.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RRoadwayModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(RRoadType RoadType READ getRoadType WRITE setRoadType)
    Q_PROPERTY(QString BottleneckList READ getBottleneckList WRITE setBottleneckList)
    Q_PROPERTY(QObject *FromLocation READ getFromLocation WRITE setFromLocation)
    Q_PROPERTY(QObject *ToLocation READ getToLocation WRITE setToLocation)
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)


    Q_ENUMS(RRoadType)
public:

    RRoadwayModel(QObject *parent, QString name);
    ~RRoadwayModel();

    int resetTime();

    QString getInputParameterList();
    void setInputParameterList(QString value);

    QObject *getFromLocation() const;
    void setFromLocation(QObject *value);

    QObject *getToLocation() const;
    void setToLocation(QObject *value);

    enum RRoadType{NoDamage, Hrd1, Hrd2};
    RRoadType getRoadType() const;
    void setRoadType(RRoadType value);

    QString getBottleneckList();
    void setBottleneckList(QString value);

    bool isOpen();

    int evaluateModel();

protected:


private:

    // Data members
    QList<QPointer<RParameter> > theInputParameterList;
    QVector<double> timePacketsCurrentlyOnboardWereReceived;

    QPointer<RParameter> theFromLocation;
    QPointer<RParameter> theToLocation;
    RRoadType theRoadType;
    QList<QPointer<RParameter> > theBottleneckList;

    RResponse *theResponse;
};

#endif
