#include "employeewindow.h"

EmployeeWindow::EmployeeWindow(QWidget* parent) : QWidget(parent) {
    const auto layout = new QVBoxLayout(this);

    table = new QTableWidget(this);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({"Имя", "Ставка"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    layout->addWidget(table);

    addButton = new QPushButton("Добавить", this);
    removeButton = new QPushButton("Удалить", this);
    updateRateButton = new QPushButton("Изменить ставку", this);
    undoButton = new QPushButton("Undo", this);
    redoButton = new QPushButton("Redo", this);

    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(updateRateButton);
    layout->addWidget(undoButton);
    layout->addWidget(redoButton);

    connect(addButton, &QPushButton::clicked, this, &EmployeeWindow::onAddEmployee);
    connect(removeButton, &QPushButton::clicked, this, &EmployeeWindow::onRemoveEmployee);
    connect(updateRateButton, &QPushButton::clicked, this, &EmployeeWindow::onUpdateRate);
    connect(undoButton, &QPushButton::clicked, this, &EmployeeWindow::onUndo);
    connect(redoButton, &QPushButton::clicked, this, &EmployeeWindow::onRedo);

    refreshTable();
}

void EmployeeWindow::refreshTable() {
    table->setRowCount(static_cast<int>(employees.size()));
    for (int i = 0; i < employees.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(employees[i]->getName()));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(employees[i]->getRate(), 'f', 2)));
    }
    emit employeeListUpdated();
}

void EmployeeWindow::onAddEmployee() {
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить сотрудника");
    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    QLineEdit rateEdit;

    const QRegularExpression rx("^[0-9]+([.,][0-9]{1,2})?$");
    const QValidator *validator = new QRegularExpressionValidator(rx, &rateEdit);
    rateEdit.setValidator(validator);

    form.addRow("Имя:", &nameEdit);
    form.addRow("Ставка:", &rateEdit);

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
        bool ok = false;
        const double rate = rateEdit.text().toDouble(&ok);

        if (name.isEmpty() || !ok) {
            QMessageBox::warning(this, "Ошибка", "Введите корректные данные");
            return;
        }

        employees.append(new Employee(name, rate));
        refreshTable();
    }
}

void EmployeeWindow::onRemoveEmployee() {
    const int row = table->currentRow();
    if (row < 0 || row >= employees.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите сотрудника для удаления");
        return;
    }

    if (const QString name = employees[row]->getName(); QMessageBox::question(this, "Удаление", "Удалить сотрудника: " + name + "?") == QMessageBox::Yes) {
        delete employees.takeAt(row);
        refreshTable();
        QMessageBox::information(this, "Удалено", "Сотрудник успешно удалён");
    }
}

void EmployeeWindow::onUpdateRate() {
    const int row = table->currentRow();
    if (row < 0 || row >= employees.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите сотрудника для редактирования");
        return;
    }

    Employee* emp = employees[row];

    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать сотрудника");
    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    nameEdit.setText(emp->getName());

    QLineEdit rateEdit;
    rateEdit.setText(QString::number(emp->getRate()));

    const QRegularExpression rx("^[0-9]+([.,][0-9]{1,2})?$");
    const QValidator *validator = new QRegularExpressionValidator(rx, &rateEdit);
    rateEdit.setValidator(validator);
    form.addRow("Имя:", &nameEdit);
    form.addRow("Ставка:", &rateEdit);

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
        bool ok;
        const double newRate = rateEdit.text().toDouble(&ok);

        if (newName.isEmpty() || !ok) {
            QMessageBox::warning(this, "Ошибка", "Введите корректные данные");
            return;
        }

        if (emp->getRate() != newRate) {
            cmdManager.execute(new ChangeEmployeeCommand(emp, newName, newRate));
        }

        if (emp->getName() != newName) {
            emp->setName(newName);  // просто применяем
        }

        refreshTable();
    }
}

void EmployeeWindow::onUndo() {
    cmdManager.undo();
    refreshTable();
}

void EmployeeWindow::onRedo() {
    cmdManager.redo();
    refreshTable();
}
