#include "mainwindow.h"
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QLabel>

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
    actions->addWidget(service);
    actions->addWidget(employees);
    actions->addWidget(clients);
    layout->addLayout(actions);

    auto* servicePage = new QWidget();
    auto* serviceLayout = new QVBoxLayout(servicePage);

    phoneInput = new QLineEdit();
    phoneInput->setPlaceholderText("Введите модель телефона");

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

    serviceLayout->addWidget(phoneInput);
    serviceLayout->addWidget(repairTypeCombo);
    serviceLayout->addWidget(employeeCombo);
    serviceLayout->addWidget(clientCombo);
    serviceLayout->addWidget(repairBtn);
    const auto buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(undoBtn);
    buttonsLayout->addWidget(redoBtn);
    serviceLayout->addLayout(buttonsLayout);
    serviceLayout->addWidget(output);

    connect(repairBtn, &QPushButton::clicked, this, &MainWindow::handleRepair);
    connect(undoBtn, &QPushButton::clicked, this, &MainWindow::handleUndo);
    connect(redoBtn, &QPushButton::clicked, this, &MainWindow::handleRedo);

    employeePage = new EmployeeWindow();
    clientPage = new ClientWindow();

    auto* stacked = new QStackedWidget();
    stacked->addWidget(servicePage);
    stacked->addWidget(employeePage);
    stacked->addWidget(clientPage);
    layout->addWidget(stacked);

    connect(service, &QPushButton::clicked, [stacked]() { stacked->setCurrentIndex(0); });
    connect(employees, &QPushButton::clicked, [stacked]() { stacked->setCurrentIndex(1); });
    connect(clients, &QPushButton::clicked, [stacked]() { stacked->setCurrentIndex(2); });
    connect(employeePage, &EmployeeWindow::employeeListUpdated, this, &MainWindow::updateEmployeeList);
    connect(clientPage, &ClientWindow::clientListUpdated, this, &MainWindow::updateClientList);

    setCentralWidget(central);
}

MainWindow::~MainWindow() {
    qDeleteAll(employeePage->employees);
}

void MainWindow::handleRepair() {
    const QString model = phoneInput->text();
    if (model.isEmpty()) return;

    const auto type = static_cast<RepairFactory::Type>(repairTypeCombo->currentData().toInt());
    RepairType* repair = RepairFactory::create(type);

    const Employee* selectedEmployee = nullptr;
    if (const int index = employeeCombo->currentIndex(); index >= 0 && index < employeePage->employees.size())
        selectedEmployee = employeePage->employees[index];

    const Client* selectedClient = nullptr;
    if (const int index = clientCombo->currentIndex(); index >= 0 && index < clientPage->clients.size())
        selectedClient = clientPage->clients[index];

    const QString costString = QString(" стоимость услуги: %1; клиент: %2")
        .arg((selectedEmployee ? selectedEmployee->getRate() : 0) + repair->cost())
        .arg(selectedClient ? selectedClient->getName() : "<не выбран>");

    auto* decorated = new class DecoratedRepair(repair, costString);
    manager.execute(new RepairCommand(model, decorated, output));
}

void MainWindow::handleUndo() {
    manager.undo();
}

void MainWindow::handleRedo() {
    manager.redo();
}

void MainWindow::updateEmployeeList() const {
    employeeCombo->clear();
    for (auto* emp : employeePage->employees)
        employeeCombo->addItem(emp->getName() + QString(" (ставка: %1)").arg(emp->getRate()), QVariant::fromValue<void*>(emp));
}

void MainWindow::updateClientList() const {
    clientCombo->clear();
    for (const auto* cli : clientPage->clients)
        clientCombo->addItem(cli->getName());
}