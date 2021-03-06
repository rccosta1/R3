#ifndef RPropertyWidget_H
#define RPropertyWidget_H

#include <QWidget>

#include "ui_RPropertyWidget.h"

class QCompleter;
class QLineEdit;
class RDomain;

class RPropertyWidget : public QWidget, public Ui::RPropertyWidgetUi
{
	Q_OBJECT

public:
	RPropertyWidget(QWidget *parent = 0);
	~RPropertyWidget();

	// Updates the properties
	void updateView();

	// Sets the Domain object
	void setDomain(RDomain *passedDomain);

	// Removes all rows
	void removeAll();

	// Converts the name of a property to a text that is shown as the property name
	QString propertyToName(QString name);

	// Converts the text that is shown as the property name to thw actual name of a property
	QString nameToProperty(QString name);

public slots:
	// Displays the properties of the object with the passed name
	void viewProperties(QString &passedName);

private slots:
	// Assings the property value that is set by the user
	void assignProperties(int row, int column);

private:
	// Auxiliary method that removes all item in the property pane
	void deleteItems();

	// Auxiliary method that is invoked when the widget is resize
	void resizeEvent(QResizeEvent *event);

	// Auxiliary method that inserts a new row of property
	void insertRow(QString propertyName, QString propertyValue, QVariant::Type type, bool isWritable);

	// Data member that stores the object for which the properties are being shown
	QObject *currentObject;

	// Data member that indicates whether the request to show object properties has come from the domain tree or analysis tree
	QObject *senderObject;

	// Pointer to the Domain object
	RDomain *theDomain;

	// A list of strings that are the potential values that the user can choose as a property value, such as a list of all parameters
	QStringList completerStringList;

	// Qt object that shows the list of potential values that the user can choose as a property value
	QCompleter *completer;

	// Flag that indicates if the object for which the properties should be shown has changed
	bool currentObjectChanged;

};

#endif // RPropertyWidget_H
