#ifndef CHANGEEMPLOYEECOMMAND_H
#define CHANGEEMPLOYEECOMMAND_H

#include "commandmanager.h"
#include "../employees/employee.h"

class ChangeEmployeeCommand final : public ICommand {
    Employee* employee;
    QString oldName;
    QString newName;
    double oldRate;
    double newRate;

public:
    ChangeEmployeeCommand(Employee* emp, const QString& newName, const double newRate)
        : employee(emp),
          oldName(emp->getName()), newName(newName),
          oldRate(emp->getRate()), newRate(newRate) {}

    void execute() override {
        employee->setName(newName);
        employee->setRate(newRate);
    }

    void undo() override {
        employee->setName(oldName);
        employee->setRate(oldRate);
    }
};

#endif // CHANGEEMPLOYEECOMMAND_H
