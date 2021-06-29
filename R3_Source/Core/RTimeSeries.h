
#ifndef RTimeSeries_H
#define RTimeSeries_H

#include "RParameter.h"
#include <memory>

class RMainWindow;
class RDomain;

class RTimeSeries : public RParameter
{
	Q_OBJECT	

public:
    RTimeSeries(QObject *parent, QString name);
	virtual ~RTimeSeries();
	
	QStringList getActionList();

    int setXvalues(double time, double x);
    int setYvalues(double time, double y);
    int setZvalues(double time, double z);
    int setXYZvalues(double time, double x, double y, double z);

    //Overload to accept parameters
    int setXYZvalues(double time, RParameter* x, RParameter* y, RParameter* z);

    virtual double getXvalue(double time, RParameter* theDDMParameter1 = nullptr, RParameter* theDDMParameter2 = nullptr);
    virtual double getYvalue(double time, RParameter* theDDMParameter1 = nullptr, RParameter* theDDMParameter2 = nullptr);
    virtual double getZvalue(double time, RParameter* theDDMParameter1 = nullptr, RParameter* theDDMParameter2 = nullptr);

    std::vector<double> getXYZvalues(double time, RParameter* theDDMParameter1  = nullptr, RParameter* theDDMParameter2 = nullptr);


    double getTimeItem(int item);
    double getXitem(int item);
    double getYitem(int item);
    double getZitem(int item);

    void clean();
    int getNumEntries();

public slots:

    void print();


public slots:

    void loadFile();

    void plot3D();
    void plotX();
    void plotY();
    void plotZ();

    void plotInt3D();
    void plotIntX();
    void plotIntY();
    void plotIntZ();

    void plotDblInt3D();
    void plotDblIntX();
    void plotDblIntY();
    void plotDblIntZ();

private:
	
    std::unique_ptr<std::vector<double>> theTimeValues = nullptr;
    std::unique_ptr<std::vector<double>> theXvalues    = nullptr;
    std::unique_ptr<std::vector<double>> theYvalues    = nullptr;
    std::unique_ptr<std::vector<double>> theZvalues    = nullptr;
    std::unique_ptr<std::vector<double>> theRotXvalues = nullptr;
    std::unique_ptr<std::vector<double>> theRotYvalues = nullptr;
    std::unique_ptr<std::vector<double>> theRotZvalues = nullptr;

    std::unique_ptr<std::vector<RParameter*>> theXParameterValues    = nullptr;
    std::unique_ptr<std::vector<RParameter*>> theYParameterValues    = nullptr;
    std::unique_ptr<std::vector<RParameter*>> theZParameterValues    = nullptr;

    void checkConsistency();

    bool consistencyChecked = false;

    int lastIt;
    double tol;

};

#endif
