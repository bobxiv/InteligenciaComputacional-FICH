#ifndef PML_H
#include "vec.h"
#include "utiles.h"
#include "RedNeuronal.h"
using namespace std;
#define PML_H

class PML : public RedNeuronal {
private:
	vector< vector<vec> > w;
public:
	PML(unsigned int n, unsigned int ncapas, int* perporlayer, double (*u)(double));
	~PML();
	bool entrenar(Particion P,unsigned int limitsteps, double tol, double factor, double momento, bool saveHistory);
	bool ParticularDraw(double xmin, double ymin, double xmax, double ymax, int w, int h);
	bool eval(vec, vec&, bool biased=false) const;
	bool printWeight();
	unsigned int eval(const vec& entrada, bool biased=false) const;
	void forget();
	bool save(string name);
	bool load(string name);
	void setw(const vector< vector<vec> > &ww);
	double nerror(const Particion &P,bool ban);
	
};

#endif

