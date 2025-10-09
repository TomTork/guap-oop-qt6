#ifndef WARRANTY_H
#define WARRANTY_H

#include <QString>

class Warranty {
    QString name; // название гарантии
    int durationMonths; // длительность гарантии
    double cost; // стоимость гарантии

public:
    Warranty(const QString& name = "", int durationMonths = 0, double cost = 0.0)
        : name(name), durationMonths(durationMonths), cost(cost) {}

    QString getName() const { return name; }
    int getDurationMonths() const { return durationMonths; }
    double getCost() const { return cost; }

    void setName(const QString& newName) { name = newName; }
    void setDurationMonths(int months) { durationMonths = months; }
    void setCost(double newCost) { cost = newCost; }
};

#endif // WARRANTY_H
