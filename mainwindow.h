#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QComboBox>

#include "clients/client.h"
#include "clients/clientwindow.h"
#include "command/commandmanager.h"
#include "devices/deviceswindow.h"
#include "employees/employeewindow.h"
#include "warranty/warrantywindow.h"

class MainWindow final : public QMainWindow {
    Q_OBJECT

    QTextEdit* output;
    // QLineEdit* phoneInput;
    QComboBox* repairTypeCombo;
    QComboBox* employeeCombo;
    QComboBox* clientCombo;
    QComboBox* deviceCombo;
    QComboBox* warrantyCombo;

    CommandManager manager;

    EmployeeWindow* employeePage;
    ClientWindow* clientPage;
    DevicesWindow* devicesPage;
    WarrantyWindow* warrantyPage;

public:
    MainWindow();
    ~MainWindow() override;

    private slots:
        void handleRepair();
        void handleUndo();
        void handleRedo();
        void handleSave();
        void saveCsvFile(QWidget *parent, const QString &csvData);
        void updateEmployeeList() const;
        void updateClientList() const;
        void updateDeviceList() const;
        void updateWarrantyList() const;
};

#endif // MAINWINDOW_H
