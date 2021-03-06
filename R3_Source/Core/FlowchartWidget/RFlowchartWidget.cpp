#include "RFlowchartWidget.h"

#ifdef NETWORKS
#include "RNetworkManagerModel.h"
#include "RIntensityModel.h"
//#include "RPowerDamModel.h"
//#include "RPowerSubstationModel.h"
//#include "RWaterDamModel.h"
//#include "RWaterPumpStationModel.h"
//#include "RWaterPipelineModel.h"
//#include "RWaterStorageTankModel.h"
//#include "RWaterTreatmentFacilityModel.h"
#include "REngineeringFirmModel.h"
#include "RContractorNegotiationModel.h"
#include "RHardwareStoreModel.h"
#include "RConcretePlantModel.h"
#include "RInspectorModel.h"
#include "RInsuranceCompanyModel.h"
#include "RPrivateMoneyLenderModel.h"
#include "RPublicMoneyLenderModel.h"
#include "RPermitAssessorModel.h"
#include "RGenericMaterialsSupplierModel.h"
//#include "RRoadwayModel.h"
//#include "RBridgeModel.h"
//#include "RPortModel.h"
#include "RNeighborhoodModel.h"
#include "REarthquakeBaseClass.h"
#include "RPGDIntensityModel.h"
//#include "RBooreAtkinson2008IntensityModel.h"
#include "RFixedTimeOccurrenceModel.h"
//#include "RGivenMagnitudeModel.h"
#endif

#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QPrintDialog>
//#include <QSvgGenerator>
#include <math.h>


RFlowchartWidget::RFlowchartWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    setupUi(this);

    setContextMenuPolicy(Qt::CustomContextMenu);
    bool ok = connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showPopup(QPoint)));

    elbowFlag = false;
    theScene = 0;

    theScaleFactor = 0.9;

    createMenu();
}

RFlowchartWidget::~RFlowchartWidget()
{

}

void RFlowchartWidget::setObjectList(QList<QObjectList> objectListList, QMap<QPointer<QObject>, QObjectList> objectMap)
{
    theObjectListList = objectListList;
    theObjectMap = objectMap;
    draw();
}

void RFlowchartWidget::draw()
{
    if (theScene) {
        delete theScene;
        theScene = 0;
    }

    theScene = new QGraphicsScene(this);
    setScene(theScene);

    theScene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    QPen pen(QColor(74, 126, 187), 1);
    QBrush brush(QColor(186, 210, 255), Qt::SolidPattern);
    QBrush penBrush(QColor(74, 126, 187), Qt::SolidPattern);
    QFontInfo fontInformation = fontInfo();
    QFont font(fontInformation.family(), 10, QFont::Normal);

    QGraphicsTextItem *tempTextItem = theScene->addText("ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz", font);
    qreal textHeight = tempTextItem->boundingRect().height();
    theScene->removeItem(tempTextItem);
    delete tempTextItem;

    qreal marginH = 50;
    qreal marginV = marginH;
    qreal textMargin = textHeight * 0.1;
    qreal rectHeight = textHeight + 2 * textMargin;
    qreal spacingH = 50;
    qreal spacingV = rectHeight * 0.5;

    qreal left = marginH;
    qreal maxWidth = 0;

    int maxItemsCount = 0;
    for (int i = 0; i < theObjectListList.count(); i++) {
        maxItemsCount = qMax(maxItemsCount, theObjectListList[i].count());
    }
    qreal maxHeight = maxItemsCount * rectHeight + (maxItemsCount - 1) * spacingV;

    QMap<QObject *, QGraphicsRectItem *> rectMap;
    for (int i = 0; i < theObjectListList.count(); i++) {
        QObjectList leveliObjectList = theObjectListList[i];

        if (i > 0) {
            left += maxWidth + spacingH;
        }
        maxWidth = 0;

        qreal currentHeight = leveliObjectList.count() * rectHeight + (leveliObjectList.count() - 1) * spacingV;
        qreal top = marginV + (maxHeight - currentHeight) / 2;

        QList<QGraphicsTextItem *> textItemList;
        for (int j = 0; j < leveliObjectList.count(); j++) {
            QObject *object = leveliObjectList[j];

            int x1 = left;
            int y1 = top + j * rectHeight + (j - 1) * spacingV;

            QGraphicsTextItem *textItem = theScene->addText(object->objectName(), font);
            textItemList << textItem;
            textItem->setPos(x1 + textMargin, y1 + textMargin);
            textItem->setZValue(1);

            maxWidth = qMax(maxWidth, textItem->boundingRect().width());
        }

        maxWidth += 2 * textMargin;
        for (int j = 0; j < leveliObjectList.count(); j++) {

            int x1 = left;
            int y1 = top + j * rectHeight + (j - 1) * spacingV;

            QBrush brushLast = brush;
            QPen penLast = pen;

            if (i == theObjectListList.count() - 1) {
                brushLast = QBrush(QColor(Qt::white), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }


#ifdef NETWORKS
//            if (RPowerDamModel *it = qobject_cast<RPowerDamModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(255,165,0), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }
//            else if (RPowerSubstationModel *it = qobject_cast<RPowerSubstationModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(255,165,0), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }


//            else if (RWaterDamModel *it = qobject_cast<RWaterDamModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(Qt::blue), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }
//            else if (RWaterPumpStationModel *it = qobject_cast<RWaterPumpStationModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(Qt::blue), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }
//            else if (RWaterPipelineModel *it = qobject_cast<RWaterPipelineModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(Qt::blue), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }
//            else if (RWaterStorageTankModel *it = qobject_cast<RWaterStorageTankModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(Qt::blue), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }
//            else if (RWaterTreatmentFacilityModel *it = qobject_cast<RWaterTreatmentFacilityModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(Qt::blue), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }


            if (REngineeringFirmModel *it = qobject_cast<REngineeringFirmModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RContractorNegotiationModel *it = qobject_cast<RContractorNegotiationModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RHardwareStoreModel *it = qobject_cast<RHardwareStoreModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RConcretePlantModel *it = qobject_cast<RConcretePlantModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RInspectorModel *it = qobject_cast<RInspectorModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RInsuranceCompanyModel *it = qobject_cast<RInsuranceCompanyModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RPrivateMoneyLenderModel *it = qobject_cast<RPrivateMoneyLenderModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RPublicMoneyLenderModel *it = qobject_cast<RPublicMoneyLenderModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RPermitAssessorModel *it = qobject_cast<RPermitAssessorModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RGenericMaterialsSupplierModel *it = qobject_cast<RGenericMaterialsSupplierModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(Qt::green), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }




//            else if (RRoadwayModel *it = qobject_cast<RRoadwayModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(105, 105, 105), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }
//            else if (RBridgeModel *it = qobject_cast<RBridgeModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(105, 105, 105), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }
//            else if (RPortModel *it = qobject_cast<RPortModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(105, 105, 105), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }


            else if (REarthquakeBaseClass *it = qobject_cast<REarthquakeBaseClass*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(255, 0, 0), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
            else if (RPGDIntensityModel *it = qobject_cast<RPGDIntensityModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(255, 0, 0), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
//            else if (RBooreAtkinson2008IntensityModel *it = qobject_cast<RBooreAtkinson2008IntensityModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(255, 0, 0), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }
            else if (RFixedTimeOccurrenceModel *it = qobject_cast<RFixedTimeOccurrenceModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(255, 0, 0), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
//            else if (RGivenMagnitudeModel *it = qobject_cast<RGivenMagnitudeModel*>(leveliObjectList[j])) {
//                brushLast = QBrush(QColor(255, 0, 0), Qt::SolidPattern);
//                penLast = QPen(QColor(0, 0, 0));
//            }


            else if (RNeighborhoodModel *it = qobject_cast<RNeighborhoodModel*>(leveliObjectList[j])) {
                brushLast = QBrush(QColor(255,192,203), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }


            else {
                brushLast = QBrush(QColor(Qt::white), Qt::SolidPattern);
                penLast = QPen(QColor(0, 0, 0));
            }
#endif

            QGraphicsRectItem *rectItem = theScene->addRect(x1, y1, maxWidth, rectHeight, penLast, brushLast);
            rectMap[leveliObjectList[j]] = rectItem;

            QGraphicsTextItem *textItem = textItemList[j];
            textItem->setPos(x1 + (maxWidth - textItem->boundingRect().width()) / 2, textItem->y());
        }
    }

    // Draw the arrows
    qreal polygonHeight = 8;
    QList<QPointer<QObject> > arrowStartList = theObjectMap.keys();
    for (int i = 0; i < arrowStartList.count(); i++) {
        QObjectList arrowEndList = theObjectMap[arrowStartList[i]];
        QGraphicsRectItem *startRectItem = rectMap[arrowStartList[i]];
        QRectF startRect = startRectItem->boundingRect();
        qreal x1 = startRect.x() + startRect.width();
        qreal y1 = startRect.y() + startRect.height() / 2;


        int theCheck = 0;

#ifdef NETWORKS
        RIntensityModel *isAIntensityModel = qobject_cast<RIntensityModel *>(arrowStartList[i]);
        if (isAIntensityModel){
            theCheck = 1;
        }
#endif

        if (theCheck == 0){
            for (int j = 0; j < arrowEndList.count(); j++) {
                QGraphicsRectItem *endRectItem = rectMap[arrowEndList[j]];
                QRectF endRect = endRectItem->boundingRect();
                qreal x2 = endRect.x();
                qreal y2 = endRect.y() + endRect.height() / 2;

                if (elbowFlag) {
                    QGraphicsLineItem *lineItem1 = theScene->addLine(x1, y1, x1 + spacingH / 2, y1, pen);
                    QGraphicsLineItem *lineItem2 = theScene->addLine(x1 + spacingH / 2, y1, x1 + spacingH / 2, y2, pen);
                    QGraphicsLineItem *lineItem3 = theScene->addLine(x1 + spacingH / 2, y2, x2, y2, pen);
                    QPolygonF polygon;
                    polygon << QPointF(x2, y2) << QPointF(x2 - polygonHeight, y2 - polygonHeight / 2) << QPointF(x2 - polygonHeight, y2 + polygonHeight / 2);
                    QGraphicsPolygonItem * polygonItem = theScene->addPolygon(polygon, pen, penBrush);
                    lineItem1->setZValue(2);
                    lineItem2->setZValue(2);
                    lineItem3->setZValue(2);
                    polygonItem->setZValue(2);
                } else {
                    QGraphicsLineItem *lineItem = theScene->addLine(x1, y1, x2, y2, pen);
                    QPolygonF polygon;
                    polygon << QPointF(x2, y2) << QPointF(x2 - polygonHeight, y2 - polygonHeight / 2) << QPointF(x2 - polygonHeight, y2 + polygonHeight / 2);
                    QGraphicsPolygonItem * polygonItem = theScene->addPolygon(polygon, pen, penBrush);
                    qreal angle = 180 / 3.14159265359 * atan2(y2 - y1, x2 - x1);
                    polygonItem->setTransform(QTransform().translate(x2, y2).rotate(angle).translate(-x2, -y2));
                    lineItem->setZValue(2);
                    polygonItem->setZValue(2);
                }
            }
        }
    }

    theScene->setSceneRect(QRectF());
    //QRectF rect = theScene->sceneRect();
    //rect.setWidth(rect.width() + marginH);
    //rect.setHeight(rect.height() + marginV);
    //theScene->setSceneRect(rect);

    setRenderHint(QPainter::Antialiasing);
    //setBackgroundBrush(QPixmap(":/images/cheese.jpg"));
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setDragMode(QGraphicsView::ScrollHandDrag);
}

//void RFlowchartWidget::mousePressEvent(QMouseEvent *event)
//{
//    lastPos = event->pos();
//}

//void RFlowchartWidget::mouseMoveEvent(QMouseEvent *event)
//{
//    int dx = event->x() - lastPos.x();
//    int dy = event->y() - lastPos.y();
//
//	if (event->buttons() & Qt::LeftButton) {
//		// Nothing
//		//QWidget::mouseMoveEvent(event);
//		this->translate(dx, dy);
//    } else if (event->buttons() & Qt::RightButton) {
//        // Nothing
//		QWidget::mouseMoveEvent(event);
//	} else if (event->buttons() & Qt::MidButton) {
//		// scale
//		qreal scale;
//		qreal scaleFactor = 0.9;
//		dy > 0 ? scale = 1.0 / scaleFactor : scale = scaleFactor;
//		this->scale(scale, scale);
//    }
//    lastPos = event->pos();
//}

void RFlowchartWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    //reset
    resetZoom();
}

void RFlowchartWidget::wheelEvent(QWheelEvent *event)
{
    qreal scale;
    qreal scaleFactor = theScaleFactor;
    event->delta() > 0 ? scale = 1.0 / scaleFactor : scale = scaleFactor;
    this->scale(scale, scale);
}

void RFlowchartWidget::saveDiagramToVectorFile()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF/PS File"), QDir::currentPath(), "PDF Files (*.pdf);;PostScript Files (*.ps)", &selectedFilter);
    if (fileName.isEmpty()) {
        return;
    }
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(fileName);

    if (selectedFilter.contains("pdf")) {
        printer.setOutputFormat(QPrinter::PdfFormat);
    } else if (selectedFilter.contains("ps")) {
        //printer.setOutputFormat(QPrinter::PostScriptFormat);
    }

    //printer.setPaperSize(QPrinter::A4);
    QPainter painter(&printer);

    // Print, fitting the viewport contents into a full page
    render(&painter);

    QRect viewport = this->viewport()->rect();
    render(&painter, QRectF(0, 0, printer.width(), printer.height()), viewport);
}

void RFlowchartWidget::saveDiagramToPixelFile()
{
    QRect port = viewport()->rect();
    int width = port.width();
    int height = port.height();

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"), QDir::currentPath(), "PNG Files (*.png);;JPG Files (*.jpg);;BMP Files (*.bmp);;PPM Files (*.PPM);;XBM Files (*.xbm);;XPM Files (*.xpm)", &selectedFilter);
    if (fileName.isEmpty()) {
        return;
    }

    QImage image(width, height, QImage::Format_ARGB32);
    if (selectedFilter.contains("png")) {
        image.fill(Qt::transparent);
    } else {
        image.fill(QColor(Qt::white).rgb());
    }

    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);

    QPainter::RenderHints hints = renderHints();
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    render(&painter, port, port);
    setRenderHints(hints);

    QString extension = QFileInfo(fileName).suffix();
    image.save(fileName, extension.toLatin1(), 100);
}

void RFlowchartWidget::printDiagram()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dialog = new QPrintDialog(&printer, this);
    dialog->setWindowTitle(tr("Diagram"));
    if (dialog->exec() != QDialog::Accepted) {
        return;
    }
    printer.setOutputFormat(QPrinter::NativeFormat);
    //printer.setPaperSize(QPrinter::A4);
    QPainter painter(&printer);

    // Print, fitting the viewport contents into a full page
    render(&painter);

    QRect viewport = this->viewport()->rect();
    render(&painter, QRectF(0, 0, printer.width(), printer.height()), viewport);
}

void RFlowchartWidget::setElbow(bool checked)
{
    elbowFlag = checked;
    QList<QPointer<QObject> > objectList = theObjectMap.keys();
    for (int i = 0; i < objectList.count(); i++) {
        if (!objectList[i]) {
            return;
        }
    }
    draw();
}

void RFlowchartWidget::zoomIn()
{
    qreal scaleFactor = 1.0 / theScaleFactor;
    this->scale(scaleFactor, scaleFactor);
}

void RFlowchartWidget::zoomOut()
{
    qreal scaleFactor = theScaleFactor;
    this->scale(scaleFactor, scaleFactor);
}

void RFlowchartWidget::resetZoom()
{
    //reset
    resetTransform();
    resetMatrix();
}

void RFlowchartWidget::showPopup(const QPoint &position)
{
    menu->exec(mapToGlobal(position));
}

void RFlowchartWidget::createMenu()
{
    menu = new QMenu(this);

    zoomInAction = new QAction(tr("Zoom In"), this);
    zoomInAction->setStatusTip(tr("Increases the magnification of the diagram."));
    zoomInAction->setIcon(QIcon(":/RImages/zoomin.png"));
    bool ok = connect(zoomInAction, SIGNAL(triggered()), this, SLOT(zoomIn()));
    menu->addAction(zoomInAction);

    zoomOutAction = new QAction(tr("Zoom Out"), this);
    zoomOutAction->setStatusTip(tr("Decreases the magnification of the diagram."));
    zoomOutAction->setIcon(QIcon(":/RImages/zoomout.png"));
    ok = connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(zoomOut()));
    menu->addAction(zoomOutAction);

    resetZoomAction = new QAction(tr("Reset Zoom"), this);
    resetZoomAction->setStatusTip(tr("Resets the magnification of the diagram."));
    resetZoomAction->setIcon(QIcon(":/RImages/resetzoom.png"));
    ok = connect(resetZoomAction, SIGNAL(triggered()), this, SLOT(resetZoom()));
    menu->addAction(resetZoomAction);

    menu->addSeparator();

    elbowAction = new QAction(tr("&Elbow Arrows"), this);
    elbowAction->setStatusTip(tr("Views arrows as elbows"));
    //elbowAction->setIcon(QIcon(":/RImages/pdf.png"));
    elbowAction->setChecked(elbowFlag);
    elbowAction->setCheckable(true);
    ok = connect(elbowAction, SIGNAL(toggled(bool)), this, SLOT(setElbow(bool)));
    menu->addAction(elbowAction);

    menu->addSeparator();

    saveDiagramVectorAction = new QAction(tr("Save Diagram As &PDF/PS"), this);
    saveDiagramVectorAction->setStatusTip(tr("Saves the diagram in a PDF or PS file."));
    saveDiagramVectorAction->setIcon(QIcon(":/RImages/pdf.png"));
    ok = connect(saveDiagramVectorAction, SIGNAL(triggered()), this, SLOT(saveDiagramToVectorFile()));
    menu->addAction(saveDiagramVectorAction);

    saveDiagramSvgAction = new QAction(tr("Save Diagram As &SVG"), this);
    saveDiagramSvgAction->setStatusTip(tr("Saves the diagram in a SVG file."));
    saveDiagramSvgAction->setIcon(QIcon(":/RImages/svg.png"));
    ok = connect(saveDiagramSvgAction, SIGNAL(triggered()), this, SLOT(saveDiagramToSvgFile()));
    menu->addAction(saveDiagramSvgAction);

    saveDiagramPixelAction = new QAction(tr("Save Diagram As &Image"), this);
    saveDiagramPixelAction->setStatusTip(tr("Saves the diagram in an image file."));
    saveDiagramPixelAction->setIcon(QIcon(":/RImages/image.png"));
    ok = connect(saveDiagramPixelAction, SIGNAL(triggered()), this, SLOT(saveDiagramToPixelFile()));
    menu->addAction(saveDiagramPixelAction);

    menu->addSeparator();

    printDiagramAction = new QAction(tr("&Print Diagram"), this);
    printDiagramAction->setStatusTip(tr("Prints the diagram."));
    printDiagramAction->setIcon(QIcon(":/RImages/print.png"));
    ok = connect(printDiagramAction, SIGNAL(triggered()), this, SLOT(printDiagram()));
    menu->addAction(printDiagramAction);
}
