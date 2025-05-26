#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

#include "clients/client.h"
#include "clients/clientwindow.h"
#include "command/commandmanager.h"
#include "employees/employee.h"
#include "employees/employeewindow.h"

class MainWindow final : public QMainWindow {
    Q_OBJECT

    QTextEdit* output;
    QLineEdit* phoneInput;
    QComboBox* repairTypeCombo;
    QComboBox* employeeCombo;
    QComboBox* clientCombo;

    CommandManager manager;

    EmployeeWindow* employeePage;
    ClientWindow* clientPage;

public:
    MainWindow();
    ~MainWindow() override;

    private slots:
        void handleRepair();
        void handleUndo();
        void handleRedo();
        void updateEmployeeList() const;
        void updateClientList() const;
};

#endif // MAINWINDOW_H
