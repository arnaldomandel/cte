#include <math.h>

typedef struct datapoint 
{
    double val, vlow, vhigh;
}  datapoint;

datapoint(*mid_and_errors)(double *v, int n);

double avg(double *v, int n);
double var(double *v, int n);
double stddev(double *v, int n);
datapoint med_and_quart(double *v, int n);
datapoint av_and_dev(double *v, int n);
double regress(double *y, int n);
void full_regress(double *y, int n, double *beta, double *dev);
//void full_regress(double *y, int n, double *beta, double *pvalue, double *tscore);
//void full_regress(double *y, int n, double conf, double *beta, double *dev, double *t);
double tvalue(double conf, int df);
double p_value(double t, int df);
