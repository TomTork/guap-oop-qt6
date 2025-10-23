#ifndef WARRANTYWINDOW_H
#define WARRANTYWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QList>
#include "warranty.h"
#include "../command/commandmanager.h"
#include "../command/changewarrantycommand.h"

class WarrantyWindow final : public QWidget {
    Q_OBJECT

    QTableWidget* table;
    QPushButton *addButton, *removeButton, *editButton, *undoButton, *redoButton;
    QList<Warranty*> warranties;
    CommandManager cmdManager;
    
    void refreshTable();
    
private slots:
    void onAddWarranty();
    void onRemoveWarranty();
    void onEditWarranty();
    void onSearch(const QString& text);
    void onUndo();
    void onRedo();
    
    void updateWarranty(Warranty* warranty, const QString& newName, int newDuration, double newCost);
    
public:
    explicit WarrantyWindow(QWidget* parent = nullptr);
    ~WarrantyWindow() override;
    
    const QList<Warranty*>& getWarranties() const { return warranties; }
    
signals:
    void warrantyListUpdated();
};

#endif // WARRANTYWINDOW_H
