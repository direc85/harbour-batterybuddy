#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QTimer>

#include "battery.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/harbour-batterybuddy.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //   - SailfishApp::pathToMainQml() to get a QUrl to the main QML file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QGuiApplication* app = SailfishApp::application(argc, argv);
    QQuickView* view = SailfishApp::createView();

    Battery battery;
    QTimer updater;
    QObject::connect(&updater, SIGNAL(timeout()), &battery, SLOT(updateData()));
    updater.start(15000);

    view->engine()->addImportPath("/usr/share/harbour-carbudget/qml");
    view->rootContext()->setContextProperty("battery", &battery);
    view->setSource(SailfishApp::pathTo("qml/harbour-batterybuddy.qml"));
    view->showFullScreen();

    return app->exec();
}
