#ifndef RShippingChannel_H
#define RShippingChannel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RShippingChannelModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(RChannelType ChannelType READ getChannelType WRITE setChannelType)
    Q_PROPERTY(QString BottleneckList READ getBottleneckList WRITE setBottleneckList)
    Q_PROPERTY(QObject *FromLocation READ getFromLocation WRITE setFromLocation)
    Q_PROPERTY(QObject *ToLocation READ getToLocation WRITE setToLocation)
    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)
    Q_PROPERTY(QString PGDIntensityList READ getPGDIntensityList WRITE setPGDIntensityList)

    Q_ENUMS(RChannelType)
public:

    RShippingChannelModel(QObject *parent, QString name);
    ~RShippingChannelModel();

    int resetTime();

    enum RChannelType{NoDamage, Generic};
    RChannelType getChannelType() const;
    void setChannelType(RChannelType value);

    QString getBottleneckList();
    void setBottleneckList(QString value);

    QString getInputParameterList();
    void setInputParameterList(QString value);

    QObject *getFromLocation() const;
    void setFromLocation(QObject *value);

    QObject *getToLocation() const;
    void setToLocation(QObject *value);

    QString getPGDIntensityList();
    void setPGDIntensityList(QString value);

    void setFragilityCurves(int value);

    int evaluateModel();

protected:
    RChannelType theChannelType;

private:
    // Data members
    QList<QPointer<RParameter> > theInputParameterList;
    QList<QPointer<RParameter> > thePGDIntensityList;
    QList<QPointer<RParameter> > theBottleneckList;

    QPointer<RParameter> theFromLocation;
    QPointer<RParameter> theToLocation;

    QVector<double> timePacketsCurrentlyOnboardWereReceived;

    RResponse *theResponse;
};

#endif
