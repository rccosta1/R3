#ifndef RResidenceFactoryModel_H
#define RResidenceFactoryModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"
#include "RNetworkModel.h"

class RParameter;
class RResponse;

class RResidenceFactoryModel : public RNetworkModel
{
    Q_PROPERTY(int BaselineRentalHomes READ getBaselineRentalHomes WRITE setBaselineRentalHomes)
    Q_PROPERTY(int InterventionID READ getInterventionID WRITE setInterventionID)
    Q_OBJECT


public:

    RResidenceFactoryModel(QObject *parent, QString name);
    ~RResidenceFactoryModel();

    int getBaselineRentalHomes();
    void setBaselineRentalHomes(int value);

    int getInterventionID();
    void setInterventionID(int value);

    int resetTime();

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

    int theBaselineRentalHomes;
    int theTotalDemandForResidences;
    int theInterventionID;
    int theNumberOfHomesCreatedInThisTimeStep;

    RResponse *theResponse;

};

#endif
