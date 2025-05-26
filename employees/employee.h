#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>

class Employee {
    QString name;
    double rate;
public:
    Employee(const QString& name, const double rate)
        : name(name), rate(rate) {}

    [[nodiscard]] QString getName() const { return name; }
    [[nodiscard]] double getRate() const { return rate; }

    void setRate(const double newRate) { rate = newRate; }
    void setName(const QString& newName) { name = newName; }
};

#endif // EMPLOYEE_H
