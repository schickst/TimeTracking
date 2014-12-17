
#include <QtGui>
#include <QApplication>
#include <QMessageBox>

#include "window.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("CubeWorks");
    QCoreApplication::setApplicationName("Gringo");

    QSettings settings;

    /*
     * Die Log Files müssen angelegt sein,
     * sonst gibt es bei der ausführung Fehlermeldungen
     */

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Gringo"),
                              QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    Window window;
    window.show();
    return app.exec();
}
