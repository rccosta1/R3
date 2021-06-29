#ifndef RLoadBaseClass_H
#define RLoadBaseClass_H

#include "RObject.h"
#include "RParameter.h"

class RLoadBaseClass : public RObject
{
	Q_OBJECT

    Q_PROPERTY(ROutputLevelType OutputLevel READ getOutputLevel WRITE setOutputLevel)

public:
    RLoadBaseClass(QObject *parent, QString name);
    virtual ~RLoadBaseClass();

	// Runs the model analysis by first calling the runAnalysis for upstream responses
    int runAnalysis(RGradientType theGradientType);

    virtual double getFOGradient() = 0;
    virtual double getSOGradient() = 0;

    RParameter *getParameter() const {	return theParameter;}



protected:

    RParameter* theParameter = nullptr;


private:

    RGradientType theGradientType;
	
};

#endif // RLoadBaseClass_H
