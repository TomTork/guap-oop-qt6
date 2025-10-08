#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>

#include "client.h"
#include "../command/commandmanager.h"
#include "client.h"
#include "../command/changeclientcommand.h"

class ClientWindow final : public QWidget {
    Q_OBJECT

    QTableWidget* table;
    QPushButton *addButton, *removeButton, *editButton, *undoButton, *redoButton;
    CommandManager cmdManager;

public:
    QList<Client*> clients;
    explicit ClientWindow(QWidget* parent = nullptr);

    private slots:
        void refreshTable();
        void onAddClient();
        void onRemoveClient();
        void onEditClient();
        void onUndo();
        void onRedo();
        void onSearch(const QString& text);
    signals:
    void clientListUpdated();
};

#endif // CLIENTWINDOW_H
