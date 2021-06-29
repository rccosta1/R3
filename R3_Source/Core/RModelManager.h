#ifndef RModelManager_H
#define RModelManager_H
#include "RMethod.h"
#include "RDomain.h"
#include "RMainWindow.h"
#include "RFlowchartWidget.h"
#include <QProcess>


#ifdef NETWORKS
#include "RNetworkModel.h"
#include "RPacket.h"
#include "RPacketResponse.h"
#endif

class RModelManager : public RMethod
{


public:
    RModelManager(RDomain *theDomain);
    ~RModelManager();

    int createModelMap(QList<QPointer<RParameter> > parameterList, QList<RModel *> functionModelList);
    int plotFlowchart();
    QList<QPair<double, QString> > *getTriggerTimes(double startTime, double endTime, double maxTimeGap);


#ifdef NETWORKS
    int createLocationDiagram();
    //int assignRoutes();
    //int assignMaritimeRoutes();
    int establishPaths();
    int removeClosedPaths();
    int printPaths();
    int setPaths();
    void setPathMatrix(QVector<QVector<RModel *> > originalPathMatrix);
    QVector<QVector<RModel *> > retrievePathMatrix();
#endif

    // Can we remove these? (And the iteration map in the cpp file?)
    int setModelIterationNumber(RModel*, int iterationNumber);
    int getModelIterationNumber(RModel* model);
    static QMap<RModel*,int> modelIterationMap;

protected:

    QVector<QVector<RModel *> > pathMatrix;

private:

     QMap<QPointer<QObject>, QObjectList> objectMap;
     QList<QObjectList> objectListList;
     QList<QPair<double, QString> > triggerTimes;
     bool theEventOccurrence;

};



#endif



