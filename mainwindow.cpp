#include "mainwindow.h"
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QStandardPaths>

#include "clients/clientwindow.h"
#include "employees/employeewindow.h"
#include "repair/repairfactory.h"
#include "command/repaircommand.h"
#include "repair/decoratedrepair.h"

MainWindow::MainWindow() {
    auto* central = new QWidget();
    const auto layout = new QVBoxLayout(central);

    const auto actions = new QHBoxLayout();
    const auto service = new QPushButton("Услуга");
    const auto employees = new QPushButton("Сотрудники");
    const auto clients = new QPushButton("Клиенты");
    const auto devices = new QPushButton("Девайсы");
    const auto warranties = new QPushButton("Гарантии");
    actions->addWidget(service);
    actions->addWidget(employees);
    actions->addWidget(clients);
    actions->addWidget(devices);
    actions->addWidget(warranties);
    layout->addLayout(actions);

    auto* servicePage = new QWidget();
    auto* serviceLayout = new QVBoxLayout(servicePage);

    repairTypeCombo = new QComboBox();
    repairTypeCombo->addItem("Экран", RepairFactory::Screen);
    repairTypeCombo->addItem("Аккумулятор", RepairFactory::Battery);
    repairTypeCombo->addItem("Камера", RepairFactory::Camera);
    repairTypeCombo->addItem("Кнопки", RepairFactory::Buttons);

    auto* repairBtn = new QPushButton("Выполнить ремонт");
    const auto undoBtn = new QPushButton("Undo");
    const auto redoBtn = new QPushButton("Redo");

    output = new QTextEdit();
    output->setReadOnly(true);

    employeeCombo = new QComboBox(this);
    employeeCombo->setPlaceholderText("Выберите сотрудника");
    clientCombo = new QComboBox(this);
    clientCombo->setPlaceholderText("Выберите клиента");
    deviceCombo = new QComboBox(this);
    deviceCombo->setPlaceholderText("Выберите устройство");
    warrantyCombo = new QComboBox(this);
    warrantyCombo->setPlaceholderText("Выберите гарантию");

    const auto saveDataToCsv = new QPushButton("Сохранить данные в CSV");

    serviceLayout->addWidget(repairTypeCombo);
    serviceLayout->addWidget(employeeCombo);
    serviceLayout->addWidget(clientCombo);
    serviceLayout->addWidget(deviceCombo);
    serviceLayout->addWidget(warrantyCombo);
    serviceLayout->addWidget(repairBtn);
    const auto buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(undoBtn);
    buttonsLayout->addWidget(redoBtn);
    serviceLayout->addLayout(buttonsLayout);
    serviceLayout->addWidget(output);
    serviceLayout->addWidget(saveDataToCsv);

    connect(repairBtn, &QPushButton::clicked, this, &MainWindow::handleRepair);
    connect(undoBtn, &QPushButton::clicked, this, &MainWindow::handleUndo);
    connect(redoBtn, &QPushButton::clicked, this, &MainWindow::handleRedo);
    connect(saveDataToCsv, &QPushButton::clicked, this, &MainWindow::handleSave);

    employeePage = new EmployeeWindow();
    clientPage = new ClientWindow();
    devicesPage = new DevicesWindow(&clientPage->clients);
    warrantyPage = new WarrantyWindow();

    auto* stacked = new QStackedWidget();
    stacked->addWidget(servicePage);
    stacked->addWidget(employeePage);
    stacked->addWidget(clientPage);
    stacked->addWidget(devicesPage);
    stacked->addWidget(warrantyPage);
    layout->addWidget(stacked);

    connect(service, &QPushButton::clicked, [stacked]() { stacked->setCurrentIndex(0); });
    connect(employees, &QPushButton::clicked, [stacked]() { stacked->setCurrentIndex(1); });
    connect(clients, &QPushButton::clicked, [stacked]() { stacked->setCurrentIndex(2); });
    connect(devices, &QPushButton::clicked, [stacked]() { stacked->setCurrentIndex(3); });
    connect(warranties, &QPushButton::clicked, [stacked]() { stacked->setCurrentIndex(4); });
    connect(employeePage, &EmployeeWindow::employeeListUpdated, this, &MainWindow::updateEmployeeList);
    connect(clientPage, &ClientWindow::clientListUpdated, this, &MainWindow::updateClientList);
    connect(devicesPage, &DevicesWindow::deviceListUpdated, this, &MainWindow::updateDeviceList);
    connect(warrantyPage, &WarrantyWindow::warrantyListUpdated, this, &MainWindow::updateWarrantyList);

    setCentralWidget(central);
}

MainWindow::~MainWindow() {
    qDeleteAll(employeePage->employees);
}

void MainWindow::handleRepair() {
    const Warranty* selectedWarranty = nullptr;
    if (const int index = warrantyCombo->currentIndex(); index >= 0 && index < warrantyPage->getWarranties().size())
        selectedWarranty = warrantyPage->getWarranties()[index];

    const Device* selectedDevice = nullptr;
    if (const int index = deviceCombo->currentIndex(); index >= 0 && index < devicesPage->getDevices().size())
        selectedDevice = devicesPage->getDevices()[index];
    if (!selectedDevice || selectedDevice->getName().isEmpty()) return;

    const auto type = static_cast<RepairFactory::Type>(repairTypeCombo->currentData().toInt());
    RepairType* repair = RepairFactory::create(type);

    const Employee* selectedEmployee = nullptr;
    if (const int index = employeeCombo->currentIndex(); index >= 0 && index < employeePage->employees.size())
        selectedEmployee = employeePage->employees[index];

    const Client* selectedClient = nullptr;
    if (const int index = clientCombo->currentIndex(); index >= 0 && index < clientPage->clients.size())
        selectedClient = clientPage->clients[index];

    const QString costString = QString(" стоимость услуги: %1; клиент: %2; гарантия: %3")
        .arg((selectedEmployee ? selectedEmployee->getRate() : 0) + repair->cost() + (selectedWarranty ? selectedWarranty->getCost() : 0))
        .arg(selectedClient ? selectedClient->getName() : "<не выбран>")
        .arg(selectedWarranty ? selectedWarranty->getName().toStdString() + " (" + std::to_string(selectedWarranty->getDurationMonths()) + ")" : "<не выбран>");

    auto* decorated = new class DecoratedRepair(repair, costString);
    manager.execute(new RepairCommand(selectedDevice->getName(), decorated, output));
}

void MainWindow::handleUndo() {
    manager.undo();
}

void MainWindow::handleRedo() {
    manager.redo();
}

void MainWindow::updateEmployeeList() const {
    employeeCombo->clear();
    for (const auto* emp : employeePage->employees) {
        employeeCombo->addItem(emp->getName() + QString(" (ставка: %1)").arg(emp->getRate()), QVariant::fromValue<void*>(const_cast<Employee*>(emp)));
    }
}

void MainWindow::updateClientList() const {
    clientCombo->clear();
    for (const auto* client : clientPage->clients) {
        clientCombo->addItem(client->getName(), QVariant::fromValue(const_cast<Client*>(client)));
    }
}

void MainWindow::updateDeviceList() const {
    deviceCombo->clear();
    const auto& devices = devicesPage->getDevices();
    for (const auto* device : devices) {
        QString displayText = QString("%1 (Серийный: %2)")
            .arg(device->getName())
            .arg(device->getSerialNumber());

        deviceCombo->addItem(displayText, QVariant::fromValue(const_cast<Device*>(device)));
    }
}

void MainWindow::updateWarrantyList() const {
    warrantyCombo->clear();
    const auto& warranties = warrantyPage->getWarranties();
    for (const auto* warranty : warranties) {
        QString displayText = QString("%1 — %2 (%3)")
            .arg(warranty->getName())
            .arg(warranty->getCost())
            .arg(warranty->getDurationMonths());

        warrantyCombo->addItem(displayText, QVariant::fromValue(const_cast<Warranty*>(warranty)));
    }
}

void MainWindow::saveCsvFile(QWidget *parent, const QString &csvData) {
    QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString defaultFile = downloadsPath + QDir::separator() + "данные.csv";
    QString fileName = QFileDialog::getSaveFileName(
        parent,
        QObject::tr("Сохранить CSV файл"),
        defaultFile,
        QObject::tr("CSV файлы (*.csv)")
    );

    if (fileName.isEmpty())
        return;

    if (!fileName.endsWith(".csv", Qt::CaseInsensitive))
        fileName += ".csv";

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(parent, QObject::tr("Ошибка"),
                             QObject::tr("Не удалось открыть файл для записи:\n%1").arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);
    out << csvData;
    file.close();

    QMessageBox::information(parent, QObject::tr("Успешно"),
                             QObject::tr("Файл сохранён в: %1").arg(fileName));
}

void MainWindow::handleSave() {
    saveCsvFile(this, output->toPlainText());
}