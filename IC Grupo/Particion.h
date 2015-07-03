#ifndef PARTICION_H
#include "vec.h"
#include "utiles.h"
using namespace std;
#define PARTICION_H

class Particion {
private:
	vector<vec> pat;
	vector<vec> ppat;
	vec d;
	vec pd;
	unsigned int k;
	unsigned int ki;
public:
	bool load(string arch);
	Particion() {};
	Particion(const Particion& p);
	Particion(int,int,int);
	~Particion();
	bool reordenar();
	bool reacomodar(float p);
	bool resize(double porcentaje, double noise);
	bool save(string arch) const;
	Particion crearParticion() const;
	void clear();
	bool initLeaveKOut(unsigned int k);
	bool nextLeaveKOut(Particion& p);
	double deseado(unsigned int i) const;
	unsigned int ncasos() const;
	const vec& patron(unsigned int i) const;
	double pdeseado(unsigned int i) const;
	unsigned int pncasos() const;
	const vec& ppatron(unsigned int i) const;
	unsigned int nin() {return pat[0].s-1;};
	vec getVecError(const vec& y, unsigned int casoi) const;
	void SetBoundingBox(double&,double&,double&,double&);
	
};

#endif

