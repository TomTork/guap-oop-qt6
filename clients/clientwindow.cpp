#include "clientwindow.h"

#include <iostream>

ClientWindow::ClientWindow(QWidget* parent) : QWidget(parent) {
    const auto layout = new QVBoxLayout(this);

    const auto search = new QLineEdit();
    search->setPlaceholderText("Поиск");
    layout->addWidget(search);

    table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setHorizontalHeaderLabels({"Имя клиента"});
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

    connect(addButton, &QPushButton::clicked, this, &ClientWindow::onAddClient);
    connect(removeButton, &QPushButton::clicked, this, &ClientWindow::onRemoveClient);
    connect(editButton, &QPushButton::clicked, this, &ClientWindow::onEditClient);
    connect(undoButton, &QPushButton::clicked, this, &ClientWindow::onUndo);
    connect(redoButton, &QPushButton::clicked, this, &ClientWindow::onRedo);
    connect(search, &QLineEdit::textChanged, this, &ClientWindow::onSearch);

    refreshTable();
}

void ClientWindow::onSearch(const QString& text) {
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
    for (int i = 0; i < clients.size(); ++i) {
        if (clients[i]->getName().toLower().contains(searchText)) {
            table->setRowHidden(i, false);
        }
    }
}

void ClientWindow::refreshTable() {
    table->setRowCount(static_cast<int>(clients.size()));
    for (int i = 0; i < clients.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(clients[i]->getName()));
    }
    emit clientListUpdated();
}

void ClientWindow::onAddClient() {
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить клиента");
    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    form.addRow("Имя клиента:", &nameEdit);

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
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите имя клиента");
            return;
        }

        clients.append(new Client(name));
        refreshTable();
    }
}

void ClientWindow::onRemoveClient() {
    const int row = table->currentRow();
    if (row < 0 || row >= clients.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите клиента для удаления");
        return;
    }

    const QString name = clients[row]->getName();
    if (QMessageBox::question(this, "Удаление", "Удалить клиента: " + name + "?") == QMessageBox::Yes) {
        delete clients.takeAt(row);
        refreshTable();
        QMessageBox::information(this, "Удалено", "Клиент успешно удалён");
    }
}

void ClientWindow::onEditClient() {
    const int row = table->currentRow();
    if (row < 0 || row >= clients.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите клиента для редактирования");
        return;
    }

    Client* client = clients[row];

    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать клиента");
    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    nameEdit.setText(client->getName());
    form.addRow("Новое имя:", &nameEdit);

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
        if (newName.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Имя не может быть пустым");
            return;
        }

        if (newName != client->getName()) {
            cmdManager.execute(new ChangeClientNameCommand(client, newName));
            refreshTable();
        }
    }
}

void ClientWindow::onUndo() {
    cmdManager.undo();
    refreshTable();
}

void ClientWindow::onRedo() {
    cmdManager.redo();
    refreshTable();
}
