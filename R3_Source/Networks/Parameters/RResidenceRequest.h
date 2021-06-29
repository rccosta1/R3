#ifndef RResidenceRequest_H
#define RResidenceRequest_H

#include "RParameter.h"
#include "RDomain.h"
#include "RRequest.h"

class RModel;

class RResidenceRequest : public RRequest
{
    Q_OBJECT

public:
    RResidenceRequest(QObject *parent, QString name);
    virtual ~RResidenceRequest();


private:


};

#endif // RResidenceRequest_H
