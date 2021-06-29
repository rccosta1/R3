#include "RResponse.h"
#include "RModel.h"
#include "RDomain.h"


RResponse::RResponse(QObject *parent, QString name)
    : RParameter(parent, name)
{
    isAnalyzedFlag = false;
    theModel = 0;
    theTotalStandardDeviation = 0;
    theBetaSensitivityWrtTotalStandardDeviation = 0;
    theTotalEpistemicStandardDeviation = 0;
    theBetaSensitivityWrtTotalEpistemicStandardDeviation = 0;
    theModelStandardDeviation = 0;
    theBetaSensitivityWrtModelStandardDeviation = 0;
    theModelEpistemicStandardDeviation = 0;
    theBetaSensitivityWrtModelEpistemicStandardDeviation = 0;
}

RResponse::~RResponse()
{

}



void RResponse::setCurrentValue(double value)
{
    //RParameter *parameter = qobject_cast<RParameter *>(this);
    //parameter->theCurrentValue(value);
    RParameter::setCurrentValue(value);
    this->setAnalyzed(true);
}

bool RResponse::isAnalyzed()
{
    return isAnalyzedFlag;
}

void RResponse::setAnalyzed(bool analyzed)
{
    //isAnalyzedFlag = analyzed;

    isAnalyzedFlag = false;

    //qDebug() << "Terje is overriding the analyzed flag here.";
    //isAnalyzedFlag = false;
}

QObject * RResponse::getModel() const
{
    return theModel;
}

double RResponse::getTotalStandardDeviation() const	
{	
    return theTotalStandardDeviation;
}	

void RResponse::setTotalStandardDeviation(double value)	
{	
    theTotalStandardDeviation = value;
}	

double RResponse::getBetaSensitivityWrtTotalStandardDeviation() const	
{	
    return theBetaSensitivityWrtTotalStandardDeviation;
}	

void RResponse::setBetaSensitivityWrtTotalStandardDeviation(double value)	
{	
    theBetaSensitivityWrtTotalStandardDeviation = value;
}	

double RResponse::getTotalEpistemicStandardDeviation() const	
{	
    return theTotalEpistemicStandardDeviation;
}	

void RResponse::setTotalEpistemicStandardDeviation(double value)	
{	
    theTotalEpistemicStandardDeviation = value;
}	

double RResponse::getBetaSensitivityWrtTotalEpistemicStandardDeviation() const	
{	
    return theBetaSensitivityWrtTotalEpistemicStandardDeviation;
}	

void RResponse::setBetaSensitivityWrtTotalEpistemicStandardDeviation(double value)	
{	
    theBetaSensitivityWrtTotalEpistemicStandardDeviation = value;
}	

double RResponse::getModelStandardDeviation() const	
{	
    return theModelStandardDeviation;
}	

void RResponse::setModelStandardDeviation(double value)	
{	
    theModelStandardDeviation = value;
}	

double RResponse::getBetaSensitivityWrtModelStandardDeviation() const	
{	
    return theBetaSensitivityWrtModelStandardDeviation;
}	

void RResponse::setBetaSensitivityWrtModelStandardDeviation(double value)	
{	
    theBetaSensitivityWrtModelStandardDeviation = value;
}	

double RResponse::getModelEpistemicStandardDeviation() const	
{	
    return theModelEpistemicStandardDeviation;
}	

void RResponse::setModelEpistemicStandardDeviation(double value)	
{	
    theModelEpistemicStandardDeviation = value;
}	

double RResponse::getBetaSensitivityWrtModelEpistemicStandardDeviation() const	
{	
    return theBetaSensitivityWrtModelEpistemicStandardDeviation;
}	

void RResponse::setBetaSensitivityWrtModelEpistemicStandardDeviation(double value)	
{	
    theBetaSensitivityWrtModelEpistemicStandardDeviation = value;
}	

void RResponse::setModel(QObject *value)
{
    RModel *tempModel = qobject_cast<RModel *>(value);
    if (tempModel) {
        theModel = tempModel;
        setParent(theModel);
    } else {
        if (theModel) {
            setParent(theModel->parent());
        }
        theModel = 0;
    }
}

void RResponse::backup()
{
    backupValue = theCurrentValue;
}

void RResponse::restore()
{
    theCurrentValue = backupValue;
}

QVector<double> RResponse::getValueVector()
{
    QVector<double> resultVector;
    resultVector << theCurrentValue;
    return resultVector;
}

QVector<double> RResponse::getBackupVector()
{
    QVector<double> resultVector;
    resultVector << backupValue;
    return resultVector;
}

void RResponse::setValueVector(QVector<double> valueVector)
{
    if (valueVector.isEmpty()) {
        qCritical() << "Error: The value vector passed to the response" << objectName() << " is empty.";
        return;
    }
    theCurrentValue = valueVector[0];
}


int RResponse::updateValue()
{
    return 0;
}
