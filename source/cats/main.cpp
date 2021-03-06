#include "gui/MainWindow.hpp"

#include "settings/Settings.hpp"

#include <QGst/Init>

#include <QtWidgets/QApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("EPFL-LSRO-Mobots");
    QApplication::setOrganizationDomain("mobots.epfl.ch");
    QApplication::setApplicationName("CATS2");

    QGst::init(nullptr, nullptr);
    QApplication app(argc, argv);

    // initialize the settings
    // TODO : consider getting this parameters from the command line,
    // or based on the arguments of the command line?
    bool needConfigFile = true;
    bool needMainCamera = true;
    bool needBelowCamera = false;
    bool needCalibration = true;

    if (Settings::get().init(argc, argv, needConfigFile, needCalibration, needMainCamera, needBelowCamera)) {
        MainWindow mainWindow;
        mainWindow.show();
        return app.exec();
    } else {
        qDebug() << "Couldn't setup the application, finished";
    }
}

