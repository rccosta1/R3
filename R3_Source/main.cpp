#include "RMainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QPointer>
#include <QPlainTextEdit>
#include <QSurfaceFormat>

QPointer<QPlainTextEdit> console;

// Custom message handler, which redirects standard output and standard error channels to the output pane
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (!console) {
        return;
    }
    QString msgStr;
    switch (type) {
    case QtDebugMsg:
        console->appendPlainText(msg);
        break;
    case QtWarningMsg:
        break;
    case QtCriticalMsg:
        msgStr = QString("<font color=%1>").arg("red") + QString(msg) + QString("</font>") + QString("<font color=%1>").arg("black") + QString("&nbsp;") + QString("</font>");
        console->appendHtml(msgStr);
        break;
    case QtFatalMsg:
        msgStr = QString("<font color=%1>").arg("green") + QString(msg) + QString("</font>") + QString("<font color=%1>").arg("black") + QString("&nbsp;") + QString("</font>");
        console->appendHtml(msgStr);
        abort();
    }
    console->ensureCursorVisible();
}



int main(int argc, char *argv[])
{
    QApplication theApplication(argc, argv);

    // Set the name and version of the application
    theApplication.setApplicationName("R3");
    theApplication.setApplicationVersion("1.0");
    //theApplication.setOrganizationName("UBC");
    //theApplication.setOrganizationDomain("http://www.inrisk.ubc.ca");

    // Declare the main window
    RMainWindow theMainWindow;

    // Install the custom message handler that is defined above
    console = theMainWindow.findChild<QPlainTextEdit *>();
    qInstallMessageHandler(myMessageOutput);
    theMainWindow.show();

    // Establish signal-slot connections
    theApplication.connect(&theApplication, SIGNAL(lastWindowClosed()), &theApplication, SLOT(quit()));

    // Open the input files that are passed in argv
    if (argc == 1) {
        theMainWindow.clearAllObjects(false); //Comment this out ***** To automatically load last file
    } else if (argc >= 2) {
        theMainWindow.clearAllObjects(false); //Comment this out ***** To automatically load last file
        for (int i = 1; i < argc; i++) {
            theMainWindow.openFile(argv[i]);
        }
    }

    // Run the R3 application
    return theApplication.exec();
}
