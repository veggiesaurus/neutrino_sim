#include <iostream>
#include <fstream>
#include <math.h>
#include <stdio.h>

using namespace std;


int main(int argc,char** argv)
{
  ifstream myfile (argv[1]);
  double sumX=0, sumX2=0, sumY=0, sumY2=0, sumZ=0, sumZ2=0, sumTheta=0, sumTheta2=0;
  int N=0;
  float E;
  float boronPercent;
  sscanf(argv[1], "neutron_capture_pos_%fkeV_%fpc.txt",&E, &boronPercent);
  if (myfile.is_open())
  {
    while ( myfile.good() )
    {
      int step;
      double x,y,z;
      myfile>>ws>>step>>ws>>x>>ws>>y>>z;
      myfile.ignore(256, '\n');
      double theta=atan2(x,y);
      sumX+=x;
      sumX2+=x*x;
      sumY+=y;
      sumY2+=y*y;
      sumZ+=z;
      sumZ2+=z*z;
      sumTheta+=theta;
      sumTheta2+=theta*theta;
      N++;      
    }
    double mu_x, mu_y, mu_z, mu_theta;
    double mu_x2, mu_y2, mu_z2, mu_theta2;
    double s_x, s_y, s_z, s_theta;
    if (N==0)
      return 1;
    mu_x=sumX/N;
    mu_y=sumY/N;
    mu_z=sumZ/N;
    mu_theta=sumTheta/N;
    mu_x2=sumX2/N;
    mu_y2=sumY2/N;
    mu_z2=sumZ2/N;
    mu_theta2=sumTheta2/N;
    
    s_x=sqrt(mu_x2-mu_x*mu_x);
    s_y=sqrt(mu_y2-mu_y*mu_y);
    s_z=sqrt(mu_z2-mu_z*mu_z);
    s_theta=sqrt(mu_theta2-mu_theta*mu_theta);
    
    cout<<E<<'\t'<<boronPercent<<'\t'<<mu_x<<'\t'<<s_x<<'\t'<<mu_y<<'\t'<<s_y<<'\t'<<mu_z<<'\t'<<s_z<<'\t'<<mu_theta<<'\t'<<s_theta<<endl;
    
    myfile.close();
    return 0;
    }
}
