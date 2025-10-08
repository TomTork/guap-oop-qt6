#ifndef DEVICESWINDOW_H
#define DEVICESWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QList>
#include "device.h"
#include "../command/commandmanager.h"
#include "../command/changedevicecommand.h"

class DevicesWindow : public QWidget {
    Q_OBJECT

    QTableWidget* table;
    QPushButton *addButton, *removeButton, *editButton, *undoButton, *redoButton;
    QList<Device*> devices;
    QList<Client*>* clientsList;  // Reference to the clients list from ClientWindow
    CommandManager cmdManager;
    
    void refreshTable();
    
private slots:
    void onAddDevice();
    void onRemoveDevice();
    void onEditDevice();
    void onSearch(const QString& text);
    void onUndo();
    void onRedo();
    void onUpdateClientsList();
    
    // Helper method to update a device using command pattern
    void updateDevice(Device* device, const QString& newName, const QString& newSerialNumber, Client* newClient = nullptr);
    
public:
    explicit DevicesWindow(QList<Client*>* clients, QWidget* parent = nullptr);
    
    // Getter for the devices list
    const QList<Device*>& getDevices() const { return devices; }
    
signals:
    void deviceListUpdated();
};

#endif // DEVICESWINDOW_H
