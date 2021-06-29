#include "RRequest.h"
#include "RModel.h"
#include "RDomain.h"


RRequest::RRequest(QObject *parent, QString name)
    : RParameter(parent, name)
{
    theContentType = QString("");
    //qCritical() << "A new request was created";
}

RRequest::~RRequest()
{

}


RModel* RRequest::getModel()
{
    return theModelPassingThisRequest;
}

void RRequest::setModel(RModel *theModel)
{
    theModelPassingThisRequest = theModel;
}


QVector<int> RRequest::getRequesterIDList()
{
    return theRequesterIDList;
}

void RRequest::setRequesterIDList(int value)
{
    theRequesterIDList << value;
}


QVector<double> RRequest::getRequestedAmountList()
{
    return theRequestAmountList;
}

void RRequest::setRequestedAmountList(double value)
{
    theRequestAmountList << value;
}


QString RRequest::getContentType()
{
    return theContentType;
}

void RRequest::setContentType(QString theType)
{
    theContentType = theType;
    //qCritical() << "A new request was created" << theContentType;
}


void RRequest::clearRequesterIDList()
{
    theRequesterIDList.clear();
}

void RRequest::clearRequestedAmountList()
{
    theRequestAmountList.clear();
}





