#include "RAlisjahbana2021FinancingTimeEstimator.h"
#include "RDomain.h"
#include <math.h>
#include <QtMath>
#include "RScenarioModel.h"
#include "RNormalDistribution.h"


RAlisjahbana2021FinancingTimeEstimator::RAlisjahbana2021FinancingTimeEstimator(QObject *parent, QString name)
    : RObject(parent, name)
{
    theMedianDelayForInsurancePayment = 0.0;
    theDispersionOfDelayForInsurancePayments = 0.0;
    theMedianDelayForBankLoans = 0.0;
    theDispersionOfDelayForBankLoans = 0.0;
    theMedianDelayForSBALoans = 0.0;
    theDispersionOfDelayForSBALoans = 0.0;
    theMedianDelayForCDBGDRGrants = 0.0;
    theDispersionOfDelayForCDBGDRGrants = 0.0;
    theAvailableSavings = 0.0;
    theSellingOption = QString("None");
    theSellingDelay = 180.0;
}


RAlisjahbana2021FinancingTimeEstimator::~RAlisjahbana2021FinancingTimeEstimator()
{
}


double RAlisjahbana2021FinancingTimeEstimator::getMedianDelayForInsurancePayments()
{
    return theMedianDelayForInsurancePayment;
}

void RAlisjahbana2021FinancingTimeEstimator::setMedianDelayForInsurancePayments(double value)
{
    theMedianDelayForInsurancePayment = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getMedianDelayForBankLoans()
{
    return theMedianDelayForBankLoans;
}

void RAlisjahbana2021FinancingTimeEstimator::setMedianDelayForBankLoans(double value)
{
    theMedianDelayForBankLoans = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getMedianDelayForSBALoans()
{
    return theMedianDelayForSBALoans;
}

void RAlisjahbana2021FinancingTimeEstimator::setMedianDelayForSBALoans(double value)
{
    theMedianDelayForSBALoans = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getMedianDelayForCDBGDRGrants()
{
    return theMedianDelayForCDBGDRGrants;
}

void RAlisjahbana2021FinancingTimeEstimator::setMedianDelayForCDBGDRGrants(double value)
{
    theMedianDelayForCDBGDRGrants = value;
}

double RAlisjahbana2021FinancingTimeEstimator::getDispersionOfDelayForInsurancePayments()
{
    return theDispersionOfDelayForInsurancePayments;
}

void RAlisjahbana2021FinancingTimeEstimator::setDispersionOfDelayForInsurancePayments(double value)
{
    theDispersionOfDelayForInsurancePayments = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getDispersionOfDelayForBankLoans()
{
    return theDispersionOfDelayForBankLoans;
}

void RAlisjahbana2021FinancingTimeEstimator::setDispersionOfDelayForBankLoans(double value)
{
    theDispersionOfDelayForBankLoans = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getDispersionOfDelayForSBALoans()
{
    return theDispersionOfDelayForSBALoans;
}

void RAlisjahbana2021FinancingTimeEstimator::setDispersionOfDelayForSBALoans(double value)
{
    theDispersionOfDelayForSBALoans = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getDispersionOfDelayForCDBGDRGrants()
{
    return theDispersionOfDelayForCDBGDRGrants;
}

void RAlisjahbana2021FinancingTimeEstimator::setDispersionOfDelayForCDBGDRGrants(double value)
{
    theDispersionOfDelayForCDBGDRGrants = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getSBACap()
{
    return theSBACap;
}

void RAlisjahbana2021FinancingTimeEstimator::setSBACap(double value)
{
    theSBACap = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getCDBGDRCap()
{
    return theCDBGDRCap;
}

void RAlisjahbana2021FinancingTimeEstimator::setCDBGDRCap(double value)
{
    theCDBGDRCap = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getAvailableSavings()
{
    return theAvailableSavings;
}


void RAlisjahbana2021FinancingTimeEstimator::setAvailableSavings(double value)
{
    theAvailableSavings = value;
}

QString RAlisjahbana2021FinancingTimeEstimator::getSellingOption()
{
    return theSellingOption;
}

void RAlisjahbana2021FinancingTimeEstimator::setSellingOption(QString value)
{
    theSellingOption = value;
}


double RAlisjahbana2021FinancingTimeEstimator::getSellingDelay()
{
    return theSellingDelay;
}

void RAlisjahbana2021FinancingTimeEstimator::setSellingDelay(double value)
{
    theSellingDelay = value;
}


double RAlisjahbana2021FinancingTimeEstimator::evaluateFinancingTime(QString theTenure, bool theMortgageStatus, QString theIncome, double theIncomeValue, double theLoss, double theBuildingValue)
{
    RNormalDistribution theNormalDistribution(theDomain);

    double theTimeNeededToGetFunding = theRNG.generateDouble() * 90.0; // Assume households will need some time to figure out immediate needs before start looking for funding
    theBuildingExpectedFunds = 0.0;
    theBuildingAmountFromInsurance = 0.0;
    theBuildingAmountLendedFromBank = 0.0;
    theBuildingAmountLendedFromSBA = 0.0;
    theBuildingAmountGrantedByCDBGDR = 0.0;
    theBuildingAmountMissing = 0.0;

    // Insurance
    double theMedianDelayForPaymentsFromInsurance = 6.0;
    double theDispersionOfDelayForPaymentsFromInsurance = 1.11;
    double m = theMedianDelayForPaymentsFromInsurance;
    double v = theDispersionOfDelayForPaymentsFromInsurance * theDispersionOfDelayForPaymentsFromInsurance;
    double phi = qSqrt(v + m * m);
    double mu = qLn(m * m / phi);
    double sigma = qSqrt(qLn((phi*phi) / (m*m)));
    double theUx = theRNG.generateDouble();
    double theDelaysFromInsurance = 30.0 * theUx + 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);

    // Bank
    double theMedianDelayForPaymentsForBank = 15.0 ;
    double theDispersionOfDelayForPaymentsFromBank = 0.68;
    m = theMedianDelayForPaymentsForBank;
    v = theDispersionOfDelayForPaymentsFromBank * theDispersionOfDelayForPaymentsFromBank;
    phi = qSqrt(v + m * m);
    mu = qLn(m * m / phi);
    sigma = qSqrt(qLn((phi*phi) / (m*m)));
    theUx = theRNG.generateDouble();
    double theDelaysFromBank = 180.0 * theUx + 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);

    // SBA
    double theMedianDelayForPaymentsForSBA = 48.0;
    double theDispersionOfDelayForPaymentsFromSBA = 0.57;
    m = theMedianDelayForPaymentsForSBA;
    v = theDispersionOfDelayForPaymentsFromSBA * theDispersionOfDelayForPaymentsFromSBA;
    phi = qSqrt(v + m * m);
    mu = qLn(m * m / phi);
    sigma = qSqrt(qLn((phi*phi) / (m*m)));
    theUx = theRNG.generateDouble();
    double theDelaysFromSBA = 180.0 * theUx + 7.0 * exp(theNormalDistribution.getInverseNormalCDF(theUx,1.0) * sigma + mu);

    // CDBG
    double theProgramStartDelay = 1.5 * 365.0;
    double theMedianDelayForPaymentsForCDBG = 208.0;
    theUx = theRNG.generateDouble();
    double theDelaysFromCDBG = theProgramStartDelay + 7.0 * theUx * theMedianDelayForPaymentsForCDBG;

    // If the building is owner occupied
    if (theTenure== QString("OwnerOccupied")){

        double rngBankLoan = theRNG.generateDouble();
        double rngSBALoan = theRNG.generateDouble();
        double rngInsurance = theRNG.generateDouble();

        // If insured and considering a deductible of 15%
        if (rngInsurance < 0.15){
            theBuildingExpectedFunds       += qMax(theLoss - 0.15 * theBuildingValue,0.0);
            theBuildingAmountFromInsurance += qMax(theLoss - 0.15 * theBuildingValue,0.0);
            theTimeNeededToGetFunding += theDelaysFromInsurance;
        }

        // If high income
        if (theIncome == QString("High")){

            // If no mortgage get a full loan
            if (theMortgageStatus == false) {
                theBuildingAmountLendedFromBank += theLoss - theBuildingExpectedFunds;
                theBuildingExpectedFunds        += theLoss - theBuildingExpectedFunds;
                theTimeNeededToGetFunding += theDelaysFromBank;
            } // If has mortgage, try bank loan for 50% of income

            else if (rngBankLoan < 0.91){
                theBuildingAmountLendedFromBank += qMin(0.5*theIncomeValue,theLoss-theBuildingExpectedFunds);
                theBuildingExpectedFunds        += qMin(0.5*theIncomeValue,theLoss-theBuildingExpectedFunds);
                theTimeNeededToGetFunding += theDelaysFromBank;
            }

        } // Moderate income
        else if (theIncome == QString("Moderate")){

            // If no mortgage get a full loan
            if (theMortgageStatus == false) {
                theBuildingAmountLendedFromBank += theLoss - theBuildingExpectedFunds;
                theBuildingExpectedFunds        += theLoss - theBuildingExpectedFunds;
                theTimeNeededToGetFunding += theDelaysFromBank;
            }

            // If still not enough
            if (theBuildingExpectedFunds < theLoss){

                // If has mortgage, try SBA loan with 47% to get up to 0.48*needed amount capped at 200,000
                if (rngSBALoan <= 0.47){
                    theBuildingAmountLendedFromSBA += qMin(0.48*(theLoss-theBuildingExpectedFunds),theSBACap);
                    theBuildingExpectedFunds       += qMin(0.48*(theLoss-theBuildingExpectedFunds),theSBACap);
                    theTimeNeededToGetFunding += theDelaysFromSBA;

                    // If still not enough after SBA
                    if (theBuildingExpectedFunds < theLoss){

                        // Try Bank loan
                        // If SBA loan was approved, bank loan will be approved with max 50% of income
                        theBuildingAmountLendedFromBank += qMin(theLoss-theBuildingExpectedFunds,0.5*theIncomeValue);
                        theBuildingExpectedFunds        += qMin(theLoss-theBuildingExpectedFunds,0.5*theIncomeValue);
                        theTimeNeededToGetFunding += theDelaysFromBank;

                        // If still not enough after Bank loan
                        if (theBuildingExpectedFunds < theLoss){
                            // if all else fails use wait for CDBG-DR
                            theBuildingAmountGrantedByCDBGDR += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                            theBuildingExpectedFunds         += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                            theTimeNeededToGetFunding += theDelaysFromCDBG;
                        } // CDBG
                    } // SBA and Bank
                } // If SBA was not approved
                else {
                    // if SBA loan was not approved the chance to get a bank loan is 58%
                    if (rngBankLoan < 0.58){
                        theBuildingAmountLendedFromBank += qMin(theLoss-theBuildingExpectedFunds,0.5*theIncomeValue);
                        theBuildingExpectedFunds        += qMin(theLoss-theBuildingExpectedFunds,0.5*theIncomeValue);
                        theTimeNeededToGetFunding += theDelaysFromBank;

                        // If still not enough after Bank loan
                        if (theBuildingExpectedFunds < theLoss){

                            // if all else fails use wait for CDBG-DR
                            theBuildingAmountGrantedByCDBGDR += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                            theBuildingExpectedFunds         += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                            theTimeNeededToGetFunding += theDelaysFromCDBG;
                        } // CDBG
                    } // If no bank loan

                    else {
                        // if all else fails use wait for CDBG-DR
                        theBuildingAmountGrantedByCDBGDR += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                        theBuildingExpectedFunds         += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                        theTimeNeededToGetFunding += theDelaysFromCDBG;
                    }
                } // SBA
            } // If mortgage
        } // If moderate

        else if (theIncome == QString("Low")){

            // If no mortgage get a full loan
            if (theMortgageStatus == false) {
                theBuildingAmountLendedFromBank += theLoss - theBuildingExpectedFunds;
                theBuildingExpectedFunds        += theLoss - theBuildingExpectedFunds;
                theTimeNeededToGetFunding += theDelaysFromBank;
            }

            // If still not enough
            if (theBuildingExpectedFunds < theLoss){

                // If has mortgage, try SBA loan with 47% to get up to 0.48*needed amount capped at 200,000
                if (rngSBALoan <= 0.47){
                    theBuildingAmountLendedFromSBA += qMin(0.48*(theLoss-theBuildingExpectedFunds),theSBACap);
                    theBuildingExpectedFunds       += qMin(0.48*(theLoss-theBuildingExpectedFunds),theSBACap);
                    theTimeNeededToGetFunding += theDelaysFromSBA;

                    // If still not enough after SBA
                    if (theBuildingExpectedFunds < theLoss){

                        // Try Bank loan
                        // If SBA loan was approved, bank loan will be approved with max 50% of income
                        theBuildingAmountLendedFromBank += qMin(theLoss-theBuildingExpectedFunds,0.5*theIncomeValue);
                        theBuildingExpectedFunds        += qMin(theLoss-theBuildingExpectedFunds,0.5*theIncomeValue);
                        theTimeNeededToGetFunding += theDelaysFromBank;

                        // If still not enough after Bank loan
                        if (theBuildingExpectedFunds < theLoss){
                            // if all else fails use wait for CDBG-DR
                            theBuildingAmountGrantedByCDBGDR += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                            theBuildingExpectedFunds         += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                            theTimeNeededToGetFunding += theDelaysFromCDBG;

                        } // CDBG
                    } // SBA and Bank
                } // If SBA was not approved
                else {
                    // if SBA loan was not approved the chance to get a bank loan is 19%
                    if (rngBankLoan < 0.19){
                        theBuildingAmountLendedFromBank += qMin(theLoss-theBuildingExpectedFunds,0.5*theIncomeValue);
                        theBuildingExpectedFunds        += qMin(theLoss-theBuildingExpectedFunds,0.5*theIncomeValue);
                        theTimeNeededToGetFunding += theDelaysFromBank;

                        // If still not enough after Bank loan
                        if (theBuildingExpectedFunds < theLoss){

                            // if all else fails use wait for CDBG-DR
                            theBuildingAmountGrantedByCDBGDR += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                            theBuildingExpectedFunds         += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                            theTimeNeededToGetFunding += theDelaysFromCDBG;
                        } // CDBG
                    } // If no bank loan
                    else {
                        // if all else fails use wait for CDBG-DR
                        theBuildingAmountGrantedByCDBGDR += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                        theBuildingExpectedFunds         += qMin(theLoss-theBuildingExpectedFunds,theCDBGDRCap);
                        theTimeNeededToGetFunding += theDelaysFromCDBG;
                    }
                } // SBA
            } // If mortgage
        } // Low income
        // If the building is renter-occupied

    } else if (theTenure== QString("RenterOccupied")) {

        double rngBankLoan = theRNG.generateDouble();

        // If no mortgage get a full loan
        if (rngBankLoan < 0.91) {
            theBuildingAmountLendedFromBank += theLoss - theBuildingExpectedFunds;
            theBuildingExpectedFunds += theLoss - theBuildingExpectedFunds;
            theTimeNeededToGetFunding += 2.0 * theDelaysFromBank;
        } //

    } else {
        qCritical() << this->objectName() << "has a problem with the building tenure.";
    }


    double theSavings = theAvailableSavings * theIncomeValue;

    // If I dont have the money yet but I am willing to use my savings
    theBuildingExpectedFunds += theSavings;

    if (theBuildingExpectedFunds > theLoss){
        if (theSellingOption == QString("SellEarly")){
            return theSellingDelay + 180.0 * theRNG.generateDouble();
        }
        else if (theSellingOption == QString("SellAfter")){
            return theTimeNeededToGetFunding + 180.0 * theRNG.generateDouble();
        }
        else {
            return 999999.0; // not successfull at obtaining funding
        }
    } else {
        return theTimeNeededToGetFunding; // not successfull at obtaining funding
    }
}

