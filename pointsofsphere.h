#ifndef POINTSOFSPHERE_H
#define POINTSOFSPHERE_H

#include <vector>

class pointsofsphere
{
public:
    pointsofsphere();

    void getpoints(); // расчёт координат заданного количества точек и внесение в coords и pointsof
    std::vector<std::vector> pointsof;
    std::vector<float> coords;
};

#endif // POINTSOFSPHERE_H
