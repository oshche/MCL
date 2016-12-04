#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <time.h>
#include <stdlib.h>
using namespace std;
int const N=9;
int Particle_count=1200;
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
{   public: double x,y,a,w;//a - угол;w - weight

};




void Map_open()

{  int n=0;
   double x1_,x2_,y1_,y2_;
   fstream F;
   char *name="map.txt";
   cout<<name<<"\n";
   F.open(name);
   if (F)
     { vector<Wall> wall(N);
      while (!F.eof())
         {
           F>>x1_>>y1_>>x2_>>y2_;

           wall[n].x1=x1_;
           wall[n].y1=y1_;
           wall[n].x2=x2_;
           wall[n].y2=y2_;
           wall[n].l=Length(x1_,y1_,x2_,y2_);


           cout<<"("<<wall[n].x1<<","<<wall[n].y1<<")-("<<wall[n].x2<<","<<wall[n].y2<<")"<<" length: "<<wall[n].l<<endl;

           n++;
         }
      F.close();
      cout<<"Number of sections: "<<n<<endl;



     }
}

void Random_pose(vector<Wall>& wall,vector<Particle>& part,int i)
{ srand(time(NULL));
  int n=rand()%9;
  part[i].x=(double)rand()/(double) RAND_MAX*(wall[n].x1-wall[n].x2)+wall[n].x2;
  part[i].y=(double)rand()/(double) RAND_MAX*(wall[n].y1-wall[n].y2)+wall[n].y2;
}


void Particale_initial_pose(vector<Particle>& part,vector<Wall>* wall)
{int i;
 for (i=0;i<part.size();i++)
   {part[i]=Random_pose(wall,part,i);
   }
}




void Particle_move(double dx,double dy,int Particle_count,vector<Particle>& part )
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
    Map_open();


}










