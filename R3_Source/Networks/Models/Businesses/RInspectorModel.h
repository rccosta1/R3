#ifndef RInspectorModel_H
#define RInspectorModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RInspectorModel : public RNetworkModel
{
    Q_PROPERTY(int BaselineNumberOfInspectors READ getBaselineNumberOfInspectors WRITE setBaselineNumberOfInspectors)
    Q_PROPERTY(double NumberOfDailyWorkHours READ getNumberOfDailyWorkHours WRITE setNumberOfDailyWorkHours)
    Q_PROPERTY(double MeanInspectionTime READ getMeanInspectionTime WRITE setMeanInspectionTime)
    Q_PROPERTY(QObject* EpicenterLocation READ getEpicenterLocation WRITE setEpicenterLocation)
    Q_OBJECT


public:

    RInspectorModel(QObject *parent, QString name);
    ~RInspectorModel();

    int getBaselineNumberOfInspectors();
    void setBaselineNumberOfInspectors(int value);

    double getNumberOfDailyWorkHours();
    void setNumberOfDailyWorkHours(double value);

    double getMeanInspectionTime();
    void setMeanInspectionTime(double value);

    QObject* getEpicenterLocation();
    void setEpicenterLocation(QObject* value);

    int resetTime();

    double computeSurfaceDistance(RLocation *requester, RLocation *epicentre);

    void getRequests();

    // Remove after paper
    int getNumberOfInspectors();

    int evaluateModel();

public slots:


private:

    // Data members
    QList<QPointer<RParameter> > theInputParameterList;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;
    QVector<double> thePacketTimeList;

    RLocation* theHypocentre;
    int theBaselineNumberOfInspectors;
    int theNumberOfInspectors;
    double theNumberOfDailyWorkHours;
    double theMeanInspectionTime;

    //Remove after paper
    double samplingMultiplier = 0.0;

    RResponse *theResponse;

};

#endif
