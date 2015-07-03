#ifndef REDNEURONAL_H
#include "Particion.h"
#define REDNEURONAL_H

class RedNeuronal {
protected:
	unsigned int nin;
	unsigned int evol;
	double (*umbral)(double);
	vector< RedNeuronal* > desarrollo;
	vector< RedNeuronal* >::iterator p;
public:
	virtual bool entrenar(Particion P,unsigned int limitsteps, double tol, double factor, double momento, bool saveHistory) {cout<<"No implementado aun!"<<endl;};
	virtual unsigned int eval(const vec& entrada, bool biased) const {cout<<"No implementado aun!"<<endl;};
	virtual void forget() {cout<<"No implementado aun!"<<endl;};
	virtual bool save(string name) {cout<<"No implementado aun!"<<endl;};
	virtual bool load(string name) {cout<<"No implementado aun!"<<endl;};
	virtual bool printWeight() {cout<<"No implementado aun!"<<endl;};
	virtual bool ParticularDraw(double xmin, double ymin, double xmax, double ymax, int w, int h) {return false;};
	bool nextState();
	bool Draw(double xmin, double ymin, double xmax, double ymax, int w, int h);
	bool Validacion(const Particion& P, double&, unsigned int iter) const;
	bool leavekout(Particion part, unsigned int k, unsigned int, double, double, double& media, double& desvest);
	unsigned int getNin() {return nin;};
	void setNin(unsigned int nn) {nin=nn;};
	
};

#endif

