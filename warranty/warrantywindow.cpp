#include "warrantywindow.h"
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QInputDialog>
#include <QHeaderView>
#include <QSpinBox>
#include <QDoubleSpinBox>

WarrantyWindow::WarrantyWindow(QWidget* parent) : QWidget(parent) {
    const auto layout = new QVBoxLayout(this);

    const auto search = new QLineEdit();
    search->setPlaceholderText("Поиск");
    layout->addWidget(search);

    table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Название", "Срок (мес.)", "Стоимость"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

    connect(addButton, &QPushButton::clicked, this, &WarrantyWindow::onAddWarranty);
    connect(removeButton, &QPushButton::clicked, this, &WarrantyWindow::onRemoveWarranty);
    connect(editButton, &QPushButton::clicked, this, &WarrantyWindow::onEditWarranty);
    connect(undoButton, &QPushButton::clicked, this, &WarrantyWindow::onUndo);
    connect(redoButton, &QPushButton::clicked, this, &WarrantyWindow::onRedo);
    connect(search, &QLineEdit::textChanged, this, &WarrantyWindow::onSearch);

    refreshTable();
}

WarrantyWindow::~WarrantyWindow() {
    qDeleteAll(warranties);
}

void WarrantyWindow::refreshTable() {
    table->setRowCount(warranties.size());
    for (int i = 0; i < warranties.size(); ++i) {
        const Warranty* warranty = warranties[i];
        table->setItem(i, 0, new QTableWidgetItem(warranty->getName()));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(warranty->getDurationMonths())));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(warranty->getCost(), 'f', 2)));
    }
    emit warrantyListUpdated();
}

void WarrantyWindow::onAddWarranty() {
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить гарантию");
    QFormLayout form(&dialog);

    QLineEdit nameEdit;
    QSpinBox durationSpinBox;
    QDoubleSpinBox costSpinBox;
    
    durationSpinBox.setMinimum(1);
    durationSpinBox.setMaximum(120);
    costSpinBox.setMinimum(0);
    costSpinBox.setMaximum(999999);
    costSpinBox.setDecimals(2);

    form.addRow("Название:", &nameEdit);
    form.addRow("Срок (мес.):", &durationSpinBox);
    form.addRow("Стоимость:", &costSpinBox);

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
        const int duration = durationSpinBox.value();
        const double cost = costSpinBox.value();
        
        if (name.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Название не может быть пустым");
            return;
        }
        warranties.append(new Warranty(name, duration, cost));
        refreshTable();
        emit warrantyListUpdated();
    }
}

void WarrantyWindow::onRemoveWarranty() {
    const int row = table->currentRow();
    if (row < 0 || row >= warranties.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите гарантию для удаления");
        return;
    }

    Warranty* warranty = warranties[row];
    if (QMessageBox::question(this, "Удаление", 
        "Удалить гарантию: " + warranty->getName() + "?") == QMessageBox::Yes) {
        refreshTable();
        emit warrantyListUpdated();
    }
}

void WarrantyWindow::onEditWarranty() {
    const int row = table->currentRow();
    if (row < 0 || row >= warranties.size()) {
        QMessageBox::warning(this, "Ошибка", "Выберите гарантию для редактирования");
        return;
    }

    Warranty* warranty = warranties[row];
    QDialog dialog(this);
    dialog.setWindowTitle("Редактировать гарантию");
    QFormLayout form(&dialog);

    QLineEdit nameEdit(warranty->getName());
    QSpinBox durationSpinBox;
    QDoubleSpinBox costSpinBox;
    
    durationSpinBox.setValue(warranty->getDurationMonths());
    durationSpinBox.setMinimum(1);
    durationSpinBox.setMaximum(120);
    
    costSpinBox.setValue(warranty->getCost());
    costSpinBox.setMinimum(0);
    costSpinBox.setMaximum(999999);
    costSpinBox.setDecimals(2);

    form.addRow("Название:", &nameEdit);
    form.addRow("Срок (мес.):", &durationSpinBox);
    form.addRow("Стоимость:", &costSpinBox);

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
        const int newDuration = durationSpinBox.value();
        const double newCost = costSpinBox.value();
        
        if (newName.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Название не может быть пустым");
            return;
        }

        updateWarranty(warranty, newName, newDuration, newCost);
    }
}

void WarrantyWindow::onSearch(const QString& text) {
    if (text.isEmpty()) {
        for (int i = 0; i < table->rowCount(); i++) {
            table->setRowHidden(i, false);
        }
        return;
    }

    const QString searchText = text.toLower();
    for (int i = 0; i < table->rowCount(); ++i) {
        const Warranty* warranty = warranties[i];
        bool matches = warranty->getName().toLower().contains(searchText) ||
                      QString::number(warranty->getDurationMonths()).contains(searchText) ||
                      QString::number(warranty->getCost()).contains(searchText);
        
        table->setRowHidden(i, !matches);
    }
}

void WarrantyWindow::updateWarranty(Warranty* warranty, const QString& newName, int newDuration, double newCost) {
    if (!warranty) return;
    
    cmdManager.execute(new ChangeWarrantyCommand(warranty, newName, newDuration, newCost));
    refreshTable();
    emit warrantyListUpdated();
}

void WarrantyWindow::onUndo() {
    cmdManager.undo();
    refreshTable();
    emit warrantyListUpdated();
}

void WarrantyWindow::onRedo() {
    cmdManager.redo();
    refreshTable();
    emit warrantyListUpdated();
}
