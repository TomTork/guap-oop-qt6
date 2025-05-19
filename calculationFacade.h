#ifndef CALCULATIONFACADE_H
#define CALCULATIONFACADE_H

#include "estate.h"

class CalculationFacade final : public QObject {
    Q_OBJECT
public:
    explicit CalculationFacade(QObject *parent = nullptr);
    static int getCost(const Estate *value);
};

#endif //CALCULATIONFACADE_H
