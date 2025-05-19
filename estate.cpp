#include "estate.h"

Estate::Estate(
    const int age,
    const int area,
    const int residents,
    const int months,
    const EstateType type,
    QString owner,
    QObject *parent
    ) : QObject(parent), age(age), area(area), residents(residents), months(months), type(type), owner(std::move(owner)) {}

int Estate::getAge() const { return age; }
int Estate::getArea() const { return area; }
int Estate::getResidents() const { return residents; }
int Estate::getMonths() const { return months; }
Estate::EstateType Estate::getType() const { return type; }
QString Estate::getOwner() const { return owner; }