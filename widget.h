#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "states.h"
#include "estate.h"
#include "calculationFacade.h"
#include "Ui_Widget.h"

class Widget final : public QWidget {
    Q_OBJECT
    public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
    public slots:
    void update() const;
    private slots:
    void btnCalcPressed();
    void btnUndoPressed();
private:
    Estate *processForm() const;
    void fillForm(const Estate *value) const;
    void showCost(const Estate *value) const;
private:
    Ui_Widget *ui;
    States info;
};

#endif //WIDGET_H
