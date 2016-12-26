#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Для рисования поверх виджета нужно переопределить метод с именем PaintEvent.
    // Очень важно, чтобы имя и сигнатура метода точно совпадали с теми, что написаны здесь,
    // назвать метод по другому и добавлять аргументы нельзя.

    // Описание метода см. в файле mainwindow.cpp
    virtual void paintEvent(QPaintEvent *);
    void set_polygon(QPolygonF& polygon_);
    void add_points(QPolygonF& points_);
    void set_point(QPointF& pose);
    virtual void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;

    // Свойства объекта, под них выделяется память при создании объекта и они сохраняют
    // свои значения между вызовами различных методов, существуя, пока существует объект.
    // Размещены в блоке private, поэтому доступны только самому объекту из его методов.
    // Для других объектов мы сделаем метод set_params, который позволит изменять x0 и y0
    // извне.

    QPolygonF polygon;
    vector<QPolygonF> points;
    //double x,y;
    QPointF xy;
    vector<QPointF> pose;
    int index = 0;
};

#endif // MAINWINDOW_H
