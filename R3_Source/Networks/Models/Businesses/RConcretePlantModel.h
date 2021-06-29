#ifndef RConcretePlantModel_H
#define RConcretePlantModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RConcretePlantModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QString MaterialsSupplierList READ getMaterialsSupplierList WRITE setMaterialsSupplierList)
    Q_PROPERTY(double ConcreteStock READ getConcreteStock WRITE setConcreteStock)
    Q_PROPERTY(int NumberOfDailyDeliveries READ getNumberOfDailyDeliveries WRITE setNumberOfDailyDeliveries)


public:

    RConcretePlantModel(QObject *parent, QString name);
     ~RConcretePlantModel();

    int resetTime();

    QString getMaterialsSupplierList();
    void setMaterialsSupplierList(QString value);

    double getConcreteStock() const;
    void setConcreteStock(double value);

    int getNumberOfDailyDeliveries() const;
    void setNumberOfDailyDeliveries(int value);

    void getRequests();

    int evaluateModel();

protected:

private:
    // Data members
    QList<QPointer<RParameter> > theMaterialsMaterialsSupplierList;

    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;


    double initialConcreteStock;
    double currentConcreteStock;
    double currentCementStock;
    double currentGravelStock;
    double currentSandStock;

    int theNumberOfDailyDeliveries;
    int numberOfCementRequestsCreatedHere;
    int numberOfSandRequestsCreatedHere;
    int numberOfGravelRequestsCreatedHere;
    double amountOfConcreteRequested;


    RResponse *theResponse;
};

#endif
