#include "calculationFacade.h"
#include "abstractCalc.h"
#include "defaultCalc.h"

CalculationFacade::CalculationFacade(QObject *parent) : QObject(parent) {}

int CalculationFacade::getCost(const Estate* value) {
    const AbstractCalc *abstractCalc = DefaultCalc::createCalc(value);

    return abstractCalc->getCost(value);
}