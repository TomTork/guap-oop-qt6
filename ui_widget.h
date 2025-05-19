#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

// класс для создания интерфейса
class Ui_Widget {
public:
    QLabel *ownerLabel;
    QLineEdit *owner;
    QLabel *ageLabel;
    QLineEdit *age;
    QLabel *residentsLabel;
    QLineEdit *residents;
    QLabel *areaLabel;
    QLineEdit *area;
    QLabel *costLabel;
    QPushButton *btnCalc;
    QPushButton *btnUndo;
    QLabel *estateTypeLabel;
    QComboBox *estateType;
    QLabel *periodLabel;
    QComboBox *period;

    void setupUi(QWidget *Widget) {
        Widget->setWindowTitle("Саиткулов Дмитрий 4332 — Расчёт страховки");
        ownerLabel = new QLabel("Имя:", Widget);
        ownerLabel->setGeometry(20, 20, 190, 30);
        owner = new QLineEdit(Widget);
        owner->setGeometry(200, 20, 200, 30);

        ageLabel = new QLabel("Возраст:", Widget);
        ageLabel->setGeometry(20, 60, 190, 30);
        age = new QLineEdit(Widget);
        age->setGeometry(200, 60, 200, 30);

        estateTypeLabel = new QLabel("Класс жилья:", Widget);
        estateTypeLabel->setGeometry(20, 100, 190, 30);
        estateType = new QComboBox(Widget);
        estateType->setGeometry(200, 100, 200, 30);
        estateType->addItem("Квартира эконом-класса");
        estateType->addItem("Элитная квартира");
        estateType->addItem("Таун-хаус");
        estateType->addItem("Коттедж");

        residentsLabel = new QLabel("Число проживающих:", Widget);
        residentsLabel->setGeometry(20, 140, 190, 30);
        residents = new QLineEdit(Widget);
        residents->setGeometry(200, 140, 200, 30);

        areaLabel = new QLabel("Площадь:", Widget);
        areaLabel->setGeometry(20, 180, 190, 30);
        area = new QLineEdit(Widget);
        area->setGeometry(200, 180, 200, 30);

        periodLabel = new QLabel("Срок страхования:", Widget);
        periodLabel->setGeometry(20, 220, 190, 30);
        period = new QComboBox(Widget);
        period->setGeometry(200, 220, 200, 30);
        period->addItem("6 месяцев");
        period->addItem("1 год");
        period->addItem("18 месяцев");

        costLabel = new QLabel("Стоимость страхового взноса: 0", Widget);
        costLabel->setGeometry(20, 270, 300, 30);

        btnCalc = new QPushButton("Рассчитать", Widget);
        btnCalc->setGeometry(50, 320, 100, 30);

        btnUndo = new QPushButton("Последний запрос", Widget);
        btnUndo->setGeometry(200, 320, 150, 30);
    }
};

#endif // UI_WIDGET_H