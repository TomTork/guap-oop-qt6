#ifndef DECORATEDREPAIR_H
#define DECORATEDREPAIR_H

#include "repairfactory.h"

class DecoratedRepair final : public RepairType {
    RepairType* base;
    QString suffix;
public:
    DecoratedRepair(RepairType* base, const QString& suffix)
        : base(base), suffix(suffix) {}

    QString process(const QString& phoneModel) override {
        return base->process(phoneModel) + suffix + ";";
    }

    ~DecoratedRepair() override {
        delete base;
    }

    int cost() override {
        return 0;
    }
};

#endif // DECORATEDREPAIR_H
