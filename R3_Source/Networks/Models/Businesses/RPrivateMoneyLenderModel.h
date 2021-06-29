#ifndef RPrivateMoneyLenderModel_H
#define RPrivateMoneyLenderModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RPrivateMoneyLenderModel : public RNetworkModel
{
    Q_PROPERTY(double MedianDelayForPayments READ getMedianDelayForPayments WRITE setMedianDelayForPayments)
    Q_PROPERTY(double DispersionOfDelayForPayments READ getDispersionOfDelayForPayments WRITE setDispersionOfDelayForPayments)
    Q_OBJECT


public:

    RPrivateMoneyLenderModel(QObject *parent, QString name);
    ~RPrivateMoneyLenderModel();

    double getMedianDelayForPayments();
    void setMedianDelayForPayments(double value);

    double getDispersionOfDelayForPayments();
    void setDispersionOfDelayForPayments(double value);

    int resetTime();
    void getRequests();

    int evaluateModel();

public slots:


private:

    // Data members
    QList<QPointer<RParameter> > theInputParameterList;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<double>> thePacketProcessingTimeList;
    QVector<QVector<QString>> theRequesterIncomeList;

    QVector<QVector<double>> timeRequestsWereReceived;

    double theMedianDelayForPayments;
    double theDispersionOfDelayForPayments;

    RResponse *theResponse;

};

#endif
