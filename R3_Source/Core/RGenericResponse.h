#ifndef RGenericResponse_H
#define RGenericResponse_H

#include "RResponse.h"
#include "RTimeSeries.h"
#include <memory>

class RGenericResponse : public RResponse
{
    Q_OBJECT

public:
    RGenericResponse(QObject *parent, QString name);
    ~RGenericResponse();

    QStringList getActionList();

private:

       RTimeSeries* theTimeSeries = nullptr;

};

#endif
