#ifndef RWarehouseModel_H
#define RWarehouseModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RNetworkModel.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RWarehouseModel : public RNetworkModel
{
    Q_OBJECT

    Q_PROPERTY(QString ConstructionMaterialsSupplierList READ getConstructionMaterialsSupplierList WRITE setConstructionMaterialsSupplierList)
    Q_PROPERTY(double Stock READ getStock WRITE setStock)
    Q_PROPERTY(int NumberOfTrucks READ getNumberOfTrucks WRITE setNumberOfTrucks)
    Q_PROPERTY(double TruckCapacity READ getTruckCapacity WRITE setTruckCapacity)
    Q_PROPERTY(double TruckVelocity READ getTruckVelocity WRITE setTruckVelocity)


public:

    RWarehouseModel(QObject *parent, QString name);
     ~RWarehouseModel();

    int resetTime();


    QString getConstructionMaterialsSupplierList();
    void setConstructionMaterialsSupplierList(QString value);

    double getStock();
    void setStock(double value);

    int getNumberOfTrucks();
    void setNumberOfTrucks(int value);

    double getTruckCapacity();
    void setTruckCapacity(double value);

    double getTruckVelocity();
    void setTruckVelocity(double value);

    int evaluateModel(RGradientType theGradientType);

protected:

private:
    QList<QPointer<RParameter> > theConstructionMaterialsSupplierList;

    double initialStock;
    double currentStock;
    int theNumberOfTrucks;
    double theTruckCapacity;
    double theTruckVelocity;

    RResponse *theResponse;
};

#endif
