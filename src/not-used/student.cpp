/* Time-stamp: <2013/05/04 22:19:10 hutzpa [hutzpa] am> */

#include <ap.h>
#include <studentttests.h>

extern "C" 
{
  
  void student(double *X, int Nx, double *Y, int Ny, double &both, double &left, double &right)
{
  ap::real_1d_array x, y;
  x.setcontent(0, Nx, X);
  y.setcontent(0, Ny, Y);
  unequalvariancettest(x, Nx, y, Ny, both, left, right);
  // studentttest2(x, Nx, y, Ny, both, left, right);
}
}

    
