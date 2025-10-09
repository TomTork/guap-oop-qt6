#ifndef EQUIPMENT_H
#define EQUIPMENT_H
#include <QString>

class Equipment {
    QString name; // название оборудования
public:
    explicit Equipment(const QString &name) : name(name) {}

    QString getName() const { return name; }
    void setName(const QString &name) { this->name = name; }
};

#endif //EQUIPMENT_H
