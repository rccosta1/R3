#ifndef RFixedTimeOccurrenceModel_H
#define RFixedTimeOccurrenceModel_H

#include "RModel.h"
#include "RResponse.h"
#include "ROccurrenceModel.h"

class RParameter;
class RResponse;

#ifdef NETWORKS
#include "RPacket.h"
#endif

class RFixedTimeOccurrenceModel : public RModel
{
	Q_OBJECT

    Q_PROPERTY(QObject *OccurrenceTime READ getOccurrenceTime WRITE setOccurrenceTime)

public:

    RFixedTimeOccurrenceModel(QObject *parent, QString name);
	~RFixedTimeOccurrenceModel();

    QObject *getOccurrenceTime() const;
    void setOccurrenceTime(QObject *value);

    QList<QPair<double, QString> > getTriggerTimes(double startTime, double endTime);
	int evaluateModel();

private:

    QPointer<RTime> theOccurrenceTime;
    RResponse *theResponse;
};

#endif
