#ifndef RBF_H
#include "vec.h"
#include <vector>
#include <cstdlib>
#include "RedNeuronal.h"
using namespace std;
#define RBF_H

class RBF : public RedNeuronal {
private:
	unsigned int nrbf;
	unsigned int np;
//	vector<double> krbf;
	vector<vec> u;
	vector<vec> var;
	vector<vec> w;
public:
	RBF(unsigned int dim, unsigned int nr, unsigned int nper, double (*u)(double));
	~RBF() {};
	bool entrenar(Particion P,unsigned int limitsteps, double tol, double factor, double momento, bool saveHistory);
	bool eval(vec, vec&, bool biased=false) const;
	unsigned int eval(const vec& entrada, bool biased=false) const;
	bool ParticularDraw(double xmin, double ymin, double xmax, double ymax, int w, int h);
	bool printWeight();
	void forget();
	
};

#endif
