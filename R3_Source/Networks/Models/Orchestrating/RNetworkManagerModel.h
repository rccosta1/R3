#ifndef RNetworkManagerModel_H
#define RNetworkManagerModel_H

#include "RResponse.h"
#include "RModelManager.h"
#include "RModel.h"
#include <QDateTime>
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RNetworkManagerModel : public RModel
{
    Q_OBJECT

    Q_PROPERTY(QString InputParameterList READ getInputParameterList WRITE setInputParameterList)
    Q_PROPERTY(double MaxTimeGap READ getMaxTimeGap WRITE setMaxTimeGap)

public:

    RNetworkManagerModel(QObject *parent, QString name);
    ~RNetworkManagerModel();\

    int resetTime();


    QStringList getActionList();

    QString getInputParameterList();
    void setInputParameterList(QString value);

    double getMaxTimeGap();
    void setMaxTimeGap(double value);

    int evaluateModel();

public slots:

    void displayGISMap();

#ifdef GIS
    void displayLocationsOnMap();
    void displayBuildingsOnMap();
    void displayNetworkOnMap();
    void displayNetworkLinksOnMap();
    void displayHazardAreaSources();
    void displayCensusTractsOnMap();
    void displayRentersOnMap();
    void displayRecentImmigrantsOnMap();
    void displayOldBuildingsOnMap();
    void displaySingleFamilyDwellingsOnMap();
    void displayMultiFamilyDwellingsOnMap();
    void displayDirectEconomicLossOnMap();
    void displayDisplacedPopulationOnMap();
    void displayInjuredPopulationOnMap();
    void displayRecoveryOnMap();
    void displayInfrastructureRecoveryOnMap();
    void displayEpicenterOnMap();
#endif

private:
    void plotResults(double habitableDwellings, double totalDwellings);
    void plotResults(double var1, double var2, double var3, double var4, QString var1Name, QString var2Name, QString var3Name, QString var4Name, double maxValue);
    void plotResults(double var1, double var2, double var3, double var4, double var5, QString var1Name, QString var2Name, QString var3Name, QString var4Name, QString var5Name, double maxValue);

    QList<QPointer<RParameter> > theInputParameterList;
    QVector<QVector<RModel *> > originalPathMatrix;


    bool theEventOccurrence;
    bool aModelfinishedRepairs;

    int stepCounter;
    int totalNumberOfPersonsSlightlyInjured;
    int totalNumberOfPersonsModeratelyInjured;
    int totalNumberOfPersonsSeverelyInjured;

    double myCounter;
    double pathMatrixSize;
    double timeOfLastPlot;


    double lastEvaluationTime;
    double timeAnalysisBegan;
    double timeSinceLastPlot;
    double theMinimumRecoveryValue;
    double theMaximumRecoveryValue;
    QVector<double> tickVector;
    QVector<QString> labelVector;
    QPointer<QCustomPlot> customPlot;
    QPointer<QCustomPlot> customPlotDisplaced;
    QPointer<QCustomPlot> customPlotResidentialMobility;
    double theMaxTimeGap;


    RResponse *theResponse;
};

#endif
