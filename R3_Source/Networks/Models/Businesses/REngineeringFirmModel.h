#ifndef REngineeringFirmModel_H
#define REngineeringFirmModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class REngineeringFirmModel : public RNetworkModel
{
    Q_PROPERTY(int BaselineNumberOfSingleFamilyCrews READ getBaselineNumberOfSingleFamilyCrews WRITE setBaselineNumberOfSingleFamilyCrews)
    Q_PROPERTY(int BaselineNumberOfMultiFamilyCrews READ getBaselineNumberOfMultiFamilyCrews WRITE setBaselineNumberOfMultiFamilyCrews)
    Q_OBJECT


public:

    REngineeringFirmModel(QObject *parent, QString name);
    ~REngineeringFirmModel();

    int getBaselineNumberOfSingleFamilyCrews();
    void setBaselineNumberOfSingleFamilyCrews(int value);

    int getBaselineNumberOfMultiFamilyCrews();
    void setBaselineNumberOfMultiFamilyCrews(int value);

    int resetTime();

    void getRequests();

    // Remove after paper
    int getNumberOfSingleFamilyCrews();
    int getNumberOfMultiFamilyCrews();

    int evaluateModel();

public slots:


private:

    // Data members
    QVector<QVector<int>> theBuildingRepairClass;
    QVector<QVector<QString>> theBuildingOccupancyClass;
    QVector<QVector<double>> theBuildingAvailabilityOfFunds;
    QList<QPointer<RParameter> > theInputParameterList;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<double>> thePacketProcessingTimeList;
    QVector<QVector<double>> theTimeUnderProcessing;
    QVector<QPair<RModel*,int>> thePriorityList;


    int theBaselineNumberOfSingleFamilyCrews;
    int theBaselineNumberOfMultiFamilyCrews;
    int theNumberOfSingleFamilyCrews;
    int theNumberOfMultiFamilyCrews;


    //Remove after paper
    double samplingMultiplier = 0.0;

    RResponse *theResponse;

};

#endif
