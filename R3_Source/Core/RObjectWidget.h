#ifndef RObjectWidget_H
#define RObjectWidget_H

#include <QWidget>
#include <QMenu>
#include "ui_RObjectWidget.h"

class RDomain;

class RObjectWidget : public QWidget, public Ui::RObjectWidgetUi
{
	Q_OBJECT

public:
	RObjectWidget(QWidget *parent = 0);
	~RObjectWidget();

    // Sets the Domain object
    void setDomain(RDomain *passedDomain);

	// Expands all branches of the object tree
	void expandAll();

	// Collapses all branches of the object tree
	void collapseAll();

	// Expands only those branches of the object tree that have objects
	void expandPopulatedBranches();

	// Creates the object tree by reading the tree headers from RDomain
	void createHeader(QString name, QTreeWidgetItem *parent = 0, int parentingStatus = 0);

public slots:
	// Updates the objects tree
	void updateView();

	// Selects an item in the tree that has the passed name
	void selectItem(QString itemName);

private slots:
	// Instantiates an object upon user's click
	void addObject();

	// Removes all objects of the current branch upon user's click
	void removeAllObject();

	// Removes the selected objects upon user's click
	void removeObject();

	// Emits a signal that indicates that the user has changes the current selection in the objects tree
	void changeCurrent(QTreeWidgetItem *current, QTreeWidgetItem *previous);

	// Emits a signal that indicates that the user has clicked on the current selection in the objects tree
	void currentClicked(QTreeWidgetItem *item);

	// Shows the "right-click" menu
	void showPopup(const QPoint &position);

	// Runs the action that the user selects on the "right-click" menu
	void runAction();

    // Ranks random variables alphabetically
    void rankRandomVariablesByName();

	// Invokes the method in the location object to obtain the latitude and longitude of the location from its address
	void getCoordinatesfromAddress();

	// Displays all location objects on the map
	void viewAllLocationsOnMap();

	// Removes all location objects from the map
	void removeAllLocationsFromMap();

	// Displays all regional models on the map
	void viewAllSectorsOnMap();

signals:
	// Signal that is emitted when the current object selection changes
	void currentObjectChanged(QString &name);

private:
	// Removes the object tree items for all objects
	void deleteItems();

	// Returns the full path of the passed tree item as a string, e.g., "Parameter Response File"
	QString getHeaderFullName(QTreeWidgetItem *item);

	// Populates the passed tree item with the objects names of its type
	void populateBranch(QTreeWidgetItem *passedItem);

	// Creates a tree item given its parnt itsm (parent branch) and its name
	void createItem(QTreeWidgetItem *parent, QString text);

	// Auxiliary method that ranks random variables according to the passed importance measure type, which can be Alpha or Gamma
	void rankRandomVariablesByImportance(QString importanceMeasureType);

    // Pointer to the Domain object
    RDomain *theDomain;

	// Data member for the list of tree items that appear in the last level and include the objects
	QList<QTreeWidgetItem *> lastLevelList;

	// Data members for the "right-click" menu for creating objects
	QMenu createMenu;
	QAction *createAction;
	QAction *removeAllAction;

	// Data members for the "right-click" menu for removing objects and other type of object-specific actions
	QMenu objectMenu;
	QAction *removeAction;

	// Flag that indicates if the new user's selection is the same as previous selection
	bool sameItemClicked;

};

#endif
