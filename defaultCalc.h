#ifndef DEFAULTCALC_H
#define DEFAULTCALC_H

#include "abstractCalc.h"
#include "cottageCalc.h"
#include "economCalc.h"
#include "luxuriousCalc.h"
#include "townhouseCalc.h"

class DefaultCalc {
public:
    static AbstractCalc* createCalc(const Estate* est) {
        switch (est->getType()) {
            case Estate::EstateType::ECONOM:
                return new EconomCalc();
            case Estate::EstateType::COTTAGE:
                return new CottageCalc();
            case Estate::EstateType::LUXURIOUS:
                return new LuxuriousCalc();
            case Estate::EstateType::TOWN_HOUSE:
                return new TownhouseCalc();
            default:
                return nullptr;
        }
    }
};

#endif // DEFAULTCALC_H
