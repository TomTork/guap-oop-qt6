// repaircommand.h

#ifndef REPAIRCOMMAND_H
#define REPAIRCOMMAND_H

#include "commandmanager.h"
#include "../repair/repairfactory.h"
#include <QTextEdit>

class RepairCommand final : public ICommand {
    QString phoneModel;
    RepairType* repair;
    QTextEdit* output;
    QString previousText;
public:
    RepairCommand(const QString& model, RepairType* rep, QTextEdit* out)
        : phoneModel(model), repair(rep), output(out) {}

    void execute() override {
        previousText = output->toPlainText();
        output->append(repair->process(phoneModel));
    }

    void undo() override {
        output->setPlainText(previousText);
    }

    ~RepairCommand() override {
        delete repair;
    }
};

#endif // REPAIRCOMMAND_H
