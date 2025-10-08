#ifndef WARRANTY_H
#define WARRANTY_H

#include <QString>

class Warranty {
    QString name;
    int durationMonths;
    double cost;

public:
    Warranty(const QString& name = "", int durationMonths = 0, double cost = 0.0)
        : name(name), durationMonths(durationMonths), cost(cost) {}

    // Getters
    QString getName() const { return name; }
    int getDurationMonths() const { return durationMonths; }
    double getCost() const { return cost; }

    // Setters
    void setName(const QString& newName) { name = newName; }
    void setDurationMonths(int months) { durationMonths = months; }
    void setCost(double newCost) { cost = newCost; }
};

#endif // WARRANTY_H
