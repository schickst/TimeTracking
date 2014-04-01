
#include <QtGui>
#include <QApplication>
#include <QMessageBox>

#include "window.hpp"

int main(int argc, char *argv[])
{
    QSettings settings("gringo.ini", QSettings::IniFormat);
    settings.setValue("TaskFile", "C:\\build\\build-Gringo-Desktop_Qt_5_1_1_MinGW_32bit-Debug\\debug\\Tasks.txt");
    settings.setValue("LogFile", "C:\\build\\build-Gringo-Desktop_Qt_5_1_1_MinGW_32bit-Debug\\debug\\Log.txt");
    settings.setValue("TextEditor", "C:\\Program Files (x86)\\Notepad++\\notepad++.exe");

    QApplication app(argc, argv);
    app.setOrganizationName("CubeWorks UG (haftungsbeschränkt)");
    app.setOrganizationDomain("http://www.cubeworksug.de");
    app.setApplicationName("Gringo");

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
