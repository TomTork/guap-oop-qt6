#ifndef ESTATE_H
#define ESTATE_H

#include <QObject>
#include <QString>

class Estate final : public QObject {
    Q_OBJECT
public:
    enum EstateType {
        ECONOM,
        LUXURIOUS,
        TOWN_HOUSE,
        COTTAGE
    };

    explicit Estate(int age, int area, int residents, int months, EstateType type, QString owner, QObject *parent = nullptr);

    [[nodiscard]] int getAge() const;
    [[nodiscard]] int getArea() const;
    [[nodiscard]] int getResidents() const;
    [[nodiscard]] int getMonths() const;
    [[nodiscard]] EstateType getType() const;
    [[nodiscard]] QString getOwner() const;

private:
    int age;
    int area;
    int residents;
    int months;
    EstateType type;
    QString owner;
};

#endif // ESTATE_H
