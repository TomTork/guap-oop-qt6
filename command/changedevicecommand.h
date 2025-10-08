#ifndef CHANGEDEVICECOMMAND_H
#define CHANGEDEVICECOMMAND_H

#include "commandmanager.h"
#include "../devices/device.h"

class ChangeDeviceCommand final : public ICommand {
    Device* device;
    QString oldName;
    QString oldSerialNumber;
    QString newName;
    QString newSerialNumber;
    Client* oldClient;
    Client* newClient;
    
public:
    ChangeDeviceCommand(Device* device, const QString& newName, const QString& newSerialNumber)
        : device(device),
          oldName(device->getName()),
          oldSerialNumber(device->getSerialNumber()),
          newName(newName),
          newSerialNumber(newSerialNumber),
          oldClient(device->getClient()),
          newClient(device->getClient()) {}
          
    ChangeDeviceCommand(Device* device, Client* newClient)
        : device(device),
          oldName(device->getName()),
          oldSerialNumber(device->getSerialNumber()),
          newName(device->getName()),
          newSerialNumber(device->getSerialNumber()),
          oldClient(device->getClient()),
          newClient(newClient) {}
    
    void execute() override {
        if (device) {
            device->setName(newName);
            device->setSerialNumber(newSerialNumber);
            device->setClient(newClient);
        }
    }
    
    void undo() override {
        if (device) {
            device->setName(oldName);
            device->setSerialNumber(oldSerialNumber);
            device->setClient(oldClient);
        }
    }
};

#endif // CHANGEDEVICECOMMAND_H
