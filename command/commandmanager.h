#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QStack>

// Поведенческий паттерн проектирования
class ICommand {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
    virtual ~ICommand() = default;
};

class CommandManager {
    QStack<ICommand*> undoStack;
    QStack<ICommand*> redoStack;
public:
    void execute(ICommand* cmd) {
        cmd->execute();
        undoStack.push(cmd);
        qDeleteAll(redoStack);
        redoStack.clear();
    }

    void undo() {
        if (!undoStack.isEmpty()) {
            ICommand* cmd = undoStack.pop();
            cmd->undo();
            redoStack.push(cmd);
        }
    }

    void redo() {
        if (!redoStack.isEmpty()) {
            ICommand* cmd = redoStack.pop();
            cmd->execute();
            undoStack.push(cmd);
        }
    }

    ~CommandManager() {
        qDeleteAll(undoStack);
        qDeleteAll(redoStack);
    }
};

#endif // COMMANDMANAGER_H
