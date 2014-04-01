#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QSystemTrayIcon>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QtGui>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QAction>
#include <QLineEdit>
#include <QMenu>

class Window : public QDialog
{
    Q_OBJECT

public:
    Window();

    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void setIcon(int index);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void updateTaskComboBox(QString project);
    void showMessage();

    void generateInvoice();
    void configureTasks();

    void writeTask();

private:
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();
    void readProjectTaskFile();

    QGroupBox *messageGroupBox;
    QLabel *durationLabel;
    QLabel *titleLabel;
    QLabel *descriptionLabel;
    QSpinBox *durationSpinBox;
    QLineEdit *titleEdit;
    QTextEdit *descriptionEdit;
    QPushButton *showMessageButton;

    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;

    // ------------------------------------
    QPushButton *addButton;
    QLabel *taskLabel;
    QLabel *projectLabel;
    QComboBox *projectComboBox;
    QComboBox *taskComboBox;
    QDoubleSpinBox *loggedHoursSpinBox;
    QLineEdit *descriptionLineEdit;

    QAction *generateInvoiceAction;
    QAction *configureTasksAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QMap<QString, QStringList> projectTaskList;
};



#endif // WINDOW_HPP
