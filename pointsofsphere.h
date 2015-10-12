#ifndef POINTSOFSPHERE_H
#define POINTSOFSPHERE_H

#include <vector>

class pointsofsphere
{
public:
    pointsofsphere();
    pointsofsphere(int);
    ~pointsofsphere();

    void getpoints(); // расчёт координат заданного количества точек и внесение в coords и pointsof
    std::vector< std::vector<float> > pointsof;
    int size;
protected:
    void init();
};

#endif // POINTSOFSPHERE_H
