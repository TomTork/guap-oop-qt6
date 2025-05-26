#ifndef EMPLOYEEWINDOW_H
#define EMPLOYEEWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QFormLayout>

#include "employee.h"
#include "../command/commandmanager.h"

class EmployeeWindow final : public QWidget {
    Q_OBJECT

public:
    QList<Employee*> employees;
    explicit EmployeeWindow(QWidget* parent = nullptr);

private:
    CommandManager cmdManager;

    QTableWidget* table;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* updateRateButton;
    QPushButton* undoButton;
    QPushButton* redoButton;

    void refreshTable();

    private slots:
        void onAddEmployee();
    void onRemoveEmployee();
    void onUpdateRate();
    void onUndo();
    void onRedo();

    signals:
    void employeeListUpdated();
};

#endif // EMPLOYEEWINDOW_H
