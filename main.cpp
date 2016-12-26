#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <QPoint>
#include <QPolygon>
#include <QApplication>
#include "mainwindow.h"
#define _USE_MATH_DEFINES
using namespace std;

int Particle_count=500;


double Length(double x1,double y1,double x2, double y2)
{
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}
double Const_k(double x1,double y1,double x2,double y2)
{   if (y1!=y2&&x1!=x2) return (y2-y1)/(x2-x1);
    return 0;
}

double Const_c(double x1,double y1,double x2,double y2,double k)
{   if (y1!=y2&&x1!=x2) return (y1-x1*k);
    return 0;
}

class Wall
{
    public: double x1,y1,x2,y2;
            double l,c,k;
};

class Particle
{   public:
    double x,y,a;
    double s[8];
};

class Delta
{    public:
     int n;
     double e;
};


void Map_open(QPolygonF& room, vector<Wall>& wall)
{
   int n=0;
   double x1_,x2_,y1_,y2_;
   fstream F;
   F.open("C:/Users/Olga/Documents/localization3/map4.txt");
   if (F)
      {
       Wall w;
       QPointF r;
       F>>x1_>>y1_>>x2_>>y2_;
       do
         {
           w.x1=x1_;
           w.y1=y1_;
           w.x2=x2_;
           w.y2=y2_;
           w.l=Length(x1_,y1_,x2_,y2_);
           w.k=Const_k(x1_,y1_,x2_,y2_);
           w.c=Const_c(x1_,y1_,x2_,y2_,w.k);
           wall.push_back(w);

           r.setX(x1_);
           r.setY(y1_);
           room.push_back(r);

           n++;
           F>>x1_>>y1_>>x2_>>y2_;
         }
         while (!F.eof());

         F.close();
      }
   else cout<<"Can't find file"<<endl;

}



QPointF Random_pose2(QPolygonF& room)
{ QPointF p;
  do {
       p.setX(pow(-1,rand()%2)*(double)rand()/(double) RAND_MAX*12);
       p.setY(pow(-1,rand()%2)*(double)rand()/(double) RAND_MAX*12);
     }
  while (!room.containsPoint(p, Qt::OddEvenFill));
  return p;
}


double Sensor(vector<Particle>& rob,QPolygonF& room, vector<Wall>& wall,int n,int i)
{int j;
 double k,c,x,y,a;
 a=(i*45+rob[n].a);
 if (a>=360) a=a-360;
 k=1/(tan(a*M_PI/180));
 c=rob[n].y-k*rob[n].x;

 for (j=0;j<wall.size();j++)
    {

     if (a<180&&wall[j].x1<rob[n].x&&wall[j].x2<rob[n].x) continue;
     if (a>=180&&wall[j].x1>rob[n].x&&wall[j].x2>rob[n].x) continue;

     if (wall[j].x1==wall[j].x2)
           {
             x=wall[j].x1;
             y=k*x+c;
             if ((y<=wall[j].y1 && y>=wall[j].y2) || (y<=wall[j].y2 && y>=wall[j].y1)) break;
             else continue;
           }
      else if (wall[j].y1==wall[j].y2)
           {
             y=wall[j].y1;
             x=(y-c)/k;
             if ((x<=wall[j].x2 && x>=wall[j].x1) || (x<=wall[j].x1 && x>=wall[j].x2)) break;
            else continue;
           }
     else
          {x=(c-wall[j].c)/(wall[j].k-k);
           y=k*x+c;
           if ((y<=wall[j].y1&&y>=wall[j].y2) || (y<=wall[j].y2&&y>=wall[j].y1))
                      {
                       if ((x<=wall[j].x1&&x>=wall[j].x2) || (x<=wall[j].x2&&x>=wall[j].x1)) break;
                      }
           else continue;
          }
    }
 return Length(x,y,rob[n].x,rob[n].y);

}


vector<Particle> Robot_initial_pose(QPolygonF& room,vector<Wall>& wall)
{int i,n;
 vector<Particle> rob(1);
 QPointF r=Random_pose2(room);
 rob[0].x=r.rx();
 rob[0].y=r.ry();
 rob[0].a=(double)(rand()%360);
 n=0;
 for (i=0;i<8;i++)
     rob[0].s[i]=Sensor(rob,room,wall,n,i);
 return rob;
}


 void Sort_bubble(vector<Delta>& E,int Particle_count)
{
 int i,j;
 Delta t;

 for (i = 0; i < Particle_count; i++)
 {
    for (j =Particle_count - 1; j > i; j--)
    {
       if (E[j-1].e > E[j].e)
       {
          t.e=E[j-1].e;
          t.n=E[j-1].n;
          E[j-1].e=E[j].e;
          E[j-1].n=E[j].n ;
          E[j].e = t.e;
          E[j].n=t.n;
       }
    }
 }
}




int Filter(vector<Particle>& part,int Particle_count,vector<Particle>& rob,Delta& E_min,int& k,int& count)
{int i,j;
 vector<Delta> E(Particle_count);
 for(i=0;i<Particle_count;i++)
 {
     E[i].n=i;
     E[i].e=0;
 }
 for(i=0;i<Particle_count;i++)
     for(j=0;j<8;j++)
         E[i].e+=pow(rob[k].s[j]-part[i].s[j],2);

 Sort_bubble(E,Particle_count);
 if (Particle_count>99)
              count=Particle_count*1/100;
 else count=1;
 int s=0;
 for(i=0;i<count;i++)
   {
       j=E.size()-1-i;
       if(E[j].n<E[0].n) E[0].n=E[0].n-1;
       if (E[j].n-s==0)
       {
           part.erase(part.begin()+E[j].n);
           s++;
       }
       else if (s!=0) part.erase(part.begin()+E[j].n-s);
       else part.erase(part.begin()+E[j].n);
   }

 Particle_count=Particle_count-count;
 E_min.e=E[0].e;
 E_min.n=E[0].n;

 return Particle_count;
}




vector<Particle> Particle_initial_pose2(QPolygonF& room,vector<Wall> wall,vector<Particle> rob,QPolygonF &points)
{int i,j;
 vector<Particle> part;
 Particle one_particle;
 QPointF p;
 for (i=0;i<Particle_count;i++)
   {
    p=Random_pose2(room);
    one_particle.x=p.rx();
    one_particle.y=p.ry();
    one_particle.a=rob[0].a;
    part.push_back(one_particle);
    points.push_back(p);

    for(j=0;j<8;j++)
       part[i].s[j]=Sensor(part,room,wall,i,j);
   }
 return part;
}


void Robot_move(vector<Particle>& rob,QPolygonF& room,vector<Wall> wall,int n,int Particle_count)
{int i,max;
 double dx,dy;
 Particle r;
 QPointF p;
 max=0;
 for (i=1;i<8;i++)
 {if ( rob[n-1].s[i]>rob[n-1].s[max] ) max=i;
 }
 r.a=rob[n-1].a+45*max;
 if (r.a>=360) r.a=r.a-360;
 dx=sin(r.a*M_PI/180)*rob[n-1].s[max]/Particle_count;
 dy=cos(r.a*M_PI/180)*rob[n-1].s[max]/Particle_count;
 p.setX(rob[n-1].x+dx);
 p.setY(rob[n-1].y+dy);
 while (!room.containsPoint(p, Qt::OddEvenFill))
   {
     dx=dx/2;
     dy=dy/2;
     p.setX(rob[n-1].x+dx);
     p.setY(rob[n-1].y+dy);
   }
 r.x=rob[n-1].x+dx;
 r.y=rob[n-1].y+dy;
 rob.push_back(r);
 for (i=0;i<8;i++)
     rob[n].s[i]=Sensor(rob,room,wall,n,i);
 cout<<n<<" - Step: "<<endl<<"("<<rob[n].x<<","<<rob[n].y<<")"<<endl;
}


void Particle_move(vector<Particle>& part,vector<Particle>& rob,QPolygonF& room,vector<Wall>& wall,int Particle_count,int n )
{ int i,j;
  double d_x,d_y,dx,dy;
  double fi;
  dx=rob[n].x-rob[n-1].x;
  dy=rob[n].y-rob[n-1].y;
  QPointF p;
  for (i=0;i<Particle_count;i++)
    { fi=atan((rob[n-1].x-part[i].x)/(rob[n-1].y-part[i].y));
      int max=0;
      double aa,bb;
      bb=fabs(part[i].s[max]-rob[n-1].s[max]);
      for (j=1;j<8;j++)
         { aa=fabs(-rob[n-1].s[j]+part[i].s[j]);
             if (aa < bb )
                 {max=j;
                  bb=fabs(part[i].s[max]-rob[n-1].s[max]);
                 }
         }
      part[i].a=part[i].a+45*max;
      if (part[i].a>=360) part[i].a=part[i].a-360;
      d_x=(part[i].s[max]-rob[n-1].s[max])*sin(fi);
      d_y=(part[i].s[max]-rob[n-1].s[max])*cos(fi);
      p.setX(part[i].x+d_x);
      p.setY(part[i].y+d_y);
      while (!room.containsPoint(p, Qt::OddEvenFill))
         {
           d_x=d_x/2;
           d_y=d_y/2;
           p.setX(part[i].x+d_x);
           p.setY(part[i].y+d_y);
         }
     part[i].x+=d_x;
     part[i].y+=d_y;
     for(j=0;j<8;j++)
            part[i].s[j]=Sensor(part,room,wall,i,j);
     }
 }

void mcl(vector<Particle>& rob,vector<Particle>& part,QPolygonF & room,
         vector<Wall> wall, MainWindow & w)
{   int n=0;
    double eps=0.01;
    int count;
    Delta E_min;
    Particle_count=Filter(part,Particle_count,rob,E_min,n,count);
    do
    {
       n++;
       Robot_move(rob,room,wall,n,Particle_count);
       Particle_move(part,rob,room,wall,Particle_count,n);
       Particle_count=Filter(part,Particle_count,rob,E_min,n,count);
       QPolygonF poly;
       for (int i = 0; i < part.size(); ++i) {
           poly << QPointF(part[i].x, part[i].y);
       }
       w.add_points(poly);
       QPointF point;
       point.setX(rob[n].x);
       point.setY(rob[n].y);
       w.set_point(point);
    }
    while((E_min.e>eps)&&(Particle_count!=0));
    cout<<"Result ( "<<part[E_min.n].x<<" , "<<part[E_min.n].y<<" ) "<<endl;
    int j;
    double s=0;
    for(j=0;j<8;j++)
        s+=pow(rob[n].s[j]-part[E_min.n].s[j],2);
}


 int main(int argc, char *argv[])
 {
    srand( time( NULL ) );
    QApplication a(argc, argv);
    QPolygonF room;
    vector<Wall> wall;
    Map_open(room,wall);
    QPolygonF points;
    vector<Particle> rob=Robot_initial_pose(room,wall);
    vector<Particle> part=Particle_initial_pose2(room,wall,rob,points);
    MainWindow w;
    w.set_polygon(room);
    w.add_points(points);
    QPointF point;
    point.setX(rob[0].x);
    point.setY(rob[0].y);
    w.set_point(point);
    w.show();
    mcl(rob,part,room,wall,w);

    return a.exec();
  }

