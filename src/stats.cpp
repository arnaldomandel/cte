#include <stdlib.h>
//#include "stats.h"
#include <studenttdistr.h>
//double studenttdistribution(int k, double t);
//#include <gsl/gsl_cdf.h>
extern "C" 
{
typedef struct datapoint 
{
    double val, vlow, vhigh;
}  datapoint;
datapoint(*mid_and_errors)(double *v, int n);

static int compar_abs(const void *x, const void *y)
{
    return fabs(*(double *) x) - fabs(*(double *) y);
}


double avg(double *v, int n)
{
    double a = 0;
    /* sort by absolute value to avoid stupid numerical errors */
    qsort(v, n, sizeof(double), compar_abs);
    for (int i = 0; i < n; i++)
	a += v[i];
    return a / n;
}
double var(double *v, int n)
{
    double a = 0, a2 = 0;
    /* sort by absolute value to avoid stupid numerical errors */
    qsort(v, n, sizeof(double), compar_abs);
    for (int i = 0; i < n; i++) {
	a += v[i];
	a2 += v[i]*v[i];
    }
    a /= n;
    a2 /= n;
    
    return a2 - a * a;
}

double stddev(double *v, int n)
{
    return sqrt(var(v, n));
}

datapoint med_and_quart(double *v, int n)
{
    datapoint p;

    /* sort by absolute value to avoid stupid numerical errors */
    qsort(v, n, sizeof(double), compar_abs);
    
    p.val = v[n/2];
    
    p.vlow = v[n/4];
    
    p.vhigh = v[3*n/4];
    
    return p;
}

datapoint av_and_dev(double *v, int n)
{
    double a = 0, d = 0;
    datapoint p;

    /* sort by absolute value to avoid stupid numerical errors */
    qsort(v, n, sizeof(double), compar_abs);
    
    for (int i = 0; i < n; i++) {
	a += v[i];
	d += v[i] * v[i];
    }
    p.val = a / n;
    double dev = sqrt( d/n - p.val * p.val);
    p.vlow = p.val - dev;
    p.vhigh = p.val + dev;
    return p;
}


/* 
 * Linear regression coefficient for numbers indexed from 1 to n-1
 */
double regress(double *y, int n)
{
    double val = 0;
    for(int i = 1; i < n; i++)
	val += (2*i - n) * y[i];
    return 12 * val /(n * (n-1) * (n-2));
}

int sum_i(int n)
{
    return n*(n+1)/2;
}

int sum_i2(int n)
{
    int s = 0;
    for(int i = 1; i <= n; i++)
	s += i*i;
    return s;
}

/* 
 * Returns the regression coefficent beta for numbers indexed from 1 to n-1,
 * and the t-score for the hypothesis beta=0
 */
void full_regress(double *y, int n, double *beta, double *pvalue, double *tscore) 
{
    double xbar = .5*n;
    double ybar = avg(y, n - 1);
    double varx = sum_i2(n - 1) - xbar*xbar;
    double vary = var(y, n - 1);
    double cov = 0;
    for(int i = 1; i < n; i++)
	cov += (i+1) * y[i];
    cov /= n - 1;
    cov -= xbar * ybar;

    *beta = cov / varx;
    *tscore = cov * sqrt((n-2) / (varx * vary - cov * cov));
    // degrees of freedom = number points - 2
    *pvalue = studenttdistribution(n - 3, *tscore);
    //*pvalue = gsl_cdf_tdist_P(*tscore, n - 3);
}
			     
}
