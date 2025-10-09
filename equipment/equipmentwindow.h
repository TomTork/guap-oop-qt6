#ifndef EQUIPMENTWINDOW_H
#define EQUIPMENTWINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>

#include "equipment.h"
#include "../command/commandmanager.h"

class EquipmentWindow final : public QWidget {
    Q_OBJECT

    QTableWidget* table;
    QPushButton *addButton, *removeButton, *editButton, *undoButton, *redoButton;
    CommandManager cmdManager;

public:
    QList<Equipment*> equipments;
    explicit EquipmentWindow(QWidget* parent = nullptr);

    private slots:
        void refreshTable();
    void onAddEquipment();
    void onRemoveEquipment();
    void onEditEquipment();
    void onUndo();
    void onRedo();
    void onSearch(const QString& text);
    signals:
    void equipmentListUpdated();
};

#endif //EQUIPMENTWINDOW_H
