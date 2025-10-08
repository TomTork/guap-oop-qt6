#ifndef DEVICE_H
#define DEVICE_H

#include <QString>

class Client;  // Forward declaration

class Device {
    QString name;
    QString serialNumber;
    Client* client;

public:
    Device(const QString& name, const QString& serialNumber, Client* client);
    
    // Getters
    QString getName() const;
    QString getSerialNumber() const;
    Client* getClient() const;
    
    // Setters
    void setName(const QString& newName);
    void setSerialNumber(const QString& newSerial);
    void setClient(Client* newClient);
};

#endif //DEVICE_H
