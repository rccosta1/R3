#ifndef RUserDefinedMagnitudeModel_H
#define RUserDefinedMagnitudeModel_H

#include "RModel.h"
#include "RContinuousRandomVariable.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RTime.h"

class RUserDefinedMagnitudeModel : public RModel
{
	Q_OBJECT

    Q_PROPERTY(QString XPoints READ getXPoints WRITE setXPoints)
    Q_PROPERTY(QString PDFPoints READ getPDFPoints WRITE setPDFPoints)
    Q_PROPERTY(QObject *Occurrence READ getOccurrence WRITE setOccurrence)

public:

	RUserDefinedMagnitudeModel(QObject *parent, QString name);
	~RUserDefinedMagnitudeModel();

    QString getXPoints();
    void setXPoints(QString value);

    QString getPDFPoints();
    void setPDFPoints(QString value);

    QObject *getOccurrence() const;
    void setOccurrence(QObject *value);

	int evaluateModel();

private:

    QPointer<RContinuousRandomVariable> theStandardNormalRandomVariable;
    QString theXPoints;
    QString thePDFPoints;
    RResponse *theMagnitude;
    QPointer<RParameter> theOccurrence;

};

#endif
