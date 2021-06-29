#ifndef RGrantProviderModel_H
#define RGrantProviderModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RGrantProviderModel : public RNetworkModel
{
    Q_PROPERTY(double MedianDelayForPayments READ getMedianDelayForPayments WRITE setMedianDelayForPayments)
    Q_PROPERTY(double DispersionOfDelayForPayments READ getDispersionOfDelayForPayments WRITE setDispersionOfDelayForPayments)
    Q_PROPERTY(double DelayMultiplierForSmallRequests READ getDelayMultiplierForSmallRequests WRITE setDelayMultiplierForSmallRequests)
    Q_PROPERTY(double SmallClaimThreshold READ getSmallClaimThreshold WRITE setSmallClaimThreshold)
    Q_PROPERTY(double DelayForOwnerOccupiedBuildings READ getDelayForOwnerOccupiedBuildings WRITE setDelayForOwnerOccupiedBuildings)
    Q_PROPERTY(double DelayForRenterOccupiedBuildings READ getDelayForRenterOccupiedBuildings WRITE setDelayForRenterOccupiedBuildings)
    Q_OBJECT


public:

    RGrantProviderModel(QObject *parent, QString name);
    ~RGrantProviderModel();

    double getMedianDelayForPayments();
    void setMedianDelayForPayments(double value);

    double getDispersionOfDelayForPayments();
    void setDispersionOfDelayForPayments(double value);

    double getDelayMultiplierForSmallRequests();
    void setDelayMultiplierForSmallRequests(double value);

    double getSmallClaimThreshold();
    void setSmallClaimThreshold(double value);

    double getDelayForOwnerOccupiedBuildings();
    void setDelayForOwnerOccupiedBuildings(double value);

    double getDelayForRenterOccupiedBuildings();
    void setDelayForRenterOccupiedBuildings(double value);

    int resetTime();
    void getRequests();

    int evaluateModel();

public slots:


private:

    // Data members
    QVector<QVector<QString>> theRequesterOccupancyType;

    QList<QPointer<RParameter> > theInputParameterList;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<double>> thePacketProcessingTimeList;

    QVector<QVector<double>> timeRequestsWereReceived;

    double theMedianDelayForPayments;
    double theDispersionOfDelayForPayments;
    double theDelayMultiplierForSmallRequests;
    double theSmallClaimThreshold;
    double totalMoneyProvided;
    double theDelayForOwnerOccupied;
    double theDelayForRenterOccupied;

    RResponse *theResponse;

};

#endif
