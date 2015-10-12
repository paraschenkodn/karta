#include "pointsofsphere.h"
/// значит так, этот класс выдаёт массив координат на единичной сфере.
/// МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ТОЧЕК ПОКА - 300
/// Входной параметр - количество точек на сфере,
/// выходные данные - массив точек.
/// Пока программно координаты не расчитываются, массив выдаётся из статической таблицы.
///
/// прим: Реализация через динамический массив была бы быстрее на 20% чем через vector

#include <QString>
#include <QFile>
#include <QTextStream>

pointsofsphere::pointsofsphere()
{
    size=300;
    init();
}

pointsofsphere::pointsofsphere(int initsize){
    size=initsize;
    init();
}

pointsofsphere::~pointsofsphere()
{

}

void pointsofsphere::getpoints()
{
    // TO DO пока проблема не решена.
}

void pointsofsphere::init()
{
    //устанавливаем размерность массива массивов координат
    pointsof.resize(size+1);
    // читаем статические координаты из файла и заносим в pointsof и coords
    QString filename, prob, line;
    QFile file;
    // 0 и 1 индексы содержат нулевые координаты
    pointsof[0].resize(3);
    pointsof[0][0]=0.0f;
    pointsof[0][1]=0.0f;
    pointsof[0][2]=0.0f;
    pointsof[1].resize(3);
    pointsof[1][0]=0.0f;
    pointsof[1][1]=0.0f;
    pointsof[1][2]=0.0f;
    for (int i=2;i<=300;i++) {
        if (i<1000) prob="";
        if (i<100) prob=" ";
        if (i<10) prob="  ";
        filename.setNum(i);
        filename=":/xyz/xyz for N= "+prob+filename+".txt";
        file.setFileName(filename);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);

        // инициализируем массив координат, умножая i на 3 (три координаты на точку)
        pointsof[i].resize(i*3);
        // пропускаем первую строку в структуре данных файлов
        line = in.readLine();
        // далее читаем количество строк по количеству координат
        for (int a=0;a<(i*3-1);a++){
            line = in.readLine();
            // старт 8, значение 18, промежуток 3
            pointsof[i][a]=line.mid(8,18).toFloat();
            pointsof[i][a+1]=line.mid(29,18).toFloat();
            pointsof[i][a+2]=line.mid(50,18).toFloat();
            a+=2;
        }
        file.close();
    }
}

