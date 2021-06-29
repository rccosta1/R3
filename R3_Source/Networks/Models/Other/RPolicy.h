#ifndef RPolicy_H
#define RPolicy_H

#include "RModel.h"
#include "RPacket.h"
#include "RResponse.h"
#include "RRequest.h"
#include "RNetworkModel.h"
#include "qcustomplot.h"

class RParameter;
class RResponse;

class RPolicy : public RObject
{ 
    //Q_PROPERTY(QObject *Location READ getLocation WRITE setLocation)

    Q_OBJECT

public:

    RPolicy(QObject *parent, QString name);
    ~RPolicy();

    int resetTime();


protected:


private: 

};

#endif
