#ifndef CHANGEEQUIPMENTCOMMAND_H
#define CHANGEEQUIPMENTCOMMAND_H

#include "commandmanager.h"
#include "../equipment/equipment.h"

class ChangeEquipmentCommand final : public ICommand {
    Equipment* equipment;
    QString oldName;
    QString newName;
public:
    ChangeEquipmentCommand(Equipment* equipment, const QString& newName)
        : equipment(equipment), oldName(equipment->getName()), newName(newName) {}

    void execute() override {
        equipment->setName(newName);
    }

    void undo() override {
        equipment->setName(oldName);
    }
};

#endif //CHANGEEQUIPMENTCOMMAND_H
