#ifndef RInsuranceCompanyModel_H
#define RInsuranceCompanyModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RInsuranceCompanyModel : public RNetworkModel
{
    Q_PROPERTY(double MedianDelayForPayments READ getMedianDelayForPayments WRITE setMedianDelayForPayments)
    Q_PROPERTY(double DispersionOfDelayForPayments READ getDispersionOfDelayForPayments WRITE setDispersionOfDelayForPayments)
    Q_PROPERTY(double DelayMultiplierForSmallClaims READ getDelayMultiplierForSmallClaims WRITE setDelayMultiplierForSmallClaims)
    Q_PROPERTY(double DelayMultiplierForLargeClaims READ getDelayMultiplierForLargeClaims WRITE setDelayMultiplierForLargeClaims)
    Q_PROPERTY(double SmallClaimThreshold READ getSmallClaimThreshold WRITE setSmallClaimThreshold)
    Q_PROPERTY(double LargeClaimThreshold READ getLargeClaimThreshold WRITE setLargeClaimThreshold)
    Q_OBJECT


public:

    RInsuranceCompanyModel(QObject *parent, QString name);
    ~RInsuranceCompanyModel();

    double getMedianDelayForPayments();
    void setMedianDelayForPayments(double value);

    double getDispersionOfDelayForPayments();
    void setDispersionOfDelayForPayments(double value);

    double getDelayMultiplierForSmallClaims();
    void setDelayMultiplierForSmallClaims(double value);

    double getDelayMultiplierForLargeClaims();
    void setDelayMultiplierForLargeClaims(double value);

    double getSmallClaimThreshold();
    void setSmallClaimThreshold(double value);

    double getLargeClaimThreshold();
    void setLargeClaimThreshold(double value);

    int resetTime();
    void getRequests();

    int evaluateModel();

public slots:


private:

    // Data members
    QVector<QVector<double>> theRequesterBuildingValue;

    QList<QPointer<RParameter> > theInputParameterList;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<double>> thePacketProcessingTimeList;
    QVector<QVector<double>> thePacketBuildingValue;

    QVector<QVector<double>> timeRequestsWereReceived;

    double theMedianDelayForPayments;
    double theDispersionOfDelayForPayments;
    double theDelayMultiplierSmallClaims;
    double theDelayMultiplierLargeClaims;
    double theSmallClaimThreshold;
    double theLargeClaimThreshold;

    RResponse *theResponse;

};

#endif
