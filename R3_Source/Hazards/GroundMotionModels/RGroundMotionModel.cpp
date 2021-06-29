#include "RGroundMotionModel.h"

#include "RParameter.h"
#include "RResponse.h"
#include "RLocation.h"
#include "RLocationResponse.h"
#include "RMainWindow.h"
//#include "RGLWidget.h"
//#include "RGLWidgetEngine.h"
#include <math.h>


RGroundMotionModel::RGroundMotionModel(QObject *parent, QString name)
    : RModel(parent, name)
{

}

RGroundMotionModel::~RGroundMotionModel()
{

}




QStringList RGroundMotionModel::getActionList()
{
    QStringList actionList;
    actionList << "Plot &Acceleration" << "Plot &Velocity" << "Plot &Displacement";
    return actionList;
}



void RGroundMotionModel::plotAcceleration()
{
/*
    // Get a pointer to the main window
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(parent()->parent());


    // Create the OpenGL "widget"
    RGLWidget *theGLWidget = new RGLWidget(mainWindow);


    // Add the widget to the visualization pane of the main window
    mainWindow->addSubWindow(theGLWidget, QString("Ground Motion Acceleration").arg(objectName()), false);


    // Create the OpenGL plotting engine
    RGLWidgetEngine *theGLWidgetEngine = 0;
    theGLWidgetEngine = theGLWidget->getEngine();
    theGLWidgetEngine->clearAll();


    // Create matrix that has t, x, y, and z as columns
    int numberOfDataPoints = 27;
    QVector< QVector<double> > *theData = new QVector< QVector<double> >(numberOfDataPoints, QVector<double>(4));


    // Set the data
    (*theData)[	0	][0] =	0.01	;		(*theData)[	0	][1] =	0.374670547	;		(*theData)[	0	][2] =	0.15742868	;		(*theData)[	0	][3] =	0.087037782	;
    (*theData)[	1	][0] =	0.02	;		(*theData)[	1	][1] =	0.30858857	;		(*theData)[	1	][2] =	0.161075175	;		(*theData)[	1	][3] =	0.910593352	;
    (*theData)[	2	][0] =	0.03	;		(*theData)[	2	][1] =	0.295148887	;		(*theData)[	2	][2] =	0.510844441	;		(*theData)[	2	][3] =	0.850526725	;
    (*theData)[	3	][0] =	0.04	;		(*theData)[	3	][1] =	0.050063066	;		(*theData)[	3	][2] =	0.827234311	;		(*theData)[	3	][3] =	0.635589307	;
    (*theData)[	4	][0] =	0.05	;		(*theData)[	4	][1] =	0.199928554	;		(*theData)[	4	][2] =	0.084807299	;		(*theData)[	4	][3] =	0.614660188	;
    (*theData)[	5	][0] =	0.06	;		(*theData)[	5	][1] =	0.317566161	;		(*theData)[	5	][2] =	0.094159827	;		(*theData)[	5	][3] =	0.811503396	;
    (*theData)[	6	][0] =	0.07	;		(*theData)[	6	][1] =	0.095432193	;		(*theData)[	6	][2] =	0.452649627	;		(*theData)[	6	][3] =	0.283249036	;
    (*theData)[	7	][0] =	0.08	;		(*theData)[	7	][1] =	0.263647875	;		(*theData)[	7	][2] =	0.985881488	;		(*theData)[	7	][3] =	0.869587736	;
    (*theData)[	8	][0] =	0.09	;		(*theData)[	8	][1] =	0.085770329	;		(*theData)[	8	][2] =	0.803290809	;		(*theData)[	8	][3] =	0.627587071	;
    (*theData)[	9	][0] =	0.1	;		(*theData)[	9	][1] =	0.902929991	;		(*theData)[	9	][2] =	0.457246099	;		(*theData)[	9	][3] =	0.46731165	;
    (*theData)[	10	][0] =	0.11	;		(*theData)[	10	][1] =	0.656183094	;		(*theData)[	10	][2] =	0.65848012	;		(*theData)[	10	][3] =	0.747816661	;
    (*theData)[	11	][0] =	0.12	;		(*theData)[	11	][1] =	0.223937027	;		(*theData)[	11	][2] =	0.380481971	;		(*theData)[	11	][3] =	0.191346077	;
    (*theData)[	12	][0] =	0.13	;		(*theData)[	12	][1] =	0.492546675	;		(*theData)[	12	][2] =	0.814537232	;		(*theData)[	12	][3] =	0.318379226	;
    (*theData)[	13	][0] =	0.14	;		(*theData)[	13	][1] =	0.379288827	;		(*theData)[	13	][2] =	0.660598547	;		(*theData)[	13	][3] =	0.338871428	;
    (*theData)[	14	][0] =	0.15	;		(*theData)[	14	][1] =	0.218668258	;		(*theData)[	14	][2] =	0.422501991	;		(*theData)[	14	][3] =	0.740089348	;
    (*theData)[	15	][0] =	0.16	;		(*theData)[	15	][1] =	0.668316582	;		(*theData)[	15	][2] =	0.81514738	;		(*theData)[	15	][3] =	0.576630393	;
    (*theData)[	16	][0] =	0.17	;		(*theData)[	16	][1] =	0.043836268	;		(*theData)[	16	][2] =	0.598770425	;		(*theData)[	16	][3] =	0.563087434	;
    (*theData)[	17	][0] =	0.18	;		(*theData)[	17	][1] =	0.792084613	;		(*theData)[	17	][2] =	0.514169662	;		(*theData)[	17	][3] =	0.535603964	;
    (*theData)[	18	][0] =	0.19	;		(*theData)[	18	][1] =	0.560757337	;		(*theData)[	18	][2] =	0.641645888	;		(*theData)[	18	][3] =	0.398060599	;
    (*theData)[	19	][0] =	0.2	;		(*theData)[	19	][1] =	0.430036091	;		(*theData)[	19	][2] =	0.306279873	;		(*theData)[	19	][3] =	0.045069533	;
    (*theData)[	20	][0] =	0.21	;		(*theData)[	20	][1] =	0.308288366	;		(*theData)[	20	][2] =	0.055098947	;		(*theData)[	20	][3] =	0.003702695	;
    (*theData)[	21	][0] =	0.22	;		(*theData)[	21	][1] =	0.103824663	;		(*theData)[	21	][2] =	0.649472287	;		(*theData)[	21	][3] =	0.140314683	;
    (*theData)[	22	][0] =	0.23	;		(*theData)[	22	][1] =	0.255827746	;		(*theData)[	22	][2] =	0.231797303	;		(*theData)[	22	][3] =	0.898299136	;
    (*theData)[	23	][0] =	0.24	;		(*theData)[	23	][1] =	0.002319606	;		(*theData)[	23	][2] =	0.3618559	;		(*theData)[	23	][3] =	0.7575611	;
    (*theData)[	24	][0] =	0.25	;		(*theData)[	24	][1] =	0.1499849	;		(*theData)[	24	][2] =	0.714331122	;		(*theData)[	24	][3] =	0.012090027	;
    (*theData)[	25	][0] =	0.26	;		(*theData)[	25	][1] =	0.496918315	;		(*theData)[	25	][2] =	0.527783221	;		(*theData)[	25	][3] =	0.649306763	;
    (*theData)[	26	][0] =	0.27	;		(*theData)[	26	][1] =	0.311432351	;		(*theData)[	26	][2] =	0.469255924	;		(*theData)[	26	][3] =	0.356750974	;






    // Plot a line
    for (int i=0; i<numberOfDataPoints-1; i++) {
        theGLWidgetEngine->addLine((*theData)[i][1], (*theData)[i][2], (*theData)[i][3], (*theData)[i+1][1], (*theData)[i+1][2], (*theData)[i+1][3], Qt::red);
    }

    // Flush the plot to the display
    theGLWidgetEngine->update3D();

    */

}



void RGroundMotionModel::plotVelocity()
{

}



void RGroundMotionModel::plotDisplacement()
{

}





int RGroundMotionModel::evaluateModel()
{
    return 0;
}

