#include "RDomain.h"
#include "RMainWindow.h"
#include "RObject.h"
#include <QMessageBox>
#include <QMetaProperty>
#include <QString>
#include <QLinkedList>
#include <QDir>


// PARAMETERS
#include "RResponse.h"
#include "RTime.h"
#include "RLocation.h"
#include "RLocationResponse.h"
#include "RConstant.h"


// METHODS
#include "RInHouseLinearSolver.h"
#include "RInHouseProbabilityDistributions.h"
#include "RIndependenceProbabilityTransformation.h"
#include "RInHouseRandomNumberGenerator.h"

// MODELS
#include "RModel.h"
#include "RScenarioModel.h"

// OTHER
#include "RFlowchartWidget.h"


#ifdef HAZARDS
#include "RPGDIntensityModel.h"
#include "RFixedTimeOccurrenceModel.h"
#endif


#ifdef SIMPLEBUILDINGS
#include "RHazusBuildingModel.h"
#endif


#ifdef NETWORKS
#include "RNetworkModel.h"
#include "RNetworkSamplingModel.h"
#include "RNetworkManagerModel.h"

#include "RBuildingPortfolio.h"
#include "RHAZUSBuildingPortfolio.h"
#include "RHouseholdPortfolio.h"

#include "RRequest.h"
#include "RFinancingRequest.h"
#include "REngineeringAssessmentRequest.h"
#include "RPermitRequest.h"
#include "RSingleFamilyContractorRequest.h"
#include "RMultiFamilyContractorRequest.h"
#include "RContractorNegotiationRequest.h"

#include "RPacket.h"
#include "RPacketResponse.h"

#include "RPrivateMoneyLenderModel.h"
#include "RHardwareStoreModel.h"
#include "RInspectorModel.h"
#include "RPermitAssessorModel.h"
#include "RSingleFamilyContractorFirmModel.h"
#include "RMultiFamilyContractorFirmModel.h"
#include "RContractorNegotiationModel.h"
#include "RPublicMoneyLenderModel.h"
#include "RGrantProviderModel.h"
#include "RInsuranceCompanyModel.h"
#include "RConcretePlantModel.h"
#include "REngineeringFirmModel.h"

#include "RNeighborhoodModel.h"
#include "RResidentialMobilityNeighborhoodModel.h"
#include "RWorkforceStudyNeighborhoodModel.h"
#include "RMainNeighborhoodModel.h"

#include "RPopulationDisplacementEstimator.h"
#include "RAHSPopulationDisplacementEstimator.h"
#include "RHAZUSPopulationDisplacementEstimator.h"
#include "RDisasterPreparednessEstimator.h"
#include "RAHSPreparednessEstimator.h"
#include "RStatisticsCanadaPreparednessEstimator.h"
#include "RSutley2016MorbidityEstimator.h"
#include "RAlisjahbana2021FinancingTimeEstimator.h"
#endif

RTime* theCurrentTime = nullptr;

RDomain::RDomain(QObject *parent, QString name)
    : RObject(parent, name)
{
    theLastAddedObject = nullptr;
    isInputFileBeingOpened = false;

    registerHeadersAsClasses();

    //Create the global time object
    theCurrentTime = qobject_cast<RTime*>(factory.instantiate("RTime",this,"officialCurrentTime"));
}

RDomain::~RDomain()
{
}


QStringList RDomain::createObjectTreeBranches(QString passedClassName)
{
    QStringList result;
    QString className = passedClassName.remove(' ');

    if (className == QString("Objects").remove(' ')) {
        result << "ORCHESTRATING MODELS"
               << "BUILDING MODELS"
               << "NETWORK MODELS"
               //<< "RELIABILITY MODELS"
               << "PARAMETERS"
               << "METHODS";
    }

    else if (className == QString("ORCHESTRATING MODELS").remove(' ')) {
        result << "Scenarios"
               << "Sampling";
    }
    else if (className == QString("PARAMETERS").remove(' ')) {
        result << "Constants"
               << "Random Variables"
               << "Design Variables"
               << "Correlation"
               << "Locations"
               << "Generic Responses"
               << "File Responses"
               << "Command Responses"
               << "Location Responses"
               << "Time Series Responses"
               << "Time Instants";
    }



#ifdef SIMPLEBUILDINGS
    if (className == QString("BUILDING MODELS").remove(' ')) {
        result << "Simple BIMs"
               << "Simple Building Response Models"
               << "Simple Building Damage Models"
               << "Simple Building Loss Models"
               << "Simple Building Repair Needs Models";
    }

#endif

#ifdef NETWORKS
    if (className == QString("REPOSITORIES").remove(' ')) {
        result << "Census Information"
               << "Building Portfolio";
    }
    else if (className == QString("NETWORK MODELS").remove(' ')) {

        result  << "Sampling Models"
                << "Network Managers"
                << "Neighborhoods"
                << "Home Contractor Firms"
                << "Building Contractor Firms"
                << "Financing Time Estimators"
                << "Morbidity Estimators"
                << "Displacement Estimators"
                << "Power Sources"
                << "Power Substations"
                << "Water Pipelines"
                << "Water Lift Stations"
                << "Water Reservoirs"
                << "Water Storage Tanks"
                << "Water Treatment Facilities"
                << "Bridges"
                << "Roads"
                << "Materials Suppliers"
                << "Concrete Plants"
                << "Communities"
                << "Fuel Pipelines"
                << "Fuel Sources"
                << "Fuel Tank Farms"
                << "Refineries"
                << "Ports"
                << "Insurance Companies"
                << "Intensity Spectrum Plotter";
    }


    else if (className == QString("PARAMETERS").remove(' ')) {
        result << "Packets"
               << "Packet Responses";
    }

    else if (className == QString("METHODS").remove(' ')) {
        result << "Methods";
    }

    if (className == QString("Objects").remove(' ')) {
        result << "OTHER OBJECTS";

    }
#endif

#ifdef HAZARDS
    if (className == QString("MODELS").remove(' ')) {

        result << "Poisson Point Occurrences"
               << "Fixed Time Occurrences"
               << "Simple Snow Loads"
               << "Ramp Loads"
               << "Scaled Ground Motions"
               << "Sine Loads"
               << "Synthetic Ground Motions"
               << "GMPEs"
               << "Magnitudes"
               << "Circular Area Sources"
               << "Multipoint Line Sources"
               << "Polygonal Area Sources"
               << "Quadrilateral Area Sources"
               << "Rectangular Area Sources"
               << "Single Line Sources"
               << "Fixed Sources";
    }
#endif

    return result;
}



int RDomain::createObject(QString name, QString passedClassName, bool isAutoGenerated)
{
    int namingResult = 0;
    if (!isInputFileBeingOpened) {
        namingResult = nameIsValid(name);
    }
    //QString className = passedClassName.remove(' ');

    if(name == "officialCurrentTime")
    {
        if(theCurrentTime)
        {
            delete theCurrentTime;
            theCurrentTime = nullptr;
        }
        theCurrentTime = qobject_cast<RTime*>(factory.instantiate("RTime",this,name));
        theLastAddedObject = theCurrentTime;
        return 0;
    }

    QString className = passedClassName;
    if (namingResult == 0) {

        theLastAddedObject = factory.instantiate(className.toStdString(),this,name);

        if (theLastAddedObject) {

            RObject *rObject = qobject_cast<RObject *>(theLastAddedObject);
            rObject->setAutoGenerated(isAutoGenerated);

        } else {

            qCritical()<<"Could not find class "<< passedClassName <<" in RDomain; check to see if it was added to the registry";
            return -1;
        }

        return 0;
    }
    else if (namingResult == -1) {

        // The name already exists in the domain.
        QString newName = name + "_New";
        //QMessageBox::warning(0, tr("Warning"), QString("An object with the name \"%1\" already exists. Rts will use the name \"%2\" instead.").arg(name).arg(newName), QMessageBox::Ok | QMessageBox::Default);
        qCritical() << "Warning: An object with the name" << name << "already exists. Rts will use the name" << newName << "instead.";
        int result = createObject(newName, className);
        if (result < 0 ) {
            return -1;
        }
        return 1;
    }
    else if (namingResult < -1) {

        // The name contains illegal characters.
        return -1;
    }
    else return -1;
}


QObjectList RDomain::getObjectList(QString passedClassName)
{
    //Dont remove the spaces, makes it more readable when you add the key to the registry
    QString className = passedClassName;
    QObjectList result;

    QList<RObject*> factoryObjectList = QList<RObject*>::fromStdList(factory.getObjects.getObjectList(className.toStdString(),this));

    if(!factoryObjectList.empty())
        for (int i = 0; i < factoryObjectList.count(); i++) {
            result << factoryObjectList[i];
        }

    // CLASSES NOT APPEARING IN THE OBJECTS PANE, BUT THAT WE MAY WANT TO CREATE LISTS OF... THE PARENT CLASSES OTHERS ARE SUBCLASSED FROM
    if (className == QString("Parameter").remove(' ')) {
        QList<RParameter *> objectList = this->findChildren<RParameter *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Model").remove(' ')) {
        QList<RModel *> objectList = this->findChildren<RModel *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    else if (className == QString("Response").remove(' ')) {
        QList<RResponse *> objectList = this->findChildren<RResponse *>();
        for (int i = 0; i < objectList.count(); i++) {
            result << objectList[i];
        }
    }
    sort(&result);
    return result;
}


void RDomain::updateObjectTree()
{
    RMainWindow *mainWindow = qobject_cast<RMainWindow *>(this->parent());
    mainWindow->updateTree("PARAMETERS");
}

QStringList RDomain::getDynamicPropertyList()
{
    QStringList result;
    result << "PerturbationFactor" << "EfficientPerturbation" << "XPoints" << "PDFPoints";
    return result;
}

QStringList RDomain::getPropertyStringList(QString propertyName)
{
    // Here we tell the user which parameters are available when he/she needs to enter a parameter in the Properties Pane
    // What is returned here must match one of the items in ::getObjectList above

    QStringList parameterPropertyList;

    parameterPropertyList << "Mean" << "StandardDeviation" << "CoefficientOfVariation" << "InitialValue" << "UpperBound" << "LowerBound" << "Parameter1" << "Parameter2" << "Parameter3" << "Parameter4" << "IncrementalCost" << "Magnitude";

    if (parameterPropertyList.contains(propertyName)) {
        return getStringList(getObjectList("Parameter"));
    }
    else if (propertyName == QString("Model")) {
        return getStringList(getObjectList("Model"));
    }
    else if (propertyName == QString("TimeParameter")) {
        return getStringList(getObjectList("Times"));
    }
    else {
        return QStringList();
    }
}

QObject * RDomain::getLastAddedObject() const
{
    return theLastAddedObject;
}

QString RDomain::getProperty(const QObject *passedObject, QString propertyName) const
{
    QByteArray byteArrayName = propertyName.toLatin1();
    const char * charName = byteArrayName.data();
    int index = passedObject->metaObject()->indexOfProperty(charName);
    QVariant valueVariant;
    if (index >= 0) {
        QMetaProperty metaProperty = passedObject->metaObject()->property(index);
        QVariant::Type type = metaProperty.type();

        valueVariant = QVariant::fromValue(passedObject->property(charName));
        if (metaProperty.isEnumType()) {
            valueVariant = metaProperty.enumerator().valueToKey(valueVariant.toInt());
        }
        //const char * typeName = passedObject->metaObject()->property(i).typeName();

        if (type == QVariant::Invalid) {
            valueVariant = "";
        } else if (type == QMetaType::QObjectStar) {
            QObject *propertyObject = passedObject->property(charName).value<QObject *>();
            if (propertyObject) {
                valueVariant = propertyObject->objectName();
            } else {
                valueVariant = "";
            }
            //const char * propertyType = QVariant::fromValue(propertyObject).typeName();
        } else if (type == QVariant::Double) {
            if (!metaProperty.isWritable()) {
                bool ok;
                double tempDouble = valueVariant.toDouble(&ok);
                if (ok) {
                    QString tempString = QString("%1").arg(tempDouble, 0, 'g', 7);
                    valueVariant = tempString;
                }
            } else {
                bool ok;
                double tempDouble = valueVariant.toDouble(&ok);
                if (ok) {
                    QString tempString = QString("%1").arg(tempDouble, 0, 'g', 13);
                    valueVariant = tempString;
                }
            }
        }
    } else {
        valueVariant = passedObject->property(charName);

        if (valueVariant.type() == QVariant::Invalid) {
            return "No such property";
        }
    }

    QString result = valueVariant.toString();
    return result;
}

int RDomain::setProperty(QObject *passedObject, QString propertyName, QString propertyValue)
{
    QByteArray byteArrayName = propertyName.toLatin1();
    const char * charName = byteArrayName.data();
    int index = passedObject->metaObject()->indexOfProperty(charName);
    QVariant::Type type = passedObject->metaObject()->property(index).type();

    bool isStaticProperty = false;

    if (type == QMetaType::QObjectStar) {
        QObject *propertyObject = findChild<QObject *>(propertyValue);
        if (propertyObject) {
            isStaticProperty = passedObject->setProperty(charName, QVariant::fromValue(propertyObject));
        } else {
            if (!propertyValue.isEmpty()) {
                qCritical() << "Error in" << passedObject->objectName() << ": Could not find an object named" << propertyValue << "in the domain.";
                return 0;
            }
            QObject *nullPointer = 0;
            isStaticProperty = passedObject->setProperty(charName, qVariantFromValue(nullPointer));
        }
    } else {
        if (propertyName == QString("objectName")) {
            // Updating the object pane if the objectName has been changed.
            int result = renameObject(passedObject, propertyValue);
            if (result == 0) {
                // If the renaming was successful, then 1 is returned, so that the property widget sends a command to the tree widget to refresh the tree
                return 1;
            }
        } else {
            QMetaProperty metaProperty = passedObject->metaObject()->property(index);
            if (metaProperty.isEnumType()) {
                int enumValue = metaProperty.enumerator().keyToValue(propertyValue.toLatin1());
                if (enumValue < 0) {
                    qCritical() << "Error: Could not set the property" << propertyName << "in the object" << passedObject->objectName() << ". The value" << propertyValue << "is invalid.";
                }
            }
            isStaticProperty = passedObject->setProperty(charName, QVariant::fromValue(propertyValue));
        }
    }
    if (isStaticProperty) {
        return 0;
    } else {
        // Returns -1 so that the main window knows that a property with this name is not a static property
        return -1;
    }
}

int RDomain::nameIsValid(QString name, bool checkForDuplicate)
{
    QRegExp regExp("[A-Za-z0-9]+[A-Za-z0-9_{}\/\"-:]*");  //SG add - allow Uuid hash to be recognized as a domain name
    if (!regExp.exactMatch(name)) {
        QMessageBox::critical(0, tr("Error"), QString(tr("The object name \"%1\" is unacceptable. It should start with a letter and contain no spaces.")).arg(name), QMessageBox::Ok | QMessageBox::Default);
        return -2;
    }

    if (checkForDuplicate) {
        // ATTENTION: THE FOLLOWING LINE MAKES THE PROCESS OF OPENING LARGE INPUT FILES VERY LONG
        QObject *theObject = this->findChild<QObject *>(name);
        if (theObject) {
            return -1;
        }
    }

    return 0;
}

void RDomain::setInputFileBeingOpened(bool value)
{
    isInputFileBeingOpened = value;
}

int RDomain::deleteObject(QString name)
{
    QObject *object = this->findChild<QObject *>(name);
    if (object) {

        bool isParameter =qobject_cast<RParameter*>(object);

        disconnect(object, 0, 0, 0);
        delete object;
        object = nullptr;

        // Make sure object trees are updated (notice the name is no longer used)
        //Also notice how it will now check if it is a parameter first before updating the tree
        //updating the tree takes a lot of time, this will makre sure that does not happen for everything that gets deleted
        if(isParameter){
            RMainWindow *mainWindow = qobject_cast<RMainWindow *>(this->parent());
            if(mainWindow)
                mainWindow->updateTree("PARAMETERS");
        }

        return 0;
    } else {
        return -1;
    }
}

int RDomain::renameObject(QObject *passedObject, QString name)
{
    int result = nameIsValid(name);
    if (result == 0) {
        // The name is fine.
        QString oldName = passedObject->objectName();
        RObject *rObject = qobject_cast<RObject *>(passedObject);
        rObject->setObjectName(name);
        return 0;
    } else if (result == -1) {
        // The name already exists in the domain.
        QMessageBox::critical(0, tr("Error"), QString("Could not rename the object. An object with the name \"%1\" already exists.").arg(name), QMessageBox::Ok | QMessageBox::Default);
        return -1;
    } else if (result < -1) {
        // The name contains illegal characters.
        return -1;
    }
}

void RDomain::print(QVector< QVector<double> > *matrix, QString outputFile)
{
    int row = matrix->size();
    int column = matrix[0][0].size();



    QString startAndEndChar = "|";
    //QString startAndEndChar = QString(QChar(0x2502));  // Unicode number of the graphic sign

    QFile *file = 0;
    if (!outputFile.isNull()) {
        file = new QFile(outputFile);
        if (!file) {
            return;
        }
        if (file->exists()) {
            file->remove();
        }
        if (!file->open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical() << "Warning: Could not create the file" << outputFile;
            return;
        }
        startAndEndChar.clear();
    }

    for (int i = 0; i < row; i++) {
        QString line = "";
        line += startAndEndChar;
        for (int j = 0; j < column; j++) {
            if (file) {
                line += QString("%1").arg((*matrix)[i][j], 0, 'g', 12);
            } else {
                // Is this enough precession?
                line += QString("%1").arg((*matrix)[i][j], 12, 'g', -1);
            }
            line += " ";
        }
        line += startAndEndChar;
        if (file) {
            line += "\n";
            QByteArray byteArray;
            byteArray.append(line);
            file->write(byteArray);
        } else {
            qDebug() << qPrintable(line);
        }
    }

    if (file) {
        file->flush();
        file->close();
        delete file;
        file = 0;
    }
}

void RDomain::print(QVector<double> *vector, QString outputFile)
{
    // Get the length of the vector
    int theSize = vector->size();

    // Create a temporary matrix to store the vector, so we can use the other ::print method
    QVector< QVector<double> > *tempmatrix = new QVector< QVector<double> >(theSize, QVector<double>(1));

    // Copy the vector into that temporary matrix
    for (int i=0; i<theSize; i++) {
        (*tempmatrix)[i][0] = vector->at(i);
    }

    // Call the other ::print method
    print(tempmatrix, outputFile);

    // Delete the temporary matrix
    delete tempmatrix;
}

int RDomain::readMatrixFromFile(QVector< QVector<double> > *matrix, QString inputFile)
{
    if (!matrix) {
        qCritical() << "Error: The passed matrix pointer is null";
        return -1;
    }
    int rowCount = matrix->size();
    int colCount = matrix[0][0].size();


    // Read the file here
    QFile file(inputFile);
    if (!file.open(QFile::ReadOnly)) {
        // Error
        qCritical() << "Error in RDomain: Could not open the file" << inputFile;
        return -1;
    }

    // Read the file content into a QString
    QTextStream stream(&file);
    stream.seek(0);
    //QString fileContentString = in.readAll();

    // Tokenize the QString of file contents by "\n" and create a string list of input file lines
    QString verticalRegExpString = QString("[n\n\v\r%1%2]").arg(QChar(QChar::ParagraphSeparator)).arg(QChar(QChar::LineSeparator));
    QRegExp verticalRegExp = QRegExp(verticalRegExpString);
    //QStringList fileContentStringList = fileContentString.split(verticalRegExp, QString::SkipEmptyParts);

    QStringList fileContentStringList;
    QString line;
    QStringList lineList;
    stream.setAutoDetectUnicode(true);
    do {
        line = stream.readLine();
        if (!line.isEmpty()) {
            lineList = line.split(verticalRegExp, QString::SkipEmptyParts);
            fileContentStringList.append(lineList);
        }
        //QCoreApplication::processEvents();
    } while (!line.isNull());

    int fileRowCount = fileContentStringList.count();

    if (fileRowCount != rowCount) {
        qCritical() << "Error: The number of rows in the file" << inputFile << "does not match the number of rows of the passed matrix/vector";
        return -1;
    }

    QRegExp regExp = QRegExp("[ ,;\t]");
    int fileColCount = 0;

    for (int i = 0; i < fileContentStringList.count(); i++) {
        QStringList valueList = fileContentStringList[i].split(regExp, QString::SkipEmptyParts);

        if (i == 0) {
            fileColCount = valueList.count();
            if (fileColCount != colCount) {
                qCritical() << "Error: The number of columns in the file" << inputFile << "does not match the number of columns of the passed matrix/vector";
                return -1;
            }
        } else {
            if (fileColCount != valueList.count()) {
                qCritical() << "Error: The number of data at the line" << i + 1 << "is different than the line 1 in the file" << inputFile;
                return -1;
            }
        }

        for (int j = 0; j < valueList.count(); j++) {
            QString tempStr = valueList[j].trimmed();
            bool ok;
            double tempValue = tempStr.toDouble(&ok);
            if (!ok) {
                qCritical() << "Error: Could not read the value number" << j + 1 << "at the line" << i + 1 << "in the file" << inputFile;
                return -1;
            }

            (*matrix)[i][j] = tempValue;
        }
    }
    file.close();
    return 0;
}

int RDomain::readVectorFromFile(QVector<double> *vector, QString inputFile)
{
    if (!vector) {
        qCritical() << "Error: The passed vector pointer is null";
        return -1;
    }

    // Create a temporary matrix so that the "read matrix from file" method can be used, to avoid code duplication
    int a = vector->size();
    QVector< QVector<double> > *tempMatrix = new QVector< QVector<double> >(a, QVector<double>(1));
    int result = readMatrixFromFile(tempMatrix, inputFile);
    if (result < 0) {
        return -1;
    }

    // Now revert back to a vector again, by picking up the first column of the matrix
    for (int i=0; i<tempMatrix->size(); i++) {
        (*vector)[i] = (*tempMatrix)[i][0];
    }

    return 0;
}

QStringList RDomain::getStringList(QObjectList objectList)
{
    QStringList result;
    for (int i = 0; i < objectList.count(); i++) {
        result << objectList[i]->objectName();
    }
    return result;
}

int RDomain::sort(QObjectList *passedList)
{
    QObjectList searchList = *passedList;

    int beforeCount = searchList.count();
    QStringList nameList;
    QMap<QString, QObject *> map;
    for (int i = 0; i < searchList.count(); i++) {
        map[searchList[i]->objectName()] = searchList[i];
    }
    *passedList = map.values();
    int afterCount = map.count();
    if (afterCount < beforeCount) {
        // means a duplicated name has been removed
        return -1;
    } else {
        return 0;
    }

    /*bool duplicate = false;
    double count = passedList->count();
    bool swapped = true;
    while (swapped) {
    swapped = false;
    count--;
    for (int i = 0; i < count; i++) {
    QObject *first = passedList->value(i);
    QObject *second = passedList->value(i+1);
    QString name1 = first->objectName();
    QString name2 = second->objectName();
    if (name1 > name2) {
    passedList->replace(i, second);
    passedList->replace(i + 1, first);
    swapped = true;
    } else if (name1 == name2) {
    duplicate = true;
    }
    }
    }
    if (duplicate) {
    // means there is a duplicated name
    return -1;
    } else {
    return 0;
    }*/

    /*for (int i = 0; i < searchList.count(); i++) {
    nameList << searchList[i]->objectName();
    }
    qSort(nameList.begin(), nameList.end());
    passedList->clear();
    for (int i = 0; i < nameList.count(); i++) {
    for (int j = 0; j < searchList.count(); j++) {
    if (searchList[j]->objectName() == nameList[i]) {
    passedList->append(searchList[j]);
    searchList.removeAt(j);
    continue;
    }
    }
    }*/
}

int RDomain::sort(QList<RModel *> *passedList)
{
    QObjectList objectList;
    for (int i = 0; i < passedList->count(); i++) {
        QObject *qObject = passedList->value(i);
        objectList << qObject;
    }
    int result = sort(&objectList);
    passedList->clear();
    for (int i = 0; i < objectList.count(); i++) {
        RModel *object = qobject_cast<RModel *>(objectList[i]);
        passedList->append(object);
    }

    return result;
}

int RDomain::sort(QList<RParameter *> *passedList)
{
    QObjectList objectList;
    for (int i = 0; i < passedList->count(); i++) {
        QObject *qObject = passedList->value(i);
        objectList << qObject;
    }
    int result = sort(&objectList);
    passedList->clear();
    for (int i = 0; i < objectList.count(); i++) {
        RParameter *object = qobject_cast<RParameter *>(objectList[i]);
        passedList->append(object);
    }

    return result;
}


int RDomain::sort(QList<RResponse *> *passedList)
{
    QObjectList objectList;
    for (int i = 0; i < passedList->count(); i++) {
        QObject *qObject = passedList->value(i);
        objectList << qObject;
    }
    int result = sort(&objectList);
    passedList->clear();
    for (int i = 0; i < objectList.count(); i++) {
        RResponse *object = qobject_cast<RResponse *>(objectList[i]);
        passedList->append(object);
    }

    return result;
}

QString RDomain::getFileName(QString filePath)
{
    return QFileInfo(filePath).completeBaseName();
}

QString RDomain::getAbsoluteFolderPath(QString filePath)
{
    return QFileInfo(filePath).absolutePath();
}

QString RDomain::stringToFilePath(QString string)
{
    string.remove('"');
    return QFileInfo(string).filePath();
}

QString RDomain::getCurrentFilePath() const
{
    return theCurrentFilePath;
}

void RDomain::setCurrentFilePath(QString value)
{
    theCurrentFilePath = value;

    QString workingPath = theCurrentFilePath;
    bool workingPathIsSet = QDir::setCurrent(workingPath);
}

void RDomain::clearAnalyzedFlags()
{
    QList<RModel *> theModelList = findChildren<RModel *>();
    for (int i=0; i<theModelList.size(); i++) {
        theModelList[i]->isAnalyzed = false;
    }
}

int RDomain::checkAllProperties()
{
    // Notice that this only checks for input that is a QObject...

    QList<RObject *> theObjectList = findChildren<RObject *>();

    bool allGood = true;

    for (int i=0; i<theObjectList.size(); i++) {

        if (theObjectList.at(i)->checkProperties() < 0) {

            allGood = false;
        }
    }

    if (allGood) {
        return 0;
    }
    else {
        return -1;
    }
}



void RDomain::registerHeadersAsClasses()
{
    // *******************************************************************************
    // *******************************************************************************
    //                            ORCHESTRATING MODELS
    // *******************************************************************************
    // *******************************************************************************
    factory.registerClass<RScenarioModel>("RScenarioModel");
    factory.registerClass<RScenarioModel>("ORCHESTRATING MODELS Scenarios");

    // *******************************************************************************
    // *******************************************************************************
    //                                     PARAMETERS
    // *******************************************************************************
    // *******************************************************************************
    factory.registerClass<RConstant>("RConstant");
    factory.registerClass<RConstant>("PARAMETERS Constants");
    factory.registerClass<RLocation>("RLocation");
    factory.registerClass<RLocation>("PARAMETERS Map Locations");
    factory.registerClass<RTime>("RTime");
    factory.registerClass<RTime>("PARAMETERS Time Instants");
    factory.registerClass<RLocationResponse>("RLocationResponse");
    factory.registerClass<RLocationResponse>("PARAMETERS Location Responses");
    factory.registerClass<RLocationResponse>("RGenericResponse");
    factory.registerClass<RLocationResponse>("PARAMETERS Generic Responses");
    factory.registerClass<RContinuousRandomVariable>("RContinuousRandomVariable");
    factory.registerClass<RContinuousRandomVariable>("PARAMETERS Random Variables");
    factory.registerClass<RInHouseRandomNumberGenerator>("RInHouseRandomNumberGenerator");
    factory.registerClass<RInHouseRandomNumberGenerator>("METHODS Random Number Generators InHouse");

    // *******************************************************************************
    // *******************************************************************************
    //                                     METHODS
    // *******************************************************************************
    // *******************************************************************************
    factory.registerClass<RInHouseLinearSolver>("RInHouseLinearSolver");
    factory.registerClass<RInHouseLinearSolver>("METHODS Linear Solvers InHouse");
    factory.registerClass<RInHouseProbabilityDistributions>("RInHouseProbabilityDistributions");
    factory.registerClass<RInHouseProbabilityDistributions>("METHODS Probability Distributions InHouse");
    factory.registerClass<RIndependenceProbabilityTransformation>("RIndependenceProbabilityTransformation");
    factory.registerClass<RIndependenceProbabilityTransformation>("METHODS Probability Transformations Independence");

    // *******************************************************************************
    // *******************************************************************************
    //                              RELIABILITY MODELS
    // *******************************************************************************
    // *******************************************************************************

#ifdef HAZARDS
    factory.registerClass<RPGDIntensityModel>("RPGDIntensityModel");
    factory.registerClass<RPGDIntensityModel>("MODELS GMPEs");
    factory.registerClass<RFixedTimeOccurrenceModel>("RFixedTimeOccurrenceModel");
    factory.registerClass<RFixedTimeOccurrenceModel>("MODELS Occurrence Models");
#endif

#ifdef SIMPLEBUILDINGS
    // *******************************************************************************
    // *******************************************************************************
    //                            SIMPLE BUILDING MODELS
    // *******************************************************************************
    // *******************************************************************************
    factory.registerClass<RHazusBuildingModel>("RHazusBuildingModel");
    factory.registerClass<RHazusBuildingModel>("BUILDING MODELS Hazus Building Models");
#endif

#ifdef NETWORKS
    // *******************************************************************************
        // *******************************************************************************
        //                            REPOSITORIES
        // *******************************************************************************
        // *******************************************************************************
    factory.registerClass<RHAZUSBuildingPortfolio>("Information Repositories");
    factory.registerClass<RHAZUSBuildingPortfolio>("RHAZUSBuildingPortfolio");
    factory.registerClass<RHouseholdPortfolio>("Information Repositories");
    factory.registerClass<RHouseholdPortfolio>("RHouseholdPortfolio");

    // *******************************************************************************
    // *******************************************************************************
    //                                     PARAMETERS
    // *******************************************************************************
    // *******************************************************************************

    factory.registerClass<RPacket>("PARAMETERS Packets");
    factory.registerClass<RPacket>("RPacket");
    factory.registerClass<RPacketResponse>("PARAMETERS Packet Responses");
    factory.registerClass<RPacketResponse>("RPacketResponse");
    factory.registerClass<RRequest>("PARAMETERS Requests");
    factory.registerClass<RRequest>("RRequest");
    factory.registerClass<RFinancingRequest>("PARAMETERS Requests");
    factory.registerClass<RFinancingRequest>("RFinancingRequest");
    factory.registerClass<REngineeringAssessmentRequest>("PARAMETERS Requests");
    factory.registerClass<REngineeringAssessmentRequest>("REngineeringAssessmentRequest");
    factory.registerClass<RPermitRequest>("PARAMETERS Requests");
    factory.registerClass<RPermitRequest>("RPermitRequest");
    factory.registerClass<RSingleFamilyContractorRequest>("PARAMETERS Requests");
    factory.registerClass<RSingleFamilyContractorRequest>("RSingleFamilyContractorRequest");
    factory.registerClass<RMultiFamilyContractorRequest>("PARAMETERS Requests");
    factory.registerClass<RMultiFamilyContractorRequest>("RMultiFamilyContractorRequest");
    factory.registerClass<RContractorNegotiationRequest>("PARAMETERS Requests");
    factory.registerClass<RContractorNegotiationRequest>("RContractorNegotiationRequest");


    // *******************************************************************************
    // *******************************************************************************
    //                                     NETWORK MODELS
    // *******************************************************************************
    // *******************************************************************************
    // NEIGHBORHOODS
    factory.registerClass<RResidentialMobilityNeighborhoodModel>("NETWORK MODELS Neighborhoods");
    factory.registerClass<RResidentialMobilityNeighborhoodModel>("RResidentialMobilityNeighborhoodModel");
    factory.registerClass<RWorkforceStudyNeighborhoodModel>("NETWORK MODELS NeighborhoodsOther");
    factory.registerClass<RWorkforceStudyNeighborhoodModel>("RWorkforceStudyNeighborhoodModel");
    factory.registerClass<RMainNeighborhoodModel>("NETWORK MODELS NeighborhoodsOther");
    factory.registerClass<RMainNeighborhoodModel>("RMainNeighborhoodModel");

    // BUSINESSES
    factory.registerClass<RPrivateMoneyLenderModel>("NETWORK MODELS Private Money Lender");
    factory.registerClass<RPrivateMoneyLenderModel>("RPrivateMoneyLenderModel");
    factory.registerClass<RContractorNegotiationModel>("NETWORK MODELS Contractor Firms");
    factory.registerClass<RContractorNegotiationModel>("RContractorNegotiationModel");
    factory.registerClass<RSingleFamilyContractorFirmModel>("NETWORK MODELS Home Contractor Firms");
    factory.registerClass<RSingleFamilyContractorFirmModel>("RSingleFamilyContractorFirmModel");
    factory.registerClass<RMultiFamilyContractorFirmModel>("NETWORK MODELS Building Contractor Firms");
    factory.registerClass<RMultiFamilyContractorFirmModel>("RMultiFamilyContractorFirmModel");
    factory.registerClass<RHardwareStoreModel>("NETWORK MODELS Material Suppliers");
    factory.registerClass<RHardwareStoreModel>("RHardwareStoreModel");
    factory.registerClass<RPublicMoneyLenderModel>("NETWORK MODELS Public Money Lender");
    factory.registerClass<RPublicMoneyLenderModel>("RPublicMoneyLenderModel");
    factory.registerClass<RGrantProviderModel>("NETWORK MODELS Grant Provider");
    factory.registerClass<RGrantProviderModel>("RGrantProviderModel");
    factory.registerClass<RConcretePlantModel>("NETWORK MODELS Concrete Plants");
    factory.registerClass<RConcretePlantModel>("RConcretePlantModel");
    factory.registerClass<REngineeringFirmModel>("NETWORK MODELS Engineering Firms");
    factory.registerClass<REngineeringFirmModel>("REngineeringFirmModel");
    factory.registerClass<RInspectorModel>("NETWORK MODELS Inspectors");
    factory.registerClass<RInspectorModel>("RInspectorModel");
    factory.registerClass<RNetworkManagerModel>("NETWORK MODELS Network Managers");
    factory.registerClass<RNetworkManagerModel>("RNetworkManagerModel");
    factory.registerClass<RPermitAssessorModel>("NETWORK MODELS Permitting Authority");
    factory.registerClass<RPermitAssessorModel>("RPermitAssessorModel");
    factory.registerClass<RNetworkSamplingModel>("NETWORK MODELS Sampling Models");
    factory.registerClass<RNetworkSamplingModel>("RNetworkSamplingModel");
    factory.registerClass<RInsuranceCompanyModel>("NETWORK MODELS Insurance Companies");
    factory.registerClass<RInsuranceCompanyModel>("RInsuranceCompanyModel");

    // ESTIMATORS
    factory.registerClass<RAHSPopulationDisplacementEstimator>("NETWORK MODELS Displacement Estimators");
    factory.registerClass<RAHSPopulationDisplacementEstimator>("RAHSPopulationDisplacementEstimator");
    factory.registerClass<RAHSPreparednessEstimator>("NETWORK MODELS Preparedness Estimators");
    factory.registerClass<RAHSPreparednessEstimator>("RAHSPreparednessEstimator");
    factory.registerClass<RSutley2016MorbidityEstimator>("NETWORK MODELS Morbidity Estimators");
    factory.registerClass<RSutley2016MorbidityEstimator>("RSutley2016MorbidityEstimator");
    factory.registerClass<RAlisjahbana2021FinancingTimeEstimator>("NETWORK MODELS Financing Time Estimators");
    factory.registerClass<RAlisjahbana2021FinancingTimeEstimator>("RAlisjahbana2021FinancingTimeEstimator");
#endif


}


