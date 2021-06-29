#ifndef RNetworkModel_H
#define RNetworkModel_H

#include "RModel.h"
#include "RPacket.h"
#include "RPacketResponse.h"
#include "RResponse.h"
#include "RRequest.h"
#include "qcustomplot.h"
#include "RContinuousRandomVariable.h"
#include "RFragilityFunctions.h"
#include "RGenericResponse.h"
#include "RLocation.h"


class RParameter;
class RResponse;

class RNetworkModel : public RModel
{
    Q_OBJECT
    Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)
    //Q_PROPERTY(QString IntensityList READ getIntensityList WRITE setIntensityList)


public:

    RNetworkModel(QObject *parent, QString name);
    ~RNetworkModel();

    QObject *getLocation() const;
    void setLocation(QObject *value);

    QString getIntensityList();
    void setIntensityList(QString value);

    // Supply functions
    virtual void getRequests();
    void setTravelPaths(QVector<QVector<RModel *> > pathMatrix);
    void handlePacketsThatReachedFinalDestination();
    void getPossiblePaths();
    void collectRequests(QString value);
    void removeNonNetworkModels();
    int getPacketsCurrentlyOnboard(); 
    int loadPackets(QList<QPointer<RParameter> > theInputParameterList);
    int unloadPackets(RPacketResponse *theResponse, QList<QPointer<RPacket> > packets);
    int getShortestPath(RModel *theSinkModel);
    bool getPathMatrixNeed();

    bool getFunctionality();
    double getExpectedRepairTime();


protected:

    // METHODS
    int isAKnowDestination(RModel* theModel);
    QVector<double> calculateIntensities();


    // Data members
    QList<QPointer<RPacket> > packetsCurrentlyOnboard;   
    QVector<QVector<RModel *> > theTravelPaths;
    QPointer<RParameter> theLocation;  
    QVector<int> myPossiblePaths;


    //INPUT TO THE FRAGILITY FUNCTIONS
    QVector<double> thePGADamageMedians;
    QVector<double> thePGADamageDispersions;
    QVector<double> thePGDDamageMedians;
    QVector<double> thePGDDamageDispersions;
    QVector<double> theRepairTimeMedians;
    QVector<double> theRepairTimeStandardDeviations;
    QVector<double> theDamageRatios;
    QVector<double> theDamageProbabilities;


    // Hazard
    double timeOfEvent;
    double theSaIntensity;
    double thePGAIntensity;
    double thePGVIntensity;
    double thePGDIntensity;
    double theFunctionality;
    double lastEvaluationTime;
    double lateralPGDIntensity;
    double theThresholdfunctionality;
    double  myProbabilityOfLiquefaction;
    QString theSoilSusceptibilityLevel;

    // Immediate impacts
    double replacementCost;
    double directLoss;
    double theExpectedRepairTime;


    // Requester variables
    QVector<QPointer<RModel>> theRequestDestinationList;
    QVector<QString> theRequestedSupplyTypeList;
    QVector<QVector<int>>  theRequesterIDList;
    QVector<QVector<double>> theRequestedAmountList;

    // Supplier variables
    int numberOfNewPacketsCreatedHere;
    int numberOfRequestsCreatedHere;

    bool thePathMatrixNeed;
    QVector<int>       theListOfKnownPaths;
    QVector<RModel *>  theShortestPath;
    QList<RModel *>    theListOfKnownSinks;

    QList<RParameter*> theListOfParameters;


    // Plots
    QPointer<QCustomPlot> customPlot;

    // PGA counter
    int thePGACounter;

private:

    // METHODS
    int plotFunctionality(double x, double y);
    QPointer<QCustomPlot> functionalityPlot;


    // PROPERTIES
    QList<QPointer<RParameter> > theIntensityList;

    double intensityRNG;

};

#endif
