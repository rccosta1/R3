#ifndef RObject_H
#define RObject_H

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QtDebug>
#include <QPointer>
#include <QRandomGenerator>

#ifdef NETWORKS
class RPacket;

#endif

const double PI = 3.141592653589793238462643;


class RModel;
class RLocation;
class RTimeSeries;
class RParameter;
class RDomain;

class RObject : public QObject
{
    Q_OBJECT

    Q_ENUMS(ROutputLevelType)
    Q_ENUMS(RParameterType)
    Q_ENUMS(RPointType)
    Q_ENUMS(RGradientType)

public:
    RObject(QObject *parent = 0, QString name = QString("RObject"));
	~RObject();

    enum ROutputLevelType{Minimum, Medium, Maximum};
    enum RParameterType{RandomVariable, DecisionVariable, Constant, Response, ALL}; //sg add, make sure to add all
    enum RPointType{Origin, CurrentValues};
    enum RGradientType{NONE,FDM, F2DM, F3DM , DDM, D2DM, D3DM};

	// setter/getter for the output display levels
    ROutputLevelType getOutputLevel();
    void setOutputLevel(ROutputLevelType value);

	// Indicates if the object is auto-generated
	bool isAutoGenerated();
	void setAutoGenerated(bool autoGenerated);

	// Sets the object name to the passed name
    virtual void setObjectName(const QString &name);

	// Checks if all the pointer meta-properties of the object are given
	int checkProperties();

	// Method that is invoked when a dynamic property is set to some value; the specific implementation of the method in sub-calsses should contain the operations that need to be carried out when the dynamic property is set; The name of the dynamic property must be added to the list in the method RDomain::getDynamicPropertyList()
	virtual void setDynamicProperty(QString propertyName, QVariant propertyValue);

	// Returns the list of items to be shown in the "right-click" menu of this object in the properties pane
	// For each item on the right-click menu, a "private slot" method with a similar name should be implemenetd in the sub-class with the follwoing rules:
	// 1. The name of the method should start with a lower-case letter
	// 2. The first letter of each word in the name of the method should be in upper-case
	// E.g., the right-clic item "Plot&PDF" requires implementing the method "void plotPDF()"
	virtual QStringList getActionList();

protected:

    RDomain* theDomain;

	// Sets the double/parameter data members; If the passed valueStr is a number, the double data member is assigned, otherwise, the parameter data member is assigned
	void setStringParameter(QString valueStr, double *value, QPointer<RParameter> *valueParameter);

	// Returns a string that contains the double/parameter name; If the parameter data memebr is nu NULL, the name of the parameter is returned, otherwise, the double value is converted to a string and returned
	QString getStringParameter(double *value, RParameter *valueParameter);

	// Returns the actual value of the double/parameter data members; If the parameter data memebr is nu NULL, the current value of the parameter is returned, otherwise, the double value is returned
	double getDoubleParameter(double *value, RParameter *valueParameter);

	// Creats a string from a the object names of a list of parameters
	QString parameterListToString(QList<QPointer<RParameter> > passedList);


#ifdef NETWORKS
    QString packetListToString(QList<QPointer<RPacket> > passedList);
    QList<QPointer<RPacket> > stringToPacketList(QString passedString);
    RPacket *parameterToPacket(RParameter *parameter);
    QList<RPacket *> parameterListToPacketList(QList<QPointer<RParameter> > parameterList);
#endif

    // Here is Terje's attempt at streamlining the "gradient type / gradient method" business (all should be theDifferentiationMethod)
    QString translateGradientTypeToString();
    void setGradientTypeInRObject(QString value);
    RGradientType theGradientType;

    // Creates a list of parameters from the passed string that contains their object names
	QList<QPointer<RParameter> > stringToParameterList(QString passedString);

    // Creates a string from a the object names of a list of models
	QString modelListToString(QList<QPointer<RModel> > passedList);

    // Creates a list of models from the passed string that contains their object names
	QList<QPointer<RModel> > stringToModelList(QString passedString);

    // Converts a pramater pointer to a Locations pointer
    RLocation *parameterToLocation(RParameter *parameter);

    // Converts a list of pramater pointers to a list of Locations pointers
    QList<RLocation *> parameterListToLocationList(QList<QPointer<RParameter> > parameterList, bool checkForDuplicates = false);

	// Creats a string from a the passed vector of integers
	QString intVectorToString(QVector<int> passedVector);

	// Creats a vector of integers from the passed string
	QVector<int> stringToIntVector(QString passedString);

	// Creats a string from a the passed vector of doubles
	QString doubleVectorToString(QVector<double> passedVector);

	// Creats a vector of doubles from the passed string
	QVector<double> stringToDoubleVector(QString passedString);

	// Auxiliary method to handle events that should happen when a dynamic property value is set
	bool event(QEvent *e);

	// Data member that indicates if the object is auto-generated
	bool theAutoGenerated;

	// Data member that stores the level of output to be displayed by analysis tools
    ROutputLevelType theOutputLevel;

    // Random number generator
    QRandomGenerator theRNG;

protected slots:

    virtual void help();

private:




};

#endif // RObject_H