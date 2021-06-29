#include "RIntensitySpectrumPlotterModel.h"

#include "RParameter.h"
#include "RResponse.h"
#include "RIntensityModel.h"
#include "RMainWindow.h"

#include <math.h>

RIntensitySpectrumPlotterModel::RIntensitySpectrumPlotterModel(QObject *parent, QString name)
    : RModel(parent, name)
{
    IntensityCustomPlot = 0;
    plotCounter = 0;
    theDomain->createObject(objectName() + "Response", "RGenericResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);
}

RIntensitySpectrumPlotterModel::~RIntensitySpectrumPlotterModel()
{

}


QString RIntensitySpectrumPlotterModel::getIntensityList()
{
    return parameterListToString(theIntensityList);
}

void RIntensitySpectrumPlotterModel::setIntensityList(QString value)
{
    theIntensityList = stringToParameterList(value);
}


QObject * RIntensitySpectrumPlotterModel::getBuildingLocation() const
{
    return theBuildingLocationParameter;
}

void RIntensitySpectrumPlotterModel::setBuildingLocation(QObject *value)
{
    theBuildingLocationParameter = qobject_cast<RParameter *>(value);

    // Verification that the user has given an RLocationResponse or an RLocation
    parameterToLocation(theBuildingLocationParameter);
}



int RIntensitySpectrumPlotterModel::evaluateModel(RGradientType theGradientType)
{	
    QVector<double> thePeriods;

    RLocation *theBuildingLocation = qobject_cast<RLocation *>(theBuildingLocationParameter);

    // For all intensity inputs
    for (int i = 0; i < theIntensityList.size(); i++){

        RResponse *theResponseComingToThisModel = qobject_cast<RResponse *>(theIntensityList.at(i));
        RIntensityModel *theIntensityModel = qobject_cast<RIntensityModel *>(theResponseComingToThisModel->getModel());
        theIntensityModel->setLocation(theBuildingLocation);
        theIntensityModel->setIntensityMeasure(QString("Sa"));

        if (theIntensityModel->isOn() == true){

            // For all periods
            for (int k = 0; k < 100; k++){
                thePeriods << (double) 2.0 * k / 100.0;
                theIntensityModel->setPeriod(thePeriods.at(k));
                // Run the intensity model and get the response
                theIntensityModel->evaluateModel(RGradientType::NONE);
                theVariablesToPlot << theResponseComingToThisModel->getCurrentValue();
            }
        }
        if (plotCounter == 0){
            plotIntensities(i, thePeriods, theVariablesToPlot);
        }
    }

    qCritical() << thePeriods;
    qCritical() << theVariablesToPlot;

    return 1;
}	


void RIntensitySpectrumPlotterModel::plotIntensities(int graphNumber, QVector<double> xValues, QVector<double> yValues)
{
    // set scatter style:
    QVector<QCPScatterStyle::ScatterShape> shapes;
    shapes << QCPScatterStyle::ssCross;
    shapes << QCPScatterStyle::ssPlus;
    shapes << QCPScatterStyle::ssCircle;
    shapes << QCPScatterStyle::ssDisc;
    shapes << QCPScatterStyle::ssSquare;
    shapes << QCPScatterStyle::ssDiamond;
    shapes << QCPScatterStyle::ssStar;
    shapes << QCPScatterStyle::ssTriangle;
    shapes << QCPScatterStyle::ssTriangleInverted;
    shapes << QCPScatterStyle::ssCrossSquare;
    shapes << QCPScatterStyle::ssPlusSquare;
    shapes << QCPScatterStyle::ssCrossCircle;
    shapes << QCPScatterStyle::ssPlusCircle;
    shapes << QCPScatterStyle::ssPeace;

    int shapeNumber = graphNumber;
    if (graphNumber > shapes.size()){
        shapeNumber = graphNumber - (qFloor((double) graphNumber / shapes.size()) * shapes.size() + 1);
    }

    double xAxisMin = qMin(*std::min_element(xValues.constBegin(),xValues.constEnd()),0.0);
    double xAxisMax = *std::max_element(xValues.constBegin(),xValues.constEnd());
    double yAxisMin = qMin(*std::min_element(yValues.constBegin(),yValues.constEnd()),0.0);
    double yAxisMax = *std::max_element(yValues.constBegin(),yValues.constEnd());

    qCritical() << "The maximum on-site acceleration is" << yAxisMax << "g at the period of" << xValues.at(yValues.indexOf(yAxisMax)) << "seconds.";

    if (theOutputLevel >= RObject::Medium) {

        if (IntensityCustomPlot == 0) {

            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());

            IntensityCustomPlot = new QCustomPlot();
            QString theString = QString("Spectral Acceleration (g) at %1").arg(theBuildingLocationParameter->objectName());
            mainWindow->addSubWindow(IntensityCustomPlot, theString, true);
            IntensityCustomPlot->xAxis->setLabel("Period (s)");
            IntensityCustomPlot->yAxis->setLabel(theString);
            IntensityCustomPlot->yAxis->setRange(yAxisMin, 1.2 * yAxisMax);
            IntensityCustomPlot->xAxis->setRange(xAxisMin, xAxisMax);
            IntensityCustomPlot->legend->setVisible(true);
            IntensityCustomPlot->legend->setFont(QFont("Helvetica",9));

            // make left and bottom axes always transfer their ranges to right and top axes:
            connect(IntensityCustomPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),  IntensityCustomPlot->xAxis2, SLOT(setRange(QCPRange)));
            connect(IntensityCustomPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),  IntensityCustomPlot->yAxis2, SLOT(setRange(QCPRange)));
            IntensityCustomPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

            // add the first graph
            IntensityCustomPlot->addGraph();
            IntensityCustomPlot->graph(graphNumber)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            IntensityCustomPlot->graph(graphNumber)->setPen(QColor(0, 0, 255, 255));
            IntensityCustomPlot->graph(graphNumber)->setData(xValues, yValues);
            IntensityCustomPlot->graph(graphNumber)->setName(QString("Spectral Acceleration at %1").arg(theBuildingLocationParameter->objectName()));
            IntensityCustomPlot->graph(graphNumber)->setScatterStyle(QCPScatterStyle(shapes.at(shapeNumber), 10));
            IntensityCustomPlot->replot();
        }

        else {

            // add extra graphs
            IntensityCustomPlot->addGraph();
            IntensityCustomPlot->graph(graphNumber)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 7));
            IntensityCustomPlot->graph(graphNumber)->setPen(QColor(0, 0, 255, 255));
            IntensityCustomPlot->graph(graphNumber)->setData(xValues, yValues);
            IntensityCustomPlot->graph(graphNumber)->setName(QString("Spectral Acceleration at %1").arg(theBuildingLocationParameter->objectName()));
            IntensityCustomPlot->graph(graphNumber)->setScatterStyle(QCPScatterStyle(shapes.at(shapeNumber), 10));
            IntensityCustomPlot->replot();
        }
    }
}

