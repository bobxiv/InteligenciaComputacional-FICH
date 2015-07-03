#ifndef INDIVIDUO_H
#define INDIVIDUO_H
#include "utiles.h"
#include "vec.h"

using namespace std;

class individuo {
private:
	vector< vector <vec> >w;
	vector< vector <vec> >best;
	vector< vector <vec> >v;
	
public:
	individuo();
	void set(int nin,int ncapas,int nperc[]);
	~individuo();
	vector< vector <vec> >get_w();
	void set_best(vector< vector <vec> >wg);
	void updatev(float c1,float c2,vector< vector <vec> >wg);
	void updatex();
	bool printWeight();
	
};

class individuoFuncion {
private:
	vector< double > w;
	vector< double >best;
	vector< double >v;
	
	double DomX[2];
	double DomY[2];
	
public:
	individuoFuncion();
	
	~individuoFuncion();
	
	void set( double DomX[2], double DomY[2] );
	
	vector< double > get_w();
	
	void set_best(vector< double >wg);
	
	void updatev(float c1,float c2,vector< double >wg);
	
	void updatex();
	
};

#endif

