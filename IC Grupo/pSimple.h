#ifndef PSIMPLE_H
#define PSIMPLE_H
#include "vec.h"
#include "RedNeuronal.h"
#include "utiles.h"
using namespace std;

class pSimple : public RedNeuronal{
private:
	vec w;
public:
	pSimple(unsigned int in, double (*umbral)(double));
	bool entrenar(Particion P,unsigned int limitsteps, double tol, double factor, double momento, bool sh);
	unsigned int eval(const vec& entrada, bool biased=false) const;
	void forget();
	bool save(string);
	bool load(string);
	//bool Draw(double xmin, double ymin, double xmax, double ymax, int weigth, int heigth)
	~pSimple();
};

#endif

