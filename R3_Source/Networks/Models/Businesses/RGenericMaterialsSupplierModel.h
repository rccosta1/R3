#ifndef RGenericMaterialsSupplierModel_H
#define RGenericMaterialsSupplierModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RGenericMaterialsSupplierModel : public RNetworkModel
{
    Q_OBJECT


    Q_PROPERTY(QString TruckList READ getTruckList WRITE setTruckList)


public:

    RGenericMaterialsSupplierModel(QObject *parent, QString name);
     ~RGenericMaterialsSupplierModel();

    int resetTime();

    QString getTruckList();
    void setTruckList(QString value);

    void getRequests();

    int evaluateModel();

protected:

private:

    // Data members
    QList<QPointer<RParameter> > theMaterialsSupplierList;


    QVector<QPair<int,int>> theMatrixOfIndexes;
    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<double> thePacketCriterionList; // this is a copy of the list of bids
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;


    //RTruck *theTruck;
    QList<QPointer<RParameter>> theTruckList;

    RResponse *theResponse;
};

#endif
