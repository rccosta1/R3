#ifndef RMultiFamilyContractorFirmModel_H
#define RMultiFamilyContractorFirmModel_H

#include "RNetworkModel.h"
#include "RMainWindow.h"
#include "RResponse.h"
#include "RModel.h"
#include "RPacket.h"
#include <QDateTime>
#include "qcustomplot.h"


class RParameter;
class RResponse;

class RMultiFamilyContractorFirmModel : public RNetworkModel
{
    Q_OBJECT
    Q_PROPERTY(int BaselineNumberOfMultiFamilyCrews READ getBaselineNumberOfMultiFamilyCrews WRITE setBaselineNumberOfMultiFamilyCrews)
    Q_PROPERTY(double HourlyWages READ getBaseHourlyWages WRITE setBaseHourlyWages)
    Q_PROPERTY(QString Behavior READ getBehavior WRITE setBehavior)

public:
    RMultiFamilyContractorFirmModel(QObject *parent, QString name);
    ~RMultiFamilyContractorFirmModel();

    int resetTime();

    int getBaselineNumberOfMultiFamilyCrews();
    void setBaselineNumberOfMultiFamilyCrews(int value);

    double getBaseHourlyWages();
    void setBaseHourlyWages(double value);

    QString getBehavior();
    void setBehavior(QString value);

    void dismissTeamOfWorkers(QString theOccupancy, int value);

    void setNumberOfWorkersAvailable(int theNumberOfDwellingsNeededRepair,int theTotalNumberOfDwellings);

    void getRequests();

    int evaluateModel();

    // Remove after paper
    int getNumberOfMultiFamilyCrews();

public slots:


private:   
    // Data members
    QVector<QVector<QString>> theBuildingOccupancy;
    QVector<double> theNeighborhoodIncome;
    QVector<QVector<double>> theBuildingAvailabilityOfFunds;

    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QVector<QVector<double>> thePacketBid;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<bool>> shouldGetWorkers;

    int theBaselineNumberOfMultiFamilyCrews;
    int theNumberOfMultiFamilyCrews;
    int theNumberOfMultiFamilyCrewsAvailable;
    double theBaseHourlyWages;
    double theCurrentHourlyWages;
    QString theBehavior;

    //Remove after paper
    double samplingMultiplier = 0.0;

    double theTimeOfFirstRequest = 0.0;


    QPointer<QCustomPlot> workforceHourlyCostCustomPlot;

    RResponse *theResponse;
};

#endif
