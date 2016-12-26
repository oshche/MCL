#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
     QPainter p(this);
     //QPainter q(this);
   QPen pen(Qt::black,0.1,Qt::SolidLine);
   p.setPen(pen);
   // QPolygonF polygon;   // Используем класс полигона, чтобы отрисовать треугольник
                  // Помещаем координаты точек в полигональную модель
    //polygon << QPointF(40,40) << QPointF(120,40) << QPointF(120,120)<< QPointF(40,120);
   p.translate(450,425);
   p.scale(30, 30);
   //q.translate(350,350);
   //q.scale(20, 20);

     p.drawPolygon(polygon);
     QPen pen1(Qt::darkGreen,0.1,Qt::SolidLine);
     p.setPen(pen1);
     p.drawPoints(points[index % points.size()]);
     QPen pen2(Qt::red,0.3,Qt::SolidLine);
     p.setPen(pen2);

     p.drawPoint(pose[index % pose.size()]);
    // p.drawPoint(QPointF(x,y));

     cout << "index" << index << endl;

}


void MainWindow::set_polygon(QPolygonF& polygon_)
{
    polygon=polygon_;

}


void MainWindow::add_points(QPolygonF& points_)
{
    points.push_back(points_);

}


void MainWindow::set_point(QPointF& pose_)
{
    pose.push_back(pose_);
    //x=x_;
    //y=y_;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    ++index;
    update();
}
