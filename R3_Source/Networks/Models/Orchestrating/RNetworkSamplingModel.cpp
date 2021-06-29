#include "RNetworkSamplingModel.h"
#include "RModel.h"
#include "RRandomNumberGenerator.h"
#include "RResponse.h"
#include "RDomain.h"
#include "RMainWindow.h"
#include "RRandomVariable.h"
//#include "RMatrixOperations.h"
#include "RNormalDistribution.h"
#include "RObject.h"
#include <QCoreApplication>
#include <QPointer>
#include <QDateTime>
#include <math.h>
#include  "QElapsedTimer"
#include "RGivenIntensityModel.h"
#include "RNeighborhoodModel.h"
#include "RResidentialMobilityNeighborhoodModel.h"
#include "RWorkforceStudyNeighborhoodModel.h"
#include "RBoundedExponentialMagnitudeModel.h"
#include "RBooreAtkinson2008IntensityModel.h"
#include "RSingleFamilyContractorFirmModel.h"
#include "RMultiFamilyContractorFirmModel.h"
#include "REngineeringFirmModel.h"
#include "RInspectorModel.h"
#include "RPermitAssessorModel.h"
#include "RGivenIntensityModel.h"
#include "RBoundedExponentialMagnitudeModel.h"
#include "RGivenMagnitudeModel.h"
#include <sstream>


RNetworkSamplingModel::RNetworkSamplingModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    // Initializing class variables
    isOrchestratingModel = true;
    theCustomPlot = nullptr;
    maxSamples = 10000;
    thePlotInterval = 100;
    theSamplingMultiplier = 0;
    theRandomNumberGenerator = nullptr;
    theProbabilityTransformation = nullptr;
    theSamplingCentre = "Origin";
    theVariableOfInterest = QString("Displaced");
}

RNetworkSamplingModel::~RNetworkSamplingModel()
{

}


QString RNetworkSamplingModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}

void RNetworkSamplingModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


QString RNetworkSamplingModel::getVariableOfInterest()
{
    return theVariableOfInterest;
}

void RNetworkSamplingModel::setVariableOfInterest(QString value)
{
    theVariableOfInterest = value;
}



int RNetworkSamplingModel::getMaxSamples() const
{
    return maxSamples;
}

void RNetworkSamplingModel::setMaxSamples(int value)
{
    if (maxSamples < 1) {
        qDebug() << "The sampling model requires the maximum number of samples to be greater than zero.";
        maxSamples = 1;
        return;
    }

    maxSamples = value;
}




int RNetworkSamplingModel::getPlotInterval() const
{
    return thePlotInterval;
}

void RNetworkSamplingModel::setPlotInterval(int value)
{
    if (thePlotInterval < 1) {
        qDebug() << "The sampling model requires the plot frequency to be greater than zero.";
        thePlotInterval = 1;
        return;
    }

    thePlotInterval = value;
}


QObject * RNetworkSamplingModel::getRandomNumberGenerator() const
{
    return theRandomNumberGenerator;
}

void RNetworkSamplingModel::setRandomNumberGenerator(QObject *value)
{
    theRandomNumberGenerator = qobject_cast<RRandomNumberGenerator *>(value);
}



QObject * RNetworkSamplingModel::getProbabilityTransformation() const
{
    return theProbabilityTransformation;
}

void RNetworkSamplingModel::setProbabilityTransformation(QObject *value)
{
    theProbabilityTransformation = qobject_cast<RProbabilityTransformation *>(value);
}


RNetworkSamplingModel::RSamplingMultiplierFunction RNetworkSamplingModel::getSamplingMultiplierFunction()
{
    return theSamplingMultiplierFunction;
}

void RNetworkSamplingModel::setSamplingMultiplierFunction(RNetworkSamplingModel::RSamplingMultiplierFunction value)
{
    theSamplingMultiplierFunction = value;
}



QString RNetworkSamplingModel::getSamplingCentre()
{
    return theSamplingCentre;
}

void RNetworkSamplingModel::setSamplingCentre(QString value)
{
    if (value.toLower().startsWith("origin")) {
        theSamplingCentre = "Origin";
    }
    else if (value.toLower().startsWith("current")) {
        theSamplingCentre = "Current Values";
    }
    else {
        qCritical() << "The sampling centre must be either Origin or Current Values.";
    }
}


QStringList RNetworkSamplingModel::getActionList()
{
    // Notice that the "run" and "help" methods are implemented in the base classes!
    QStringList actionList;
    actionList << "&Run" << "Separator" << "&Help";
    return actionList;
}


double RNetworkSamplingModel::getSamplingMultiplier()
{
    return theSamplingMultiplier;
}



int RNetworkSamplingModel::evaluateModel()
{
    // ***************************************************
    //  START TIMER
    // ***************************************************
    if (theOutputLevel >= RObject::Medium) {
        qDebug() << "The analysis in" << this->objectName() << "started...";
        qDebug() << "";
    }
    QDateTime startTime = QDateTime::currentDateTime();


    // ***************************************************
    //  GET A LIST OF THE RANDOM VARIABLES
    // ***************************************************
    QList<RRandomVariable *> theRVs = theDomain->findChildren<RRandomVariable *>();
    int numRVs = theRVs.size();
    QList<RParameter *> theRVsCastAsParameters;
    for (int i=0; i<numRVs; i++) {
        theRVsCastAsParameters << qobject_cast<RParameter *>(theRVs.at(i));

        // This is necessary in analyses with mean of random variables being decision variables:
        theRVs.at(i)->updateParameters();
    }
    theProbabilityTransformation->setRandomVariableList(theRVs);


    // ***************************************************
    //       RESET THE RANDOM NUMBER GENERATOR
    // ***************************************************
    theRandomNumberGenerator->reset();


    // ***************************************************
    //         INITIAL DECLARATIONS
    // ***************************************************
    QVector<double> uCentre(numRVs);
    QVector<double> u(numRVs);
    QVector<double> x(numRVs);
    double limitStateFunctionValue;



    // ***************************************************
    //     DECLARE A COUPLE OF TOOLS INTERNALLY
    // ***************************************************
    //RMatrixOperations theMatrixOperations(theDomain);
    RNormalDistribution theNormalDistribution(theDomain);


    // ***************************************************
    //       KEEP A COPY OF THE RANDOM VARIABLES
    // ***************************************************
    QVector<double> randomVariablesBackup;
    for (int i = 0; i < numRVs; i++) {
        randomVariablesBackup.append(theRVs[i]->getCurrentValue());
    }


    // ***************************************************
    //   SET THE SAMPLING CENTRE
    // ***************************************************
    if (theSamplingCentre == QString("Origin")) {
        // Leave the uCentre vector as is, at zero
    }
    else {

        for (int i = 0; i < numRVs; i++) {
            x[i] = theRVs[i]->getCurrentValue();
        }
        theProbabilityTransformation->transformPointToStandardNormalSpace(&x, &uCentre);
    }


    // ***************************************************
    //           START SAMPLING LOOP
    // ***************************************************
    int i=0;
    while (i < maxSamples) {
        i++;

        // Sampling multiplier
        if (theSamplingMultiplierFunction == RNetworkSamplingModel::Constant){
            theSamplingMultiplier = 0.1;

        } else if (theSamplingMultiplierFunction == RNetworkSamplingModel::Linear){
            theSamplingMultiplier = (i) * 0.1;

        } else if (theSamplingMultiplierFunction == RNetworkSamplingModel::None){
            theSamplingMultiplier = 1;
        }

        QElapsedTimer myTimer;
        myTimer.start();

        // ***************************************************
        // KEEP GUI RESPONSIVE
        // ***************************************************
        QCoreApplication::processEvents();

        // ***************************************************
        //     GENERATE STANDARD NORMAL NUMBERS
        // ***************************************************
        theRandomNumberGenerator->generateStandardNormalNumbers(&u);

        // ***************************************************
        //     GIVE NUMBERS TO RANDOM VARIABLES
        // ***************************************************
        for (int j=0; j<numRVs; j++) {
            theRVs[j]->setCurrentValue(x.at(j));
            //qDebug()<<"For the RV "<<theRVs[j]->objectName()<<" the value is "<<x.at(j);
        }

        // ***************************************************
        //     EVALUATE BOTH FUNCTIONS
        // ***************************************************
        // ***************************************************
        limitStateFunctionValue = evaluateLimitStateFunction();
        // ***************************************************
        // ***************************************************

    } // End sampling loop

    // ***************************************************
    //  STOP TIMER
    // ***************************************************
    QDateTime finishTime = QDateTime::currentDateTime();
    double numDays = startTime.daysTo(finishTime);
    double numMilliSecs = startTime.time().msecsTo(finishTime.time());
    double numSecs = numDays * 86400.0 + numMilliSecs / 1000.0;
    if (theOutputLevel >= RDomain::Medium) {
        qDebug() << "Sampling in" << this->objectName() << "ended after" << i << "iterations";
        qDebug() << "";
        //qDebug() << "The analysis in" << this->objectName() << "completed in" << numSecs << "seconds.";
        qDebug() << "";
    }


    return 0;
}




double RNetworkSamplingModel::evaluateLimitStateFunction()
{
    // Re-set the "is-analyzed" flags in the models, to make sure we start afresh
    theDomain->clearAnalyzedFlags();

    double functionValue = 0.0;

    RResponse *theInputCastAsResponse = qobject_cast<RResponse *>(theInputParameterList.at(0));
    if (!theInputCastAsResponse) {
        qCritical() << this->objectName() << "needs the input parameter to be a generic response";
        return -1;
    }
    RModel *theModel = qobject_cast<RModel *>(theInputCastAsResponse->getModel());
    theModel->runAnalysis(RGradientType::NONE);
    functionValue = 0.0;

    return functionValue;
}


void RNetworkSamplingModel::PlotSamplingResults(QVector<double> timeVector, QVector<QVector<double>> valueVector)
{
    if (theCustomPlot == nullptr){
        RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

        theCustomPlot = new QCustomPlot();
        QString theString = QString("Total habitable dwellings %1").arg(this->objectName());
        mainWindow->addSubWindow(theCustomPlot, theString, true);

        // configure right and top axis to show ticks but no labels:
        // (see QCPAxisRect::setupFullAxesBox for a quicker method to do this)
        theCustomPlot->xAxis2->setVisible(true);
        theCustomPlot->xAxis2->setTickLabels(false);
        theCustomPlot->yAxis2->setVisible(true);
        theCustomPlot->yAxis2->setTickLabels(false);
        // make left and bottom axes always transfer their ranges to right and top axes:
        connect(theCustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),theCustomPlot->xAxis2, SLOT(setRange(QCPRange)));
        connect(theCustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),theCustomPlot->yAxis2, SLOT(setRange(QCPRange)));

        // Note: we could have also just calledtheCustomPlot->rescaleAxes(); instead
        // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
        theCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }

    for (int i = 0; i < valueVector.size(); i++){
        // add two new graphs and set their look:
        theCustomPlot->addGraph();
        theCustomPlot->graph(i)->setPen(QPen(Qt::blue)); // line color blue for first graph
        //theCustomPlot->graph(i)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
        // pass data points to graphs:
        QVector<double> x, y;
        x = timeVector;
        y = valueVector.at(i);
        theCustomPlot->graph(i)->setData(x, y);

        // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
        theCustomPlot->graph(i)->rescaleAxes();
    }

    theCustomPlot->rescaleAxes();
}


