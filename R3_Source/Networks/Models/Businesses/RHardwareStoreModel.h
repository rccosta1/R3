#ifndef RHardwareStoreModel_H
#define RHardwareStoreModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RHardwareStoreModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QString MaterialsSupplierList READ getMaterialsSupplierList WRITE setMaterialsSupplierList)
    Q_PROPERTY(double StockOfWoodProducts READ getStockOfWoodProducts WRITE setStockOfWoodProducts)
    Q_PROPERTY(int NumberOfDailyDeliveries READ getNumberOfDailyDeliveries WRITE setNumberOfDailyDeliveries)


public:

    RHardwareStoreModel(QObject *parent, QString name);
     ~RHardwareStoreModel();

    int resetTime();


    QString getMaterialsSupplierList();
    void setMaterialsSupplierList(QString value);

    QString getTruckList();
    void setTruckList(QString value);

    double getStockOfWoodProducts() const;
    void setStockOfWoodProducts(double value);

    int getNumberOfDailyDeliveries() const;
    void setNumberOfDailyDeliveries(int value);

    void getRequests();

    int evaluateModel();

protected:

private:

    // Data members
    QList<QPointer<RParameter> > theMaterialsSupplierList;

    QVector<QPointer<RModel>> thePacketDestinationList; // this is a copy of the destination list
    QVector<QVector<double>> thePacketAmountList; // this is a copy of the list of requests
    QVector<QString> thePacketSupplyTypeList;
    QList<QVector<int>> thePacketIDList;


    int theNumberOfDailyDeliveries;
    int numberOfWoodProductsRequestsCreatedHere;
    double theNumberOfWoodRequesters;
    double initialStockOfWoodProducts;
    double currentStockOfWoodProducts;
    double amountOfWoodRequested;

    QList<QPointer<RParameter>> theTruckList;

    RResponse *theResponse;
};

#endif
