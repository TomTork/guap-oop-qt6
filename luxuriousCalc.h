#ifndef LUXURIOUSCALC_H
#define LUXURIOUSCALC_H

#include "abstractCalc.h"

class LuxuriousCalc final : public AbstractCalc {
public:
    int getCost(const Estate* est) const override {
        constexpr int base = 100;
        const int areaCost = est->getArea() * 2;
        const int residentCost = est->getResidents() * 10;
        const int durationCost = est->getMonths() * 3;
        constexpr int typeCoef = 2;

        return (base + areaCost + residentCost + durationCost) * typeCoef * static_cast<int>((abs(100 - est->getAge()) + 1) / 1.2);
    }
};

#endif //LUXURIOUSCALC_H
