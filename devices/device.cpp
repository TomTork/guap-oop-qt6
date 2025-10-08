#include "device.h"
#include "../clients/client.h"

Device::Device(const QString& name, const QString& serialNumber, Client* client)
    : name(name), serialNumber(serialNumber), client(client) {}

QString Device::getName() const {
    return name;
}

QString Device::getSerialNumber() const {
    return serialNumber;
}

Client* Device::getClient() const {
    return client;
}

void Device::setName(const QString& newName) {
    name = newName;
}

void Device::setSerialNumber(const QString& newSerial) {
    serialNumber = newSerial;
}

void Device::setClient(Client* newClient) {
    client = newClient;
}
