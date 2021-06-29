#include "RScenarioModel.h"
#include "RParameter.h"
#include "RTime.h"
#include <QMap>
#include <QCoreApplication>
#include <QDateTime>
#include "RMainWindow.h"
#include "RDomain.h"
#include "RResponse.h"
#include "RModel.h"
#include "RLocation.h"
#include "math.h"
#include <QMessageBox>
#include <QEventLoop>
#include <math.h>
#include <QDateTime>
#include "RTime.h"

#ifdef NETWORKS
#include "RNeighborhoodModel.h"
#include "RSingleFamilyContractorFirmModel.h"
#include "RMultiFamilyContractorFirmModel.h"
#include "REngineeringFirmModel.h"
#include "RInspectorModel.h"
#include "RPermitAssessorModel.h"
#include "RBooreAtkinson2008IntensityModel.h"
#include "RBoundedExponentialMagnitudeModel.h"
//#include "RWaterDemandZoneModel.h"
#endif


RScenarioModel::RScenarioModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    isOrchestratingModel = true;
    theStartTime = 0;
    theEndTime = 0;
    theEventOccurrence = false;

    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
    theResponse->setCurrentValue(0.0);
}


RScenarioModel::~RScenarioModel()
{
}


QString RScenarioModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}

void RScenarioModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


QObject * RScenarioModel::getStartTime() const
{
    return theStartTime;
}

void RScenarioModel::setStartTime(QObject *value)
{
    theStartTime = qobject_cast<RParameter *>(value);
}


QObject * RScenarioModel::getEndTime() const
{
    return theEndTime;
}

void RScenarioModel::setEndTime(QObject *value)
{
    theEndTime = qobject_cast<RParameter *>(value);
}


QString RScenarioModel::getRealInterestRate()
{
    return getStringParameter(&theRealInterestRateDouble, theRealInterestRateParameter);
}

void RScenarioModel::setRealInterestRate(QString value)
{
    setStringParameter(value, &theRealInterestRateDouble, &theRealInterestRateParameter);
}

double RScenarioModel::getRealInterestRateValue()
{
    return getDoubleParameter(&theRealInterestRateDouble, theRealInterestRateParameter);
}


QString RScenarioModel::getMaxTimeGap()
{
    return getStringParameter(&theMaxTimeGapDouble, theMaxTimeGapParameter);
}

void RScenarioModel::setMaxTimeGap(QString value)
{
    setStringParameter(value, &theMaxTimeGapDouble, &theMaxTimeGapParameter);
}

double RScenarioModel::getMaxTimeGapValue()
{
    return getDoubleParameter(&theMaxTimeGapDouble, theMaxTimeGapParameter);
}


QStringList RScenarioModel::getActionList()
{
    // Remember that the "run" and "help" methods are implemented in base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Plot Model Flowchart"<< "Separator"<< "&Help";
    return actionList;
}


void RScenarioModel::plotModelFlowchart()
{
    if (theDomain->checkAllProperties() >= 0) {
        QList<RModel *> listOfModels = theDomain->findChildren<RModel *>();
        RModelManager theModelManager(theDomain);
        theModelManager.createModelMap(theInputParameterList, listOfModels);
        theModelManager.plotFlowchart();
    }
}


int RScenarioModel::evaluateModel()
{

    // Get start and end time
    double startTime = theStartTime->getCurrentValue();
    double endTime = theEndTime->getCurrentValue();


    // Create plot
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());
    QVector<double> tickVector;
    tickVector << 0;
    QVector<QString> labelVector;
    labelVector << QString("%1").arg(0);



    // Get the models that serve input to this model
    QList<RModel *> inputModelList;
    for (int i=0; i<theInputParameterList.size(); i++) {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameterList.at(i));
        inputModelList << qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    }

    // Re-generate occurrence times (we are called repeatedly by sampling, and we want a new scenario every time)
    QList<RModel *> theModels = theDomain->findChildren<RModel *>();
    for (int i=0; i<theModels.size(); i++) {
        theModels.at(i)->resetTime();
    }

    // Get all trigger times
    RModelManager theModelManager(theDomain);
    QList<QPair<double, QString> > *triggerTimes = theModelManager.getTriggerTimes(startTime, endTime, getMaxTimeGapValue());

    /*
    // Make sure there are no time-gaps bigger than "maximum time between evaluations"
    double timeGap, previousTriggerTime, newTriggerTime;
    bool newTimesWereInserted = false;

    for (int i=0; i<triggerTimes->count()-1; i++) {
        timeGap = triggerTimes->at(i+1).first - triggerTimes->at(i).first;
        previousTriggerTime = triggerTimes->at(i).first;

        while (timeGap > getMaxTimeGapValue()) {
            newTimesWereInserted = true;
            newTriggerTime = previousTriggerTime + getMaxTimeGapValue();
            triggerTimes->append(qMakePair(newTriggerTime, QString("Maximum Time Gap Evaluation")));
            timeGap = triggerTimes->at(i+1).first - newTriggerTime;
            previousTriggerTime = newTriggerTime;
        }
    }


    // Sort the trigger times
    if (newTimesWereInserted) {
        qSort(triggerTimes->begin(), triggerTimes->end());
    }
    */


    // Create a dummy time object to assist with output
    RTime dummyTime(theDomain, "dummyTime");

    // Give output if requested
    if (theOutputLevel == RObject::Maximum) {
        qDebug() << this->objectName() << "has these trigger times:";
        for (int i = 0; i < triggerTimes->size(); i++) {
            dummyTime.setCurrentValue(triggerTimes->at(i).first);
            qDebug() << dummyTime.getTime() << "prompted by" << triggerTimes->at(i).second;
        }
    }



    // *****************************************************************
    // Loop over trigger times, which are already sorted chronologically
    // *****************************************************************
    double totalCost = 0.0;
    double triggerTime;

    for (int i=0; i<triggerTimes->size(); i++) {

        // Keep the GUI responsive
        QCoreApplication::processEvents();


        // Get trigger time
        triggerTime = triggerTimes->at(i).first;


        // Give output if requested
        if (theOutputLevel == RObject::Maximum) {
            dummyTime.setCurrentValue(triggerTimes->at(i).first);
            qDebug() << this->objectName() << "starting analysis at" << triggerTimes->at(i).first << "due to" << triggerTimes->at(i).second;
        }


        // Set the current time in all model to the trigger time
        theCurrentTime->setCurrentValue(triggerTime);


        // Run analysis in all models
        for (int i=0; i<inputModelList.size(); i++) {
            inputModelList.at(i)->runAnalysis(RGradientType::NONE);
        }

        // Give output if requested
        if (theOutputLevel >= RObject::Medium) {
            dummyTime.setCurrentValue(triggerTimes->at(i).first);
            qDebug() << "Analysis at" << triggerTimes->at(i).second << dummyTime.getTime() << "Cumulative cost=" << totalCost << "Discount factor=" << exp(-getRealInterestRateValue()*(triggerTime-startTime)/365.0);
        }


        // Re-set the "is-analyzed" flags in the models, so they are re-analyzed next time
        theDomain->clearAnalyzedFlags();

    }


    for (int i=0; i<theInputParameterList.size(); i++) {
        RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameterList.at(i));
        // The response from this model
        theResponse->setCurrentValue(theResponse->getCurrentValue() + theInputCastAsResponse->getCurrentValue());
    }

    return 1;
}









