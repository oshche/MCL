#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <time.h>
#include <stdlib.h>

using namespace std;

int const N=9;
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
   char *name="map.txt";
   cout<<name<<"\n";
   F.open(name);
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
  vector<Wall> wall=Map_open();
  vector<Particle> part=Particle_initial_pose(wall);
}














