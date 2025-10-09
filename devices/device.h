#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include "../clients/client.h"

class Device {
    QString name; // название девайса
    QString serialNumber; // серийный номер
    Client* client; // привязанный клиент

public:
    Device(const QString& name, const QString& serialNumber, Client* client);
    
    QString getName() const;
    QString getSerialNumber() const;
    Client* getClient() const;
    
    void setName(const QString& newName);
    void setSerialNumber(const QString& newSerial);
    void setClient(Client* newClient);
};

#endif //DEVICE_H
