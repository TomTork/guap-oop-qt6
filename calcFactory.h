#ifndef CALCFACTORY_H
#define CALCFACTORY_H

#include "abstractcalc.h"

class CalcFactory {
public:
    virtual AbstractCalc* create() const = 0;
    virtual ~CalcFactory() = default;
};

#endif // CALCFACTORY_H
