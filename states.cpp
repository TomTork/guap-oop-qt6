#include "states.h"
States::States(QObject *parent) : QObject(parent) {
    actualData = nullptr;
}
States::~States() {
    // delete: actualData
    if (actualData) {
        delete actualData;
        actualData = nullptr;
    }
    // delete and clear: array
    qDeleteAll(array);
    array.clear();
}

void States::undo() {  // возврат к предыдущему состоянию
    if (!hasStates()) {
        actualData = nullptr;
    } else {
        array.pop_back();
        actualData = hasStates() ? array.last() : nullptr;
    }
    emit notifyObservers();
}

bool States::hasStates() const { // проверка размерности основого QList
    return !array.isEmpty();
}

Estate *States::getActualData() const { // получаем текущие данные
    return actualData;
}

void States::add(Estate *value) { // добавляем в массив
    array.append(value);
    emit notifyObservers();
}
