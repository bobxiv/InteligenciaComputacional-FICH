#ifndef ENJAMBRE_H
#define ENJAMBRE_H
#include "utiles.h"
#include "vec.h"
#include "individuo.h"
#include "PML.h"
#include "Particion.h"
using namespace std;

class enjambre{
private:
	int cant;
	vector<individuo>x;
	vector<double>f;
	vector<double>fbest;
	individuo* yglobal;
	double yglob;
public:
	enjambre(int cantindv,int nin,int ncapas,int nperc[]);
	~enjambre();
	void evaluar(PML &nn,const  Particion &P);
	void run(PML &nn,const  Particion &P,unsigned int maxiter,double tol);
	
};


class enjambreFuncion{
private:
	int cant;
	vector<individuoFuncion>x;
	vector<double>f;
	vector<double>fbest;
	individuoFuncion yglobal;
	double yglob;
public:
	enjambreFuncion(int cantindv, double DomX[2], double DomY[2]);
	~enjambreFuncion();
	void evaluar( double (*fn)(double x, double y) );
	void run( double (*fn)(double x, double y), unsigned int maxiter, double& minX, double& minY);
	
};

#endif

