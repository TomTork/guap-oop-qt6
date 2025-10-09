#include "equipmentwindow.h"
#include "../command/changeequipmentcommand.h"
#include <iostream>

EquipmentWindow::EquipmentWindow(QWidget* parent) : QWidget(parent) {
    const auto layout = new QVBoxLayout(this);

    const auto search = new QLineEdit();
    search->setPlaceholderText("Поиск");
    layout->addWidget(search);

    table = new QTableWidget(this);
    table->setColumnCount(1);
    table->setHorizontalHeaderLabels({"Название оборудования"});
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

    connect(addButton, &QPushButton::clicked, this, &EquipmentWindow::onAddEquipment);
    connect(removeButton, &QPushButton::clicked, this, &EquipmentWindow::onRemoveEquipment);
    connect(editButton, &QPushButton::clicked, this, &EquipmentWindow::onEditEquipment);
    connect(undoButton, &QPushButton::clicked, this, &EquipmentWindow::onUndo);
    connect(redoButton, &QPushButton::clicked, this, &EquipmentWindow::onRedo);
    connect(search, &QLineEdit::textChanged, this, &EquipmentWindow::onSearch);

    refreshTable();
}

void EquipmentWindow::onSearch(const QString& text) {
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
    for (int i = 0; i < equipments.size(); ++i) {
        if (equipments[i]->getName().toLower().contains(searchText)) {
            table->setRowHidden(i, false);
        }
    }
}

void EquipmentWindow::refreshTable() {
    table->setRowCount(static_cast<int>(equipments.size()));
    for (int i = 0; i < equipments.size(); ++i) {
        table->setItem(i, 0, new QTableWidgetItem(equipments[i]->getName()));
    }
    emit equipmentListUpdated();
}

void EquipmentWindow::onAddEquipment() {
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить оборудование");
    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    form.addRow("Название оборудования:", &nameEdit);

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
            QMessageBox::warning(this, "Ошибка", "Введите название оборудования");
            return;
        }

        equipments.append(new Equipment(name));
        refreshTable();
    }
}

void EquipmentWindow::onRemoveEquipment() {
    int row = table->currentRow();
    if (row < 0 || row >= equipments.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите оборудование для удаления");
        return;
    }

    const QString name = equipments[row]->getName();
    if (QMessageBox::question(this, "Удаление", "Удалить оборудование: " + name + "?") == QMessageBox::Yes) {
        delete equipments.takeAt(row);
        refreshTable();
        QMessageBox::information(this, "Удалено", "Клиент успешно удалён");
    }
}

void EquipmentWindow::onEditEquipment() {
    int row = table->currentRow();
    if (row < 0 || row >= equipments.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите оборудование для редактирования");
        return;
    }

    Equipment* equipment = equipments[row];

    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать оборудование");
    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    nameEdit.setText(equipment->getName());
    form.addRow("Новое название:", &nameEdit);

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
            QMessageBox::warning(this, "Ошибка", "Название не может быть пустым");
            return;
        }

        if (newName != equipment->getName()) {
            cmdManager.execute(new ChangeEquipmentCommand(equipment, newName));
            refreshTable();
        }
    }
}

void EquipmentWindow::onUndo() {
    cmdManager.undo();
    refreshTable();
}

void EquipmentWindow::onRedo() {
    cmdManager.redo();
    refreshTable();
}
