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

class DevicesWindow final : public QWidget {
    Q_OBJECT

    QTableWidget* table;
    QPushButton *addButton, *removeButton, *editButton, *undoButton, *redoButton;
    QList<Device*> devices;
    QList<Client*>* clientsList;
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
    
    void updateDevice(Device* device, const QString& newName, const QString& newSerialNumber, Client* newClient = nullptr);
    
public:
    explicit DevicesWindow(QList<Client*>* clients, QWidget* parent = nullptr);
    
    const QList<Device*>& getDevices() const { return devices; }
    
signals:
    void deviceListUpdated();
};

#endif // DEVICESWINDOW_H
