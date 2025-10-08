#ifndef CHANGEWARRANTYCOMMAND_H
#define CHANGEWARRANTYCOMMAND_H

#include "commandmanager.h"
#include "../warranty/warranty.h"

class ChangeWarrantyCommand final : public ICommand {
    Warranty* warranty;
    QString oldName;
    QString newName;
    int oldDuration;
    int newDuration;
    double oldCost;
    double newCost;

public:
    ChangeWarrantyCommand(Warranty* warranty, const QString& newName, int newDuration, double newCost)
        : warranty(warranty),
          oldName(warranty->getName()),
          newName(newName),
          oldDuration(warranty->getDurationMonths()),
          newDuration(newDuration),
          oldCost(warranty->getCost()),
          newCost(newCost) {}

    void execute() override {
        if (warranty) {
            warranty->setName(newName);
            warranty->setDurationMonths(newDuration);
            warranty->setCost(newCost);
        }
    }

    void undo() override {
        if (warranty) {
            warranty->setName(oldName);
            warranty->setDurationMonths(oldDuration);
            warranty->setCost(oldCost);
        }
    }
};

#endif // CHANGEWARRANTYCOMMAND_H
