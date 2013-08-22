/* Time-stamp: <2013/08/21 14:23:56 benavuya.ime.usp.br [benavuya] am> */
#include <stdlib.h>
#include "stats.h"
#include "glob.h"
// #include <studenttdistr.h>
//double studenttdistribution(int k, double t);
#include <gsl/gsl_cdf.h>
#include <gsl/gsl_fit.h>

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
 * Returns the regression coefficent beta for numbers indexed from 1 to n-1,
 * and the t-score for the hypothesis beta=0
 */
/* 
 * void full_regress(double *y, int n, double conf, double *beta, double *dev, double *t) 
 * {
 *     double x[n];
 *     double alpha, cov00, cov01, cov11, sumsq;
 *     for(int i=0; i < n; i++) x[i] = i;
 *     gsl_fit_linear(x, 1, y+1, 1, n-1, &alpha, beta, &cov00, &cov01, &cov11, &sumsq);
 *     mess("%13.10f %13.10f %13.10f %13.10f %13.10f %13.10f\n", alpha, *beta, cov00, cov01, cov11, sumsq);
 *     *dev = sqrt(sumsq/((n-3)*cov11));
 *     *t = gsl_cdf_tdist_Pinv(.5+conf/2, n-3);
 * }
 */

			     

void full_regress(double *y, int n, double *beta, double *dev) 
{
    double xbar = .5*n;
    double ybar = avg(y+1, n - 1);
    double varx = n*(n-2)/12.;
    
    double vary = var(y+1, n - 1);
    double cov = 0;
    for(int i = 1; i < n; i++)
	cov +=  i * y[i];
    cov /= (n - 1);
    cov -= xbar * ybar;

    *beta = cov / varx;
    *dev = sqrt((varx*vary-cov*cov)/(n-3))/varx;
    
    // degrees of freedom = number points - 2
    //*pvalue = studenttdistribution(n - 3, t);
    //*t = gsl_cdf_tdist_Pinv(.5+conf/2, n-3);
}

/* 
 * t value for given confidence level
 * and degrees of freedom		     
 */
double tvalue(double conf, int df)
{
    return gsl_cdf_tdist_Pinv(.5+conf/2, df);
}

/* 
 * p-value corresponding to given t and degrees of freedom 
 */
double p_value(double t, int df)
{
    return 2*gsl_cdf_tdist_P(-t, df);
}


// Not used

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
