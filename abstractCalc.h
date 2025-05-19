#ifndef ABSTRACTCALC_H
#define ABSTRACTCALC_H

#include "estate.h"

class AbstractCalc {
public:
    virtual int getCost(const Estate* est) const = 0;
    virtual ~AbstractCalc() = default;
};

#endif // ABSTRACTCALC_H
