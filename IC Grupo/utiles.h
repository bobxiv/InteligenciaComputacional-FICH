#ifndef utiles_H
#define utiles_H
#include <omp.h>
#include <stdio.h>
#include <fstream>
//#include <sys/time.h>
#include <string>
#include <vector>
#include <iostream>
#include <GL/gl.h>
#include <cmath>
using namespace std;

/*
#define tic struct timeval t_ini, t_fin; gettimeofday(&t_ini, NULL);

#define toc gettimeofday(&t_fin, NULL);\
cout<<(double)(t_fin.tv_sec+(double)t_fin.tv_usec/1000000)-(double)(t_ini.tv_sec + (double)t_ini.tv_usec/1000000)<<endl;
*/

//#define _revienta(cond) if (cond) asm("int3"); asm("nop");
#define _revienta(cond) ;

static double signo(double v) {
	return (v<0? -1 : 1);
}

static double sigmodea(double v) {
	return 2/(1+exp(-0.1*v))-1;
}



#endif
