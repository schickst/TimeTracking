
#include "window.hpp"

Window::Window()
{
    readProjectTaskFile();
    createMessageGroupBox();

    createActions();
    createTrayIcon();

    connect(addButton, SIGNAL(clicked()), this, SLOT(writeTask()));
    connect(projectComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(updateTaskComboBox(QString)));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(messageGroupBox);
    setLayout(mainLayout);

    setIcon(0);
    trayIcon->show();

    setWindowTitle(tr("Gringo"));
    resize(400, 300);
}

void Window::setVisible(bool visible)
{
    minimizeAction->setEnabled(visible);
    maximizeAction->setEnabled(!isMaximized());
    restoreAction->setEnabled(isMaximized() || !visible);
    QDialog::setVisible(visible);

    // FIXME Tasks neu lesen
    readProjectTaskFile();
}

void Window::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void Window::setIcon(int index)
{
    QIcon icon(":/images/timer.svg");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);

    trayIcon->setToolTip("Test");
}

void Window::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        setVisible(true);
    case QSystemTrayIcon::DoubleClick:
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void Window::showMessage()
{
    QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(
            taskComboBox->itemData(taskComboBox->currentIndex()).toInt());
    trayIcon->showMessage(titleEdit->text(), descriptionEdit->toPlainText(), icon,
                          loggedHoursSpinBox->value() * 1000);
}

void Window::createMessageGroupBox()
{
    messageGroupBox = new QGroupBox(tr("Log Work"));

    // Project
    projectLabel = new QLabel(tr("Project:"));

    projectComboBox = new QComboBox;
    projectComboBox->addItems(projectTaskList.uniqueKeys());
    projectComboBox->setCurrentIndex(0);

    // Task
    taskLabel = new QLabel(tr("Task:"));

    taskComboBox = new QComboBox;
    taskComboBox->addItems(projectTaskList.value(projectComboBox->currentText()));
    taskComboBox->setCurrentIndex(0);

    durationLabel = new QLabel(tr("Hours:"));

    loggedHoursStartSpinBox = new QDoubleSpinBox;
    loggedHoursStartSpinBox->setRange(0.0, 24.0);
    loggedHoursStartSpinBox->setSingleStep(0.25);
    loggedHoursStartSpinBox->setSuffix(" h");
    loggedHoursStartSpinBox->setValue(10.0);

    loggedHoursSpinBox = new QDoubleSpinBox;
    loggedHoursSpinBox->setRange(0.0, 24.0);
    loggedHoursSpinBox->setSingleStep(0.25);
    loggedHoursSpinBox->setSuffix(" h");
    loggedHoursSpinBox->setValue(1.0);

    loggedHoursEndSpinBox = new QDoubleSpinBox;
    loggedHoursEndSpinBox->setRange(0.0, 24.0);
    loggedHoursEndSpinBox->setSingleStep(0.25);
    loggedHoursEndSpinBox->setSuffix(" h");
    loggedHoursEndSpinBox->setValue(11.0);

    connect(loggedHoursStartSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateHoursStartSpinBoxes(double)));
    connect(loggedHoursSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateHoursSpinBoxes(double)));
    connect(loggedHoursEndSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateHourEndSpinBoxes(double)));


    descriptionLabel = new QLabel(tr("Description:"));
    descriptionEdit = new QTextEdit;

    addButton = new QPushButton(tr("Add"));
    addButton->setDefault(true);

    QGridLayout *messageLayout = new QGridLayout;
    messageLayout->addWidget(projectLabel, 0, 0);
    messageLayout->addWidget(projectComboBox, 0, 1, 1, 4);
    messageLayout->addWidget(taskLabel, 1, 0);
    messageLayout->addWidget(taskComboBox, 1, 1, 1, 4);
    messageLayout->addWidget(durationLabel, 2, 0);
    messageLayout->addWidget(loggedHoursStartSpinBox, 2, 1, 1, 1);
    messageLayout->addWidget(loggedHoursSpinBox, 2, 2, 1, 1);
    messageLayout->addWidget(loggedHoursEndSpinBox, 2, 3, 1, 1);
    messageLayout->addWidget(descriptionLabel, 3, 0);
    messageLayout->addWidget(descriptionEdit, 3, 1, 1, 4);
    messageLayout->addWidget(addButton, 5, 4);
    messageGroupBox->setLayout(messageLayout);
}

void Window::createActions()
{
    minimizeAction = new QAction(tr("Mi&nimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

    maximizeAction = new QAction(tr("Ma&ximize"), this);
    connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

    generateInvoiceAction = new QAction(tr("Generate &Invoice"), this);
    connect(generateInvoiceAction, SIGNAL(triggered()), this, SLOT(generateInvoice()));

    configureTasksAction = new QAction(tr("Configure &Tasks"), this);
    connect(configureTasksAction, SIGNAL(triggered()), this, SLOT(configureTasks()));

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Window::createTrayIcon()
{
    trayIconMenu = new QMenu(this);

    trayIconMenu->addAction(generateInvoiceAction);
    trayIconMenu->addAction(configureTasksAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
}

void Window::updateTaskComboBox(QString project)
{
    taskComboBox->clear();
    taskComboBox->addItems(projectTaskList.value(project));
    taskComboBox->setCurrentIndex(0);
}

void Window::updateHoursStartSpinBoxes(double value)
{
    double newValue = value + loggedHoursSpinBox->value();
    loggedHoursEndSpinBox->setValue(newValue);
}

void Window::updateHoursSpinBoxes(double value)
{
    double newValue = loggedHoursStartSpinBox->value() + value;
    loggedHoursEndSpinBox->setValue(newValue);
}

void Window::updateHourEndSpinBoxes(double value)
{
    double newValue = value - loggedHoursSpinBox->value();
    loggedHoursStartSpinBox->setValue(newValue);
}


void Window::generateInvoice()
{
    // projekt und task aufwände berechnen
    QSettings settings;

    // pdflatex starten
    QString templateFileName = settings.value("InvoiceGenerator", "").toString();
    QFile templateFile(templateFileName);

    if (!templateFile.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(0, tr("Gringo"), tr("Unable to find Template File!"));
        return;
    }

    QString destinationPath = settings.value("InvoiceDestinationPath", "").toString();
    QString destinationFileName = "invoice_" + QDate::currentDate().toString() + ".tex";
    QFile destinationFile(destinationPath + destinationFileName);

    if (!destinationFile.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(0, tr("Gringo"), tr("Unable to find Template File!"));
        return;
    }

    QTextStream templateStream(&templateFile);
    QTextStream destinationStream(&destinationFile);

    while (!templateStream.atEnd()) {
        QString line = templateStream.readLine();

        if (line.contains("{% data %}")) {
            QString logFileName = settings.value("LogFile", "").toString();
            QFile logFile(logFileName);

            if (!logFile.open(QFile::ReadOnly | QFile::Text)) {
                QMessageBox::warning(0, tr("Gringo"), tr("Unable to open Log File!"));
                return;
            }
            QTextStream logStream(&logFile);

            // FIXME Berechnen und in datei schreiben
            logStream << "Test";


            logFile.close();
        }
        destinationStream << line;
    }
    destinationFile.close();
    templateFile.close();

    QString program = settings.value("InvoiceGenerator", "").toString();
    QStringList arguments;
    arguments << destinationFileName;

    QProcess *process = new QProcess();
    process->start(program, arguments);
    qDebug() << "Generate Invoice";
}

void Window::configureTasks()
{
    QSettings settings;
    QString file = settings.value("TaskFile", "").toString();

    if (file.isEmpty()) {
        QMessageBox::warning(0, tr("Gringo"), tr("Unable to find Task File!"));
        return;
    }

    QString program = settings.value("TextEditor", "").toString();
    QStringList arguments;
    arguments << file;

    QProcess *process = new QProcess();
    process->start(program, arguments);
    // Open Config
    // urxvt -e vim Tasks.txt
    // on finished: readProjectTaskFile();
}

void Window::readProjectTaskFile()
{    
    QSettings settings;
    QString fileName = settings.value("TaskFile", "").toString();
    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(0, tr("Gringo"), tr("Unable to find Task File!"));
        return;
    }

    projectTaskList.clear();

    QTextStream input(&file);
    while(!input.atEnd()) {
        QStringList line = input.readLine().split(';');

        if (line.size() == 2) {
            if (projectTaskList.find(line.at(0)) == projectTaskList.end()) {
                QStringList tasks;
                tasks << line.at(1);
                projectTaskList.insert(line.at(0), tasks);
            } else {
                QStringList tasks = projectTaskList.value(line.at(0));
                tasks << line.at(1);
                projectTaskList.insert(line.at(0), tasks);
            }
        }
    }
    file.close();
    qDebug() << "Read project task file";
}


void Window::writeTask()
{
    QSettings settings;
    QString fileName = settings.value("LogFile", "").toString();
    QFile file(fileName);

    if (!file.open(QFile::Append | QFile::Text)) {
        QMessageBox::warning(0, tr("Gringo"), tr("Unable to find Log File!"));
        return;
    }

    QTextStream stream(&file);
    /*
     * Ein Eintag besitzt folgende Bestandteile:
     * - Username
     * - Datum
     * - Projekt
     * - Task
     * - Zeit (Start/Duration/End)
     * - Beschreibung
     */
    QString line;

    // Username
#ifdef Q_OS_LINUX
    QString user = getenv("USER");     //for Mac or Linux
#else
    QString user = getenv("USERNAME"); //for windows
#endif
    line.append(user + ";");
    line.append(QDate::currentDate().toString() + ";");     // Datum
    line.append(projectComboBox->currentText() + ";");      // Projekt
    line.append(taskComboBox->currentText() + ";");         // Task
    line.append(QString::number(loggedHoursStartSpinBox->value()) + ";");    // Startzeit
    line.append(QString::number(loggedHoursSpinBox->value()) + ";");         // Dauer
    line.append(QString::number(loggedHoursEndSpinBox->value()) + ";");    // Endzeit
    line.append(descriptionEdit->toPlainText() + ";");      // Description
    line.append("\n");

    stream << line;
    file.close();

    descriptionEdit->clear();
}




























