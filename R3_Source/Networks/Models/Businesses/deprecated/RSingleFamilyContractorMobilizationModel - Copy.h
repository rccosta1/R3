#ifndef RSingleFamilyContractorMobilizationModel_H
#define RSingleFamilyContractorMobilizationModel_H

#include "RNetworkModel.h"
#include "RMainWindow.h"
#include "RResponse.h"
#include "RModel.h"
#include "RPacket.h"
#include <QDateTime>
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RSingleFamilyContractorMobilizationModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(int BaselineNumberOfSingleFamilyCrews READ getBaselineNumberOfSingleFamilyCrews WRITE setBaselineNumberOfSingleFamilyCrews)
    Q_PROPERTY(double HourlyWages READ getBaseHourlyWages WRITE setBaseHourlyWages)


public:
    RSingleFamilyContractorMobilizationModel(QObject *parent, QString name);
    ~RSingleFamilyContractorMobilizationModel();

    int resetTime();

    int getBaselineNumberOfSingleFamilyCrews();
    void setBaselineNumberOfSingleFamilyCrews(int value);

    double getBaseHourlyWages();
    void setBaseHourlyWages(double value);

    void dismissTeamOfWorkers(QString theOccupancy, int value);

    void getRequests();

    int evaluateModel(RGradientType theGradientType);

    // Remove after paper
    int getNumberOfSingleFamilyCrews();

public slots:


private:   
    // Data members
    QVector<QVector<QString>> theBuildingOccupancy;
    QVector<double> theNeighborhoodIncome;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<bool>> shouldGetWorkers;
    QVector<RModel*> thePriorityList;


    int theBaselineNumberOfSingleFamilyCrews;
    int theNumberOfSingleFamilyCrews;
    int theNumberOfSingleFamilyCrewsAvailable;
    double theBaseHourlyWages;
    double theCurrentHourlyWages;


    //Remove after paper
    double samplingMultiplier = 0.0;


    QPointer<QCustomPlot> workforceHourlyCostCustomPlot;

    RResponse *theResponse;
};

#endif
