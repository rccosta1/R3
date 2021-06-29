#ifndef RRequest_H
#define RRequest_H

#include "RParameter.h"
#include "RDomain.h"

class RModel;

class RRequest : public RParameter
{
    Q_OBJECT

public:
    RRequest(QObject *parent, QString name);
    virtual ~RRequest();

    RModel* getModel();
    void setModel(RModel *theModel);

    QVector<int> getRequesterIDList();
    void setRequesterIDList(int value);

    QVector<double> getRequestedAmountList();
    void setRequestedAmountList(double value);

    QString getContentType();
    void setContentType(QString theType);

    void clearRequesterIDList();
    void clearRequestedAmountList();

private:

    // Data member for meta-properties
    RModel *theModelPassingThisRequest;
    double theRequestCriteria;
    QVector<int> theRequesterIDList;
    QVector<double> theRequestAmountList;
    QString theContentType;
    QVector<QString> theRequesterArchetypeList;
    QVector<double> theRequesterNumber;

};

#endif // RRequest_H
