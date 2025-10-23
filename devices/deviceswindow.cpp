#include "deviceswindow.h"
#include "../clients/client.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>

DevicesWindow::DevicesWindow(QList<Client*>* clients, QWidget* parent)
    : QWidget(parent), clientsList(clients) {
    const auto layout = new QVBoxLayout(this);

    const auto search = new QLineEdit();
    search->setPlaceholderText("Поиск");
    layout->addWidget(search);

    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Название", "Серийный номер", "Владелец"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(table);

    addButton = new QPushButton("Добавить", this);
    removeButton = new QPushButton("Удалить", this);
    editButton = new QPushButton("Редактировать", this);
    undoButton = new QPushButton("Undo", this);
    redoButton = new QPushButton("Redo", this);

    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(editButton);
    layout->addWidget(undoButton);
    layout->addWidget(redoButton);

    connect(addButton, &QPushButton::clicked, this, &DevicesWindow::onAddDevice);
    connect(removeButton, &QPushButton::clicked, this, &DevicesWindow::onRemoveDevice);
    connect(editButton, &QPushButton::clicked, this, &DevicesWindow::onEditDevice);
    connect(undoButton, &QPushButton::clicked, this, &DevicesWindow::onUndo);
    connect(redoButton, &QPushButton::clicked, this, &DevicesWindow::onRedo);
    connect(search, &QLineEdit::textChanged, this, &DevicesWindow::onSearch);

    refreshTable();
}

void DevicesWindow::onSearch(const QString& text) {
    if (text.isEmpty()) {
        for (int i = 0; i < table->rowCount(); i++) {
            table->setRowHidden(i, false);
        }
        return;
    }

    const QString searchText = text.toLower();
    for (int i = 0; i < table->rowCount(); ++i) {
        table->setRowHidden(i, true);
    }

    for (int i = 0; i < devices.size(); ++i) {
        const Device* device = devices[i];
        const bool matches = device->getName().toLower().contains(searchText) ||
                      device->getSerialNumber().toLower().contains(searchText) ||
                      (device->getClient() && device->getClient()->getName().toLower().contains(searchText));
        
        if (matches) {
            table->setRowHidden(i, false);
        }
    }
}

void DevicesWindow::refreshTable() {
    table->setRowCount(devices.size());
    for (int i = 0; i < devices.size(); ++i) {
        const Device* device = devices[i];
        table->setItem(i, 0, new QTableWidgetItem(device->getName()));
        table->setItem(i, 1, new QTableWidgetItem(device->getSerialNumber()));
        
        QString ownerName = device->getClient() ? 
                           device->getClient()->getName() : 
                           "Не назначен";
        table->setItem(i, 2, new QTableWidgetItem(ownerName));
    }
    emit deviceListUpdated();
}

void DevicesWindow::onAddDevice() {
    if (clientsList->isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Нет доступных клиентов. Сначала создайте клиента.");
        return;
    }

    QDialog dialog(this);
    dialog.setWindowTitle("Добавить устройство");
    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    QLineEdit serialEdit;
    QComboBox clientCombo;
    
    for (Client* client : *clientsList) {
        clientCombo.addItem(client->getName(), QVariant::fromValue(client));
    }

    form.addRow("Название устройства:", &nameEdit);
    form.addRow("Серийный номер:", &serialEdit);
    form.addRow("Клиент:", &clientCombo);

    QPushButton ok("OK");
    QPushButton cancel("Отмена");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&ok);
    btnLayout.addWidget(&cancel);
    form.addRow(&btnLayout);

    connect(&ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        const QString name = nameEdit.text().trimmed();
        const QString serial = serialEdit.text().trimmed();
        
        if (name.isEmpty() || serial.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
            return;
        }

        auto* selectedClient = clientCombo.currentData().value<Client*>();
        const auto newDevice = new Device(name, serial, selectedClient);
        devices.append(newDevice);
        refreshTable();
        emit deviceListUpdated();
    }
}

void DevicesWindow::onRemoveDevice() {
    int row = table->currentRow();
    if (row < 0 || row >= devices.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите устройство для удаления");
        return;
    }

    if (const QString name = devices[row]->getName(); QMessageBox::question(this, "Удаление",
                                                                            "Удалить устройство: " + name + "?") == QMessageBox::Yes) {
        const Device* device = devices.takeAt(row);
        delete device;
        refreshTable();
        emit deviceListUpdated();
        QMessageBox::information(this, "Удалено", "Устройство успешно удалено");
    }
}

void DevicesWindow::onEditDevice() {
    const int row = table->currentRow();
    if (row < 0 || row >= devices.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите устройство для редактирования");
        return;
    }

    Device* device = devices[row];
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать устройство");
    QFormLayout form(&dialog);

    QLineEdit nameEdit(device->getName());
    QLineEdit serialEdit(device->getSerialNumber());
    QComboBox clientCombo;
    
    int currentIndex = -1;
    for (int i = 0; i < clientsList->size(); ++i) {
        Client* client = clientsList->at(i);
        clientCombo.addItem(client->getName(), QVariant::fromValue(client));
        if (device->getClient() == client) {
            currentIndex = i;
        }
    }
    if (currentIndex >= 0) {
        clientCombo.setCurrentIndex(currentIndex);
    }

    form.addRow("Название устройства:", &nameEdit);
    form.addRow("Серийный номер:", &serialEdit);
    form.addRow("Клиент:", &clientCombo);

    QPushButton ok("OK");
    QPushButton cancel("Отмена");
    QHBoxLayout btnLayout;
    btnLayout.addWidget(&ok);
    btnLayout.addWidget(&cancel);
    form.addRow(&btnLayout);

    connect(&ok, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(&cancel, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        const QString newName = nameEdit.text().trimmed();
        const QString newSerial = serialEdit.text().trimmed();
        auto* newClient = clientCombo.currentData().value<Client*>();
        
        if (newName.isEmpty() || newSerial.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
            return;
        }

        updateDevice(device, newName, newSerial, newClient);
    }
}

void DevicesWindow::updateDevice(Device* device, const QString& newName, const QString& newSerialNumber, Client* newClient) {
    if (!device) return;
    
    if (device->getClient() != newClient) {
        cmdManager.execute(new ChangeDeviceCommand(device, newClient));
    } else {
        cmdManager.execute(new ChangeDeviceCommand(device, newName, newSerialNumber));
    }
    
    refreshTable();
    emit deviceListUpdated();
}

void DevicesWindow::onUndo() {
    cmdManager.undo();
    refreshTable();
    emit deviceListUpdated();
}

void DevicesWindow::onRedo() {
    cmdManager.redo();
    refreshTable();
    emit deviceListUpdated();
}

void DevicesWindow::onUpdateClientsList() {
    refreshTable();
}