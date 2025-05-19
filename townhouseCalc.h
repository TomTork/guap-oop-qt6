#ifndef TOWNHOUSECALC_H
#define TOWNHOUSECALC_H

#include "abstractCalc.h"

class TownhouseCalc final : public AbstractCalc {
public:
    int getCost(const Estate* est) const override {
        constexpr int base = 100;
        const int areaCost = est->getArea() * 2;
        const int residentCost = est->getResidents() * 10;
        const int durationCost = est->getMonths() * 3;
        constexpr int typeCoef = 3;

        return (base + areaCost + residentCost + durationCost) * typeCoef * static_cast<int>((abs(100 - est->getAge()) + 1) / 1.2);
    }
};

#endif //TOWNHOUSECALC_H
