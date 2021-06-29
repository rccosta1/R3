#ifndef RAlisjahbana2021FinancingTimeEstimator_H
#define RAlisjahbana2021FinancingTimeEstimator_H

#include "RContinuousRandomVariable.h"
#include <math.h>

class RParameter;
class RResponse;

/*
 By Rodrigo Costa (2021)
 This class is used to simulate the time needed for a household to obtain funds and is called by the neighborhood models
 Inputs are the medians, dispersions, and cap (maximum payment) for the payment times for each source
 Available savings: the savings of the household as fraction of the household income, 1 = 100% income available as savings
 Selling option: what happens when the household is unable to get funding. It can sell or do nothing.
 Selling delay: time needed to sell the building
*/

class RAlisjahbana2021FinancingTimeEstimator : public RObject
{

    Q_OBJECT
    Q_PROPERTY(double MedianDelayForInsurancePayments READ getMedianDelayForInsurancePayments WRITE setMedianDelayForInsurancePayments)
    Q_PROPERTY(double DispersionOfDelayForInsurancePayments READ getDispersionOfDelayForInsurancePayments WRITE setDispersionOfDelayForInsurancePayments)
    Q_PROPERTY(double MedianDelayForBankLoans READ getMedianDelayForBankLoans WRITE setMedianDelayForBankLoans)
    Q_PROPERTY(double DispersionOfDelayForBankLoans READ getDispersionOfDelayForBankLoans WRITE setDispersionOfDelayForBankLoans)
    Q_PROPERTY(double MedianDelayForSBALoans READ getMedianDelayForSBALoans WRITE setMedianDelayForSBALoans)
    Q_PROPERTY(double DispersionOfDelayForSBALoans READ getDispersionOfDelayForSBALoans WRITE setDispersionOfDelayForSBALoans)
    Q_PROPERTY(double SBACap READ getSBACap WRITE setSBACap)
    Q_PROPERTY(double MedianDelayForCDBGDRGrants READ getMedianDelayForCDBGDRGrants WRITE setMedianDelayForCDBGDRGrants)
    Q_PROPERTY(double DispersionOfDelayForCDBGDRGrants READ getDispersionOfDelayForCDBGDRGrants WRITE setDispersionOfDelayForCDBGDRGrants)
    Q_PROPERTY(double CDBGDRCap READ getCDBGDRCap WRITE setCDBGDRCap)
    Q_PROPERTY(double AvailableSavings READ getAvailableSavings WRITE setAvailableSavings)
    Q_PROPERTY(QString SellingOption READ getSellingOption WRITE setSellingOption)
    Q_PROPERTY(double SellingDelay READ getSellingDelay WRITE setSellingDelay)

public:

    RAlisjahbana2021FinancingTimeEstimator(QObject *parent, QString name);
    ~RAlisjahbana2021FinancingTimeEstimator();

    double getMedianDelayForInsurancePayments();
    void setMedianDelayForInsurancePayments(double value);

    double getMedianDelayForBankLoans();
    void setMedianDelayForBankLoans(double value);

    double getMedianDelayForSBALoans();
    void setMedianDelayForSBALoans(double value);

    double getMedianDelayForCDBGDRGrants();
    void setMedianDelayForCDBGDRGrants(double value);

    double getDispersionOfDelayForInsurancePayments();
    void setDispersionOfDelayForInsurancePayments(double value);

    double getDispersionOfDelayForBankLoans();
    void setDispersionOfDelayForBankLoans(double value);

    double getDispersionOfDelayForSBALoans();
    void setDispersionOfDelayForSBALoans(double value);

    double getDispersionOfDelayForCDBGDRGrants();
    void setDispersionOfDelayForCDBGDRGrants(double value);

    double getSBACap();
    void setSBACap(double value);

    double getCDBGDRCap();
    void setCDBGDRCap(double value);

    double getAvailableSavings();
    void setAvailableSavings(double value);

    QString getSellingOption();
    void setSellingOption(QString value);

    double getSellingDelay();
    void setSellingDelay(double value);

    double evaluateFinancingTime(QString theTenure, bool theMortgageStatus, QString theIncome, double theIncomeValue, double theLoss, double theBuildingValue);

protected:

private:

    // Random variables that define payment times
    double theMedianDelayForInsurancePayment;
    double theDispersionOfDelayForInsurancePayments;
    double theMedianDelayForBankLoans;
    double theDispersionOfDelayForBankLoans;
    double theMedianDelayForSBALoans;
    double theDispersionOfDelayForSBALoans;
    double theMedianDelayForCDBGDRGrants;
    double theDispersionOfDelayForCDBGDRGrants;
    double theAvailableSavings;
    double theSBACap;
    double theCDBGDRCap;
    QString theSellingOption;
    double theSellingDelay;

    // Amounts paid from each source
    double theBuildingExpectedFunds;
    double theBuildingAmountFromInsurance;
    double theBuildingAmountLendedFromBank;
    double theBuildingAmountLendedFromSBA;
    double theBuildingAmountGrantedByCDBGDR;
    double theBuildingAmountMissing;

};

#endif
