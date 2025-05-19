#include "widget.h"
#include <iostream>
#include "ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui_Widget), info(this) { // конструктор
    ui->setupUi(this);
    ui->btnUndo->setEnabled(false);

    // декларация сигналов для отслеживания изменений
    connect(&info, SIGNAL(notifyObservers()), this, SLOT(update()));
    connect(ui->btnCalc, SIGNAL(pressed()), this, SLOT(btnCalcPressed()));
    connect(ui->btnUndo, SIGNAL(pressed()), this, SLOT(btnUndoPressed()));
}
Widget::~Widget() { // деструктор
    delete ui;
}
// public slots
void Widget::update() const { // обновление формы по нажатию на кнопку
    if (const auto value = info.getActualData(); value != nullptr) {
        fillForm(value);
    }
    ui->btnUndo->setEnabled(info.hasStates());
}
// private slots
void Widget::btnCalcPressed() { // логика нажатия вычисления
    const auto value = processForm();
    showCost(value);
    info.add(value);
    ui->btnUndo->setEnabled(true);
}
void Widget::btnUndoPressed() { // логика возврата к предыдущей записи в истории
    info.undo();
    const auto value = processForm(); // вычисление предыдущей стоимости
    showCost(value);
}

// private
Estate *Widget::processForm() const { // вычисление формы
    const int age = ui->age->text().toInt();
    const int area = ui->area->text().toInt();
    const int residents = ui->residents->text().toInt();
    const QString owner = ui->owner->text();

    Estate::EstateType type;
    switch (ui->estateType->currentIndex()) { // нахождение выбранного индекса и сопоставление с типом
        case 0:
            type = Estate::ECONOM;
            break;
        case 1:
            type = Estate::LUXURIOUS;
            break;
        case 2:
            type = Estate::TOWN_HOUSE;
            break;
        case 3:
            type = Estate::COTTAGE;
            break;
        default:
            return nullptr;
    }

    int months;
    switch (ui->period->currentIndex()) { // нахождение выбранного индекса и сопоставление со значением
        case 0:
            months = 6;
            break;
        case 1:
            months = 12;
            break;
        case 2:
            months = 18;
            break;
        default:
            return nullptr;
    }

    auto *value = new Estate(age, area, residents, months, type, owner); // создание новой записи
    const int cost = CalculationFacade::getCost(value); // вычисление новой стоимости
    ui->costLabel->setText("");
    ui->costLabel->setText("Стоимость страхового взноса: " + QString::number(cost));
    return value;
}


void Widget::fillForm(const Estate *value) const { // обработка заполения формы из объекта Estate
    if (!value) return;

    ui->age->setText(QString::number(value->getAge()));
    ui->area->setText(QString::number(value->getArea()));
    ui->residents->setText(QString::number(value->getResidents()));
    ui->owner->setText(value->getOwner());

    switch (value->getType()) {
        case Estate::ECONOM:
            ui->estateType->setCurrentIndex(0);
            break;
        case Estate::LUXURIOUS:
            ui->estateType->setCurrentIndex(1);
            break;
        case Estate::TOWN_HOUSE:
            ui->estateType->setCurrentIndex(2);
            break;
        case Estate::COTTAGE:
            ui->estateType->setCurrentIndex(3);
            break;
    }

    switch (value->getMonths()) {
        case 6:
            ui->period->setCurrentIndex(0);
            break;
        case 12:
            ui->period->setCurrentIndex(1);
            break;
        case 18:
            ui->period->setCurrentIndex(2);
            break;
        default:
            break;
    }
}

void Widget::showCost(const Estate *value) const { // обновить стоимость
    if (!value) return;

    const int cost = CalculationFacade::getCost(value);
    ui->costLabel->setText("Стоимость страхового взноса: " + QString::number(cost));
}
