#ifndef CLIENT_H
#define CLIENT_H
#include <QtCore/qstring.h>

class Client {
    QString name; // имя клиента
public:
    explicit Client(const QString &name) : name(name) {}

    QString getName() const { return name; }
    void setName(const QString &name) { this->name = name; }
};

#endif //CLIENT_H
