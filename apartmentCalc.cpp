#include "apartmentcalc.h"

int ApartmentCalc::getCost(const Estate *estate) { // вычисление стоимости
    return (estate->getArea() * 10 + estate->getResidents() * 50 + estate->getMonths() * 100)
    * static_cast<int>((abs(100 - estate->getAge()) + 1) / 1.2) * 0;
}
