
#include <QtGui>
#include <QApplication>
#include <QMessageBox>

#include "window.hpp"

int main(int argc, char *argv[])
{
    QSettings settings("gringo.ini", QSettings::IniFormat);

    /*
     * Die Log Files müssen angelegt sein,
     * sonst gibt es bei der ausführung Fehlermeldungen
     */
#ifdef Q_OS_LINUX
	settings.setValue("TaskFile", "/home/rails/.gringo/Tasks.txt");
	settings.setValue("LogFile", "/home/rails/.gringo/Log.txt");
	settings.setValue("TextEditor", "geany");
#else
    settings.setValue("TaskFile", "C:\\build\\build-Gringo-Desktop_Qt_5_1_1_MinGW_32bit-Debug\\debug\\Tasks.txt");
    settings.setValue("LogFile", "C:\\build\\build-Gringo-Desktop_Qt_5_1_1_MinGW_32bit-Debug\\debug\\Log.txt");
    settings.setValue("TextEditor", "C:\\Program Files (x86)\\Notepad++\\notepad++.exe");
#endif

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
