#include "RGenericResponse.h"
#include "RMainWindow.h"

RGenericResponse::RGenericResponse(QObject *parent, QString name)
    : RResponse(parent, name)
{

}

RGenericResponse::~RGenericResponse()
{
    if(theTimeSeries)
    {
        theDomain->deleteObject(theTimeSeries->objectName());
        theTimeSeries = nullptr;
    }
}




QStringList RGenericResponse::getActionList()
{
    QStringList actionList;
    actionList << "&Help";
    return actionList;
}


