#ifndef REPAIRFACTORY_H
#define REPAIRFACTORY_H

#include <QString>

class RepairType {
public:
    virtual QString process(const QString& phoneModel) = 0;
    virtual int cost() = 0;
    virtual ~RepairType() = default;
};

class ScreenRepair final : public RepairType {
public:
    QString process(const QString& phoneModel) override {
        return "Заменён экран на: " + phoneModel;
    }
    int cost() override {
        return 100;
    }
};

class BatteryRepair final : public RepairType {
public:
    QString process(const QString& phoneModel) override {
        return "Заменён аккумулятор на: " + phoneModel;
    }
    int cost() override {
        return 200;
    }
};

class CameraRepair final : public RepairType {
public:
    QString process(const QString& phoneModel) override {
        return "Заменена камера на: " + phoneModel;
    }
    int cost() override {
        return 300;
    }
};

class ButtonsRepair final : public RepairType {
public:
    QString process(const QString& phoneModel) override {
        return "Заменены кнопки на: " + phoneModel;
    }
    int cost() override {
        return 50;
    }
};

class RepairFactory {
public:
    enum Type { Screen, Battery, Camera, Buttons };

    static RepairType* create(const Type type) {
        switch (type) {
            case Screen: return new ScreenRepair();
            case Battery: return new BatteryRepair();
            case Camera: return new CameraRepair();
            case Buttons: return new ButtonsRepair();
        }
        return nullptr;
    }
};

#endif // REPAIRFACTORY_H
