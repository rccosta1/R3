#include "RBridgeModel.h"
#include "RParameter.h"
#include "RResponse.h"
#include "RContinuousRandomVariable.h"
#include "RNormalDistribution.h"
#include "RIntensityModel.h"
#include <algorithm>
#include <math.h>


RBridgeModel::RBridgeModel(QObject *parent, QString name)
    : RNetworkModel(parent, name)
{
    theLocation = 0;
    theAnalysisType = RBridgeModel::GroundShaking;
    theBridgeType = RBridgeModel::HWB1;
    theNumberOfSpans = 1;
    theAngleOfSkew = 0;
    theBridgeLength = 100;
    theSpanWidth = 10;
    
    tempCounter = 0;
    theRepairTimeMedians << 0.6  << 2.5  << 75.0  << 230.0;
    theRepairTimeStandardDeviations << 0.6 << 2.7 << 42.0 << 110.0;

    // Instantiating a packet response
    theDomain->createObject(objectName() + "Response", "RPacketResponse");
    theResponse = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theResponse->setModel(this);

    // Instantiating the response
    theDomain->createObject(objectName() + "PercentFunctionalResponse", "RGenericResponse");
    thePercentFunctional = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    thePercentFunctional->setModel(this);

    theDomain->createObject(objectName() + "DamageFactorResponse", "RGenericResponse");
    theDamageFactor = qobject_cast<RResponse *>(theDomain->getLastAddedObject());
    theDamageFactor->setModel(this);

    // Create the model parameter
    theDomain->createObject(objectName() + "_theta", "RContinuousRandomVariable");
    theModelUncertainty = qobject_cast<RContinuousRandomVariable *>(theDomain->getLastAddedObject());
    theModelUncertainty->setDistributionType(QString("Normal (mean, stdv)"));
    theModelUncertainty->setMean("0.0");
    theModelUncertainty->setStandardDeviation("1.0");
    theAutoParametersList << theModelUncertainty;

}

RBridgeModel::~RBridgeModel()
{

}


int RBridgeModel::resetTime()
{
    theTimeUnderRepairs = 0.0;
    timeOfEvent = 0.0;
    thePercentFunctional->setCurrentValue(1.0);
    theFunctionality = 1.0;
    timePacketsCurrentlyOnboardWereReceived.clear();

    return 0;
}


RBridgeModel::RAnalysisType RBridgeModel::getAnalysisType() const
{	
    return theAnalysisType;
}	

void RBridgeModel::setAnalysisType(RBridgeModel::RAnalysisType value)
{	
    theAnalysisType = value;
}	

RBridgeModel::RBridgeType RBridgeModel::getBridgeType() const
{	
    return theBridgeType;
}	

void RBridgeModel::setBridgeType(RBridgeModel::RBridgeType value)
{	
    theBridgeType = value;
}	


QString RBridgeModel::getSaIntensityList()
{
    return parameterListToString(theSaIntensityList);
}

void RBridgeModel::setSaIntensityList(QString value)
{
    theSaIntensityList = stringToParameterList(value);
}


QString RBridgeModel::getPGDIntensityList()
{
    return parameterListToString(thePGDIntensityList);
}

void RBridgeModel::setPGDIntensityList(QString value)
{
    thePGDIntensityList = stringToParameterList(value);
}


QString RBridgeModel::getInputParameterList()
{
    return parameterListToString(theInputParameterList);
}

void RBridgeModel::setInputParameterList(QString value)
{
    theInputParameterList = stringToParameterList(value);
}


int RBridgeModel::getNumberOfSpans() const
{	
    return theNumberOfSpans;
}	

void RBridgeModel::setNumberOfSpans(int value)
{	
    theNumberOfSpans = value;
}	


double RBridgeModel::getAngleOfSkew() const
{	
    return theAngleOfSkew;
}	

void RBridgeModel::setAngleOfSkew(double value)
{	
    theAngleOfSkew = value;
}	


double RBridgeModel::getBridgeLength() const
{	
    return theBridgeLength;
}	

void RBridgeModel::setBridgeLength(double value)
{	
    theBridgeLength = value;
}	


double RBridgeModel::getSpanWidth() const
{	
    return theSpanWidth;
}	

void RBridgeModel::setSpanWidth(double value)
{	
    theSpanWidth = value;
}	


int RBridgeModel::evaluateModel()
{	
    tempCounter++;
    probabilityVector.clear();

    // Output
    if (theOutputLevel == RDomain::Maximum) {
        qDebug() << "";
        qDebug() << "Now evaluating" << this->objectName();
    }


    // -------------------------------------------------------------------------------------------
    // 1 - Checking responses coming to this model
    // -------------------------------------------------------------------------------------------
    QList<QPointer <RParameter>> thePacketList;

    // Check if the households in this zone have an object to request disaster aid from
    for (int i = 0; i < theInputParameterList.size(); i++){

        // Cast the input as a response
        RPacketResponse *theResponseComingToThisModel = qobject_cast<RPacketResponse *>(theInputParameterList.at(i));

        if (theResponseComingToThisModel){
            thePacketList << theInputParameterList.at(i);
        }
    }



    // Load the packets received
    int numPackets = 0;
    int newPackets = packetsCurrentlyOnboard.size();
    this->loadPackets(thePacketList);
    numPackets = packetsCurrentlyOnboard.size();
    newPackets = numPackets - newPackets;

    if (newPackets > 0){
        for (int i = 0; i < newPackets; i++){
            timePacketsCurrentlyOnboardWereReceived << theCurrentTime->getCurrentValue();
        }
    }




    for (int i = 0; i < timePacketsCurrentlyOnboardWereReceived.size(); i++){
        int ii = timePacketsCurrentlyOnboardWereReceived.size() - (1 + i);


        //if (roadLength < theVelocity * (theCurrentTime->getCurrentValue() - timePacketsCurrentlyOnboardWereReceived.at(ii)) * 24.0){
        timePacketsCurrentlyOnboardWereReceived.removeAt(ii);
        unloadPackets(qobject_cast<RPacketResponse *>(theResponse),packetsCurrentlyOnboard);
        // }
    }





    // GROUND MOTION INTENSITY
    RResponse *theResponseObject;
    RIntensityModel *theIntensityModel;

    if (Sa1 == 0.0){
        for (int i = 0; i < theSaIntensityList.count(); i++) {

            // Get the pointer to the intensity model
            theResponseObject = qobject_cast<RResponse *>(theSaIntensityList[i]);
            theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());

            // Tell that intensity model what we want
            theIntensityModel->setLocation(theLocation);
            theIntensityModel->setPeriod(1.0);
            theIntensityModel->setIntensityMeasure(QString("Sa"));

            // Run the intensity model and get the response
            theIntensityModel->evaluateModel();
            Sa1 += theResponseObject->getCurrentValue();

            // Change the period to 0.3 sec and do it again
            theIntensityModel->setPeriod(0.3);
            theIntensityModel->evaluateModel();
            Sa0_3 += theResponseObject->getCurrentValue();
            //qDebug() << "The PGA at" << this->objectName() << "is" << Sa1;
        }
    }


    if(PGD == 0.0) {
        for (int i = 0; i < thePGDIntensityList.count(); i++) {

            // Get the pointer to the intensity model
            theResponseObject = qobject_cast<RResponse *>(thePGDIntensityList[i]);
            theIntensityModel = qobject_cast<RIntensityModel *>(theResponseObject->getModel());

            // Tell the intensity model what we want
            theIntensityModel->setLocation(theLocation);
            theIntensityModel->setPeriod(1.0);

            // Evaluate the intensity modela and get the response
            theIntensityModel->evaluateModel();
            PGD += theResponseObject->getCurrentValue();
        }
    }

    double timeO = 0.0;

    // Setting the time of event to the first time an intensity measured is identified to be greater than zero
    if (timeOfEvent == 0.0 && (Sa1 > 0.0 || PGD > 0.0)){
        timeOfEvent = theCurrentTime->getCurrentValue();
    }

    double timeSinceEvent = theCurrentTime->getCurrentValue() - timeOfEvent;

    double A;
    int B;
    int IShape;
    double betaSa = 0.6;
    double betaPGD = 0.2;
    QVector<double> SaVector;
    QVector<double> PGDVector;
    double f1;
    double f2;
    double fSine = sin(3.14159265359 / 180.0 * (theAngleOfSkew));
    double fLong = 0.5 * theBridgeLength / (theNumberOfSpans * theSpanWidth * sin(3.14159265359 / 180.0 * (theAngleOfSkew)));

    // Damage ratios - Hazus pg. 626
    // These data are from the tables in chapter 7 of the FEMA-NIBS document
    if (theBridgeType == RBridgeModel::HWB1) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.4 << 0.5 << 0.7 << 0.9;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0; // fSine  // fLong
        f2 = 1.0;

    } else if (theBridgeType == RBridgeModel::HWB2) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.6 << 0.9 << 1.1 << 1.7;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = 1.0;

    } else if (theBridgeType == RBridgeModel::HWB3) {
        A = 0.25;
        B = 1;
        IShape = 1;

        SaVector << 0.8 << 1.0 << 1.2 << 1.7;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = 1.0;

    } else if (theBridgeType == RBridgeModel::HWB4) {
        A = 0.25;
        B = 1;
        IShape = 1;

        SaVector << 0.8 << 1.0 << 1.2 << 1.7;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = 1.0;

    } else if (theBridgeType == RBridgeModel::HWB5) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.25 << 0.35 << 0.45 << 0.70;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB6) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.3 << 0.5 << 0.6 << 0.9;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB7) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.5 << 0.8 << 1.1 << 1.7 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB8) {
        A = 0.33;
        B = 0;
        IShape = 0;

        SaVector << 0.35 << 0.45 << 0.55 << 0.8 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB9) {
        A = 0.33;
        B = 1;
        IShape = 0;

        SaVector << 0.6 << 0.9 << 1.3 << 1.6 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB10) {
        A = 0.33;
        B = 0;
        IShape = 1;

        SaVector << 0.6 << 0.9 << 1.1 << 1.5 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB11) {
        A = 0.33;
        B = 1;
        IShape = 1;

        SaVector << 0.9 << 0.9 << 1.1 << 1.5 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB12) {
        A = 0.09;
        B = 1;
        IShape = 0;

        SaVector << 0.25 << 0.35 << 0.45 << 0.7 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB13) {
        A = 0.09;
        B = 1;
        IShape = 0;

        SaVector << 0.3 << 0.5 << 0.6 << 0.9 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB14) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.5 << 0.8 << 1.1 << 1.7 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB15) {
        A = 0.05;
        B = 0;
        IShape = 1;

        SaVector << 0.75 << 0.75 << 0.75 << 1.1 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB16) {
        A = 0.33;
        B = 1;
        IShape = 1;

        SaVector << 0.9 << 0.9 << 1.1 << 1.5 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB17) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.25 << 0.35 << 0.45 << 0.7 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB18) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.3 << 0.5 << 0.6 << 0.9 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB19) {
        A = 0.25;
        B = 1;
        IShape = 0;

        SaVector << 0.5 << 0.8 << 1.1 << 1.7 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB20) {
        A = 0.33;
        B = 0;
        IShape = 0;

        SaVector << 0.35 << 0.45 << 0.55 << 0.8 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB21) {
        A = 0.33;
        B = 1;
        IShape = 0;

        SaVector << 0.6 << 0.9 << 1.3 << 1.6 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB22) {
        A = 0.33;
        B = 0;
        IShape = 1;

        SaVector << 0.6 << 0.9 << 1.1 << 1.5 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB23) {
        A = 0.33;
        B = 1;
        IShape = 1;

        SaVector << 0.9 << 0.9 << 1.1 << 1.5 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB24) {
        A = 0.2;
        B = 1;
        IShape = 0;

        SaVector << 0.25 << 0.35 << 0.45 << 0.7 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB25) {
        A = 0.2;
        B = 1;
        IShape = 0;

        SaVector << 0.3 << 0.5 << 0.6 << 0.9 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = fLong;
        f2 = fLong;

    } else if (theBridgeType == RBridgeModel::HWB26) {
        A = 0.1;
        B = 0;
        IShape = 1;

        SaVector << 0.75 << 0.75 << 0.75 << 1.1 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB27) {
        A = 0.1;
        B = 0;
        IShape = 1;

        SaVector << 0.75 << 0.75 << 0.75 << 1.1 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = fSine;

    } else if (theBridgeType == RBridgeModel::HWB28) {
        // Attention: A, B, and IShape for this bridge type are not given in the FEMA-NIBS document
        A = 0.1;
        B = 0;
        IShape = 1;

        SaVector << 0.8 << 1.0 << 1.2 << 1.7 ;
        PGDVector << 3.9 << 3.9 << 3.9 << 13.8;

        f1 = 1.0;
        f2 = 1.0;
    }


    // Calculating the modifiers
    double KSkew = sqrt(sin(3.14159265359 / 180.0 * (90.0 - theAngleOfSkew)));
    double KShape = 2.5 * Sa1 / Sa0_3;
    double K3D = 1.0 + (double) (A / (theNumberOfSpans - B));

    double slightFactor;
    if (IShape == 0) {
        slightFactor = 1.0;
    } else {
        slightFactor = std::min(1.0, KShape);
    }

    SaVector[0] *= slightFactor;
    for (int i = 1; i < SaVector.count(); i++) {
        SaVector[i] *= K3D * KSkew;
    }

    PGDVector[3] *= f2;
    for (int i = 0; i < PGDVector.count() - 1; i++) {
        PGDVector[i] *= f1;
    }


    // Calculate the probability of each damage states
    double prob = 1.0;
    RNormalDistribution theNormalDistribution(theDomain);

    for (int i = 0; i < SaVector.count(); i++) {
        double oldProb = prob;
        if (theAnalysisType == RBridgeModel::GroundShaking) {
            prob = theNormalDistribution.getNormalCDF(1.0 / betaSa * log(Sa1 / SaVector[i]), 1);

        } else if (theAnalysisType == RBridgeModel::GroundFailure) {
            prob = theNormalDistribution.getNormalCDF(1.0 / betaPGD * log(PGD / PGDVector[i]), 1);

        } else if (theAnalysisType == RBridgeModel::Both) {
            double PGDProb = theNormalDistribution.getNormalCDF(1.0 / betaPGD * log(PGD / PGDVector[i]), 1);
            double SaProb = theNormalDistribution.getNormalCDF(1.0 / betaSa * log(Sa1 / SaVector[i]), 1);
            prob = PGDProb + SaProb - PGDProb * SaProb;
        }
        probabilityVector << oldProb - prob;
    }
    probabilityVector << prob - 0.0;

    // Enter the central damage factors (Terje's addendum: Table 15.18)
    QVector<double> CDFVector;
    double completeCDF = 100.0;
    if (theNumberOfSpans > 2) {
        completeCDF = 2.0 / theNumberOfSpans * 100.0;
    }

    CDFVector << 0.0 << 3.0 << 8.0 << 25.0 << completeCDF;
    //CDFVector << 1.0 << 2.0 << 3.0 << 4.0 << 5.0;

    // Caluclate the mean damage factor and standard deviation
    double MDF = 0;
    double sigma2DF = 0;
    for (int i = 0; i < CDFVector.count(); i++) {
        MDF += CDFVector[i] * probabilityVector[i];
        sigma2DF += CDFVector[i] * CDFVector[i] * probabilityVector[i];
    }
    double sigmaDF = sqrt(sigma2DF - MDF * MDF);


    // Calculate the percent functional for each damage state at the "time" days after the earthquake
    QVector<double> percentFunctionalVector;

    percentFunctionalVector << 1.0;
    for (int i = 0; i < theRepairTimeMedians.count(); i++) {
        percentFunctionalVector << theNormalDistribution.getNormalCDF(timeSinceEvent - theRepairTimeMedians[i],  theRepairTimeStandardDeviations[i]);
    }


    // Caluclate the mean percent functional and standard deviation
    double MPF = 0;
    double sigma2PF = 0;
    for (int i = 0; i < percentFunctionalVector.count(); i++) {
        MPF += percentFunctionalVector[i] * probabilityVector[i];
        sigma2PF += percentFunctionalVector[i] * percentFunctionalVector[i] * probabilityVector[i];
    }
    double sigmaPF = sqrt(sigma2PF - MPF * MPF);
    if (qAbs(sigma2PF - MPF * MPF) < 1e-8) {
        sigmaPF = 0.0;
    }


    /*

    // Apply model error
    double modelUcertainty = theModelUncertainty->getCurrentValue();
    double modelUcertaintyProb = theNormalDistribution.getNormalCDF(modelUcertainty, 1.0);

    // Calculating damage factor

    // Considering DF = LN(MDF, sigmaDF)
    double lognormalZetaDF = log(MDF)-0.5*log(1+pow((sigmaDF/MDF), 2.0));
    double lognormalSigmaDF = sqrt(log(pow((sigmaDF/MDF), 2.0) + 1));
    // Inverse lognormal CDF, by using the standard normal inverse CDF:
    double z = theNormalDistribution.getInverseNormalCDF(modelUcertaintyProb, 1.0);
    double damageFactor = exp(lognormalSigmaDF*z) / exp(lognormalZetaDF);
    damageFactor /= 100.0;

    // Calculating percent functional

    // Considering PF = LN(MPF, sigmaPF)
    double lognormalZetaPF = log(MPF)-0.5*log(1+pow((sigmaPF/MPF), 2.0));
    double lognormalSigmaPF = sqrt(log(pow((sigmaPF/MPF), 2.0) + 1));
    double percentFunctional = exp(lognormalSigmaPF*z) / exp(lognormalZetaPF);


    // Adjusting damage factor
    if (damageFactor < 0) {
        damageFactor = 0;
    }
    if (damageFactor > 1.0) {
        damageFactor = 1.0;
    }


    // Adjusting percent functional
    if (percentFunctional < 0) {
        percentFunctional = 0;
    }
    if (percentFunctional > 1.0) {
        percentFunctional = 1.0;
    }

    */
    double damageFactor = MDF;
    double percentFunctional = MPF;


    // Updating the response current value
    thePercentFunctional->setCurrentValue(percentFunctional);
    theDamageFactor->setCurrentValue(damageFactor);
    theFunctionality = thePercentFunctional->getCurrentValue();


    // Getting hold of how many days have passed since the facility lost functionality
    if (theFunctionality <= theThresholdfunctionality){
        theTimeUnderRepairs += theCurrentTime->getCurrentValue() - lastEvaluationTime;
    }

    return 0;
}


double RBridgeModel::calculateTimeUntilGivenRestorationRatio(double ratio)
{
    QVector<double> timeVector;
    double meanTime = 0.0;
    double probSum = 0.0;

    for (int i=0; i<probabilityVector.size(); i++) {
        probSum += probabilityVector.at(i);
    }

    if (probSum != 0.0) {

        // Create an auxiliary normal distribution
        RNormalDistribution theNormalDistribution(theDomain);


        // Record time since event
        double timeSinceEvent = 0.0;//theCurrentTime->getCurrentValue()-timeOfEvent;;


        // Calculate the time from event until given ratio, for each damage state

        for (int i = 0; i < theRepairTimeMedians.count(); i++) {
            timeVector << theRepairTimeMedians[i] + theNormalDistribution.getInverseNormalCDF(ratio, theRepairTimeStandardDeviations[i]) - timeSinceEvent;
        }


        // Calculate the mean of those times; does that make sense?
        for (int i = 0; i < timeVector.count(); i++) {
            meanTime += timeVector[i] * probabilityVector[i+1];
        }
    }

    else {
        meanTime = 1.0;
    }

    return meanTime;
}


