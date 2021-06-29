#ifndef RPermitAssessorModel_H
#define RPermitAssessorModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RPermitAssessorModel : public RNetworkModel
{
    Q_PROPERTY(int BaselineNumberOfEngineers READ getBaselineNumberOfEngineers WRITE setBaselineNumberOfEngineers)
    Q_OBJECT


public:

    RPermitAssessorModel(QObject *parent, QString name);
    ~RPermitAssessorModel();

    int getBaselineNumberOfEngineers();
    void setBaselineNumberOfEngineers(int value);

    int resetTime();

    // Remove after paper
    int getNumberOfEngineers();

    int evaluateModel();

public slots:


private:

    // Data members
    QVector<QVector<int>> theBuildingRepairClass;
    QList<QPointer<RParameter> > theInputParameterList;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;
    QVector<QVector<double>> thePacketProcessingTimeList;
    QVector<QVector<double>> theTimeUnderProcessing;

    int theBaselineNumberOfEngineers;
    int theNumberOfEngineers;

    //Remove after paper
    double samplingMultiplier = 0.0;

    RResponse *theResponse;

};

#endif
