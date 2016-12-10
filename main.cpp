#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <QPoint>
#include <QPolygon>
using namespace std;

//int const N=9;
int Particle_count=10;
//double dt=0,1;
void drand48();

double Length(double x1,double y1,double x2, double y2)
{
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

class Wall
{
    public: double x1,y1,x2,y2;
            double l;
};

class Particle
{   public: double x,y,a,w;//a - óãîë;w - weight

};


vector<Wall>  Map_open()

{  int n=0;
   double x1_,x2_,y1_,y2_;
   fstream F;
   //char *name="map.txt";
   cout<<"map.txt"<<"\n";
   //F.open(name);

   F.open("C:\\Qt\\Qt5.2.1\\Tools\\QtCreator\\bin\\untitled2\\map.txt");
   if (F)
     { vector<Wall> wall;
       Wall w;
       while (!F.eof())
         {
           F>>x1_>>y1_>>x2_>>y2_;

           w.x1=x1_;
           w.y1=y1_;
           w.x2=x2_;
           w.y2=y2_;
           w.l=Length(x1_,y1_,x2_,y2_);
           wall.push_back(w);

           cout<<"("<<wall[n].x1<<","<<wall[n].y1<<")-("<<wall[n].x2<<","<<wall[n].y2<<")"<<" length: "<<wall[n].l<<endl;

           n++;
         }


       F.close();
       cout<<"Number of sections: "<<n<<endl;
       return wall;
       }
       else cout<<"Can't find"<<endl;

}


QPolygonF  Map_open2()

{  int n=0;
   double x1_,x2_,y1_,y2_;
   fstream F;
   //char *name="map2.txt";
   cout<<"map2.txt"<<"\n";
   //F.open(name);

   F.open("C:\\Qt\\Qt5.2.1\\Tools\\QtCreator\\bin\\untitled2\\map2.txt");
   if (F)
     { QPolygonF room(1);
       room.pop_back();
       QPointF r;
       F>>x1_>>y1_;
       do
         {
           r.setX(x1_);
           r.setY(y1_);
           //w.x2=x2_;
           //w.y2=y2_;
           //w.l=Length(x1_,y1_,x2_,y2_);
           room.push_back(r);

           cout<<n<<"  - "<<room[n].rx()<<","<<room[n].ry()<<endl;

           n++;
           F>>x1_>>y1_;

         }
       while (!F.eof());
      //  r.setX(x2_);
       // r.setY(y2_);
       // room.push_back(r);
        //cout<<room[n].rx()<<","<<room[n].ry()<<endl;

       F.close();
       cout<<"Number of sections: "<<n<<endl;
       return room;
       }
       else cout<<"Can't find"<<endl;
}






Particle Random_pose(vector<Wall>& wall,int i)
{ //srand(time(NULL));
  int n=rand()%9;
  Particle p;
  p.x=(double)rand()/(double) RAND_MAX*(wall[n].x1-wall[n].x2)+wall[n].x2;
  p.y=(double)rand()/(double) RAND_MAX*(wall[n].y1-wall[n].y2)+wall[n].y2;
  return p;
}

vector<Particle> Particle_initial_pose(vector<Wall>& wall)
{int i;
 vector<Particle> part;
 cout<<endl<<"Particles initial pose"<<endl;
 for (i=0;i<Particle_count;i++)
   {part.push_back(Random_pose(wall,i));
     cout<<"("<<part[i].x<<" , "<<part[i].y<<")"<<endl;
   }
 return part;
}




QPointF Random_pose2(QPolygonF& room,int i)
{ //srand(time(NULL));
  //int n=rand()%9;
  QPointF p;
  p.setX((double)rand()/(double) RAND_MAX*12);
  p.setY((double)rand()/(double) RAND_MAX*12);
  if (room.containsPoint(p, Qt::OddEvenFill))
  {cout<<"ura"<<endl;
   return p;
  }
  p=Random_pose2(room,i);
  return p;
}

vector<QPointF> Particle_initial_pose2(QPolygonF& room)
{int i;
 vector<QPointF> part;
 cout<<endl<<"Particles initial pose"<<endl;
 for (i=0;i<Particle_count;i++)
   {part.push_back(Random_pose2(room,i));
     cout<<part[i].rx()<<" , "<<part[i].ry()<<")"<<endl;
   }
 return part;
}








void Particle_move(double dx,double dy,int Particle_count,vector<Particle>& part, vector<Wall>& wall )
{ int i;
  double d_x,d_y;
  for (i=0;i<Particle_count;i++)
    {d_x=dx*cos(part[i].a)+dy*sin(part[i].a);
     d_y=dy*cos(part[i].a)+dx*cos(part[i].a);
     part[i].x+=d_x;
     part[i].y+=d_y;
     //part[i].a+=
    }
}


int main()
{
 // vector<Wall> wall=Map_open();
  //vector<Particle> part=Particle_initial_pose(wall);
 /* QPolygon polygon(1);
          polygon.push_back(QPoint(4, 5));
          polygon.putPoints(1, 2, 6,7, 8,9);
          cout<<polygon[0].rx()<<endl;
   QPointF k;
   k.setX(4.8);
   k.setY(7);
   cout<<k.rx()<<" "<<k.ry()<<endl;
   QPolygonF k1(1);
   k1.pop_back();
   k1.push_back(k);
   cout<<k1[0].rx()<<" "<<k1[0].ry()<<endl;*/

   QPolygonF room=Map_open2();
   vector<QPointF> part=Particle_initial_pose2(room);





   return 0;
}














