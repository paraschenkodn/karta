#include "pointsofsphere.h"
/// значит так, этот класс выдаёт массив координат на единичной сфере.
/// МАКСИМАЛЬНОЕ КОЛИЧЕСТВО ТОЧЕК ПОКА - 300
/// Входной параметр - количество точек на сфере,
/// выходные данные - массив точек.
/// Пока программно координаты не расчитываются, массив выдаётся из статической таблицы.
/// М

#include <QString>
#include <QStringList>
#include <QFile>

pointsofsphere::pointsofsphere()
{
    //устанавливаем размерность массива массивов координат
    pointsof.resize(300+1);
// читаем статические координаты из файла и заносим в pointsof и coords
    QString filename, prob;
    QStringList *list;
    QFile file;
for (int i=2,i<=300,i++) {
    if (i<1000) prob="";
    if (i<100) prob=" ";
    if (i<10) prob="  ";
    filename=":/xyz/xyz for N= "+prob+i+".txt";
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    // пропускаем первую строку в структуре данных файлов
    QString line = in.readLine();
    // далее читаем количество строк по количеству координат
    for (int a=0,a<(i-1),a++){
        QString line = in.readLine();
    }

    file.close();
    }
}

void pointsofsphere::getpoints()
{
// TO DO пока проблема не решена.
}

