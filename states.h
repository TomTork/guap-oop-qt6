#ifndef STATES_H
#define STATES_H

#include <QObject>
#include "estate.h"

class States final : public QObject
{
    Q_OBJECT
    public:
    explicit States(QObject *parent = nullptr);
    ~States() override;
    void undo();
    [[nodiscard]] bool hasStates() const;
    [[nodiscard]] Estate *getActualData() const;
    void add(Estate *value);

    signals:
    void notifyObservers();
private:
    QList<Estate *> array;
    Estate *actualData;
};

#endif // STATES_H
