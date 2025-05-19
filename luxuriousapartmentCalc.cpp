#include "luxuriousapartmentcalc.h"

int LuxuriousApartmentCalc::getCost(const Estate *estate) { // вычисление стоимости
    return (estate->getArea() * 30 + estate->getResidents() * 150 + estate->getMonths() * 100) * static_cast<int>((abs(100 - estate->getMonths()) + 1) / 1.2);
}
