#include "RCommunityModel.h"
#include "RParameter.h"
#include "RFuelTankFarmModel.h"
#include <math.h>

RCommunityModel::RCommunityModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theDailyDemand = 0.0;
    
    lastEvaluationTime = theCurrentTime->getCurrentValue();

    // Instantiating a continuous response
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}

RCommunityModel::~RCommunityModel()
{

}


int RCommunityModel::resetTime(){

    return 0;
}


double RCommunityModel::getDailyDemand() const
{
    return theDailyDemand;
}

void RCommunityModel::setDailyDemand(double value)
{
    theDailyDemand = value;
}


QString RCommunityModel::getContracts()
{
    return parameterListToString(theContracts);
}

void RCommunityModel::setContracts(QString value)
{
    theContracts = stringToParameterList(value);
}


QString RCommunityModel::getFuelSupplierList()
{
    return parameterListToString(theFuelSupplierList);
}

void RCommunityModel::setFuelSupplierList(QString value)
{
    theFuelSupplierList = stringToParameterList(value);
}



int RCommunityModel::evaluateModel()
{

    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName() << "at time" << theCurrentTime->getTime();
    }




    // Get hold of the input model, i.e., a tank farm
    for (int i = 0; i < theFuelSupplierList.size(); i++){

        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theFuelSupplierList.at(i));
        RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());

        RFuelTankFarmModel *theTankFarm;
        theTankFarm = qobject_cast<RFuelTankFarmModel *>(theModel);


        if (theTankFarm && theCurrentTime->getCurrentValue()-lastEvaluationTime >= 0.0) {
            int gotFuel = theTankFarm->extractSupply((theCurrentTime->getCurrentValue()-lastEvaluationTime) * theDailyDemand / theFuelSupplierList.size() );
            //qCritical() << this->objectName() << "is extracting" << (theCurrentTime->getCurrentValue()-lastEvaluationTime) * theDailyDemand / theFuelSupplierList.size() << "from" << theTankFarm->objectName();
        }
    }


    //timeUnderRepairs = 0;
    lastEvaluationTime = theCurrentTime->getCurrentValue();


    return 1;
}
