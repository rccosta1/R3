#ifndef RContractorNegotiationModel_H
#define RContractorNegotiationModel_H

#include "RNetworkModel.h"
#include "RMainWindow.h"
#include "RResponse.h"
#include "RModel.h"
#include "RPacket.h"
#include <QDateTime>
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RContractorNegotiationModel : public RNetworkModel
{
    Q_OBJECT

public:
    RContractorNegotiationModel(QObject *parent, QString name);
    ~RContractorNegotiationModel();

    int resetTime();

    void getRequests();

    int evaluateModel();

public slots:


private:   
    // Data members
    QVector<QVector<int>> theBuildingRepairClass;
    QVector<QVector<int>> theBuildingNumberOfStoreys;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<double>> theBiddingDelayList;

    QVector<QVector<double>> timeRequestsWereReceived;

    RResponse *theResponse;
};

#endif
