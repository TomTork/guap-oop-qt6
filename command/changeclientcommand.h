#ifndef CHANGECLIENTCOMMAND_H
#define CHANGECLIENTCOMMAND_H

#include "commandmanager.h"
#include "../clients/client.h"

class ChangeClientNameCommand final : public ICommand {
    Client* client;
    QString oldName;
    QString newName;
public:
    ChangeClientNameCommand(Client* client, const QString& newName)
        : client(client), oldName(client->getName()), newName(newName) {}

    void execute() override {
        client->setName(newName);
    }

    void undo() override {
        client->setName(oldName);
    }
};

#endif //CHANGECLIENTCOMMAND_H
