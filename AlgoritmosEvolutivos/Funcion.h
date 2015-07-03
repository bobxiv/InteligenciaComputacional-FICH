#ifndef FUNCION_H
#include <cstdlib>
#include <cmath>
#include <GL/gl.h>
#include <cfloat>
#include <vector>
using namespace std;
#define FUNCION_H

#define SMOOTHNESS 100
extern const unsigned int w, h;

#if 0
#define ULX 512
#define ULY 512
#define LLX -512
#define LLY -512
#define funcion(x,y) -x * sin( sqrt(abs(x)) )
#endif
#if 0
#define ULX 20
#define ULY 20
#define LLX 0
#define LLY 0
#define funcion(x,y)  x + 5*sin(3*x) + 8*cos(5*x);
#endif
#if 1
#define ULX 100
#define ULY 100
#define LLX -100
#define LLY -100
#define funcion(x,y) pow(x*x+y*y, 0.25) * (pow( sin(50 * pow(x*x+y*y, 0.1)), 2) + 1);
#endif
#if 0
#define ULX 10
#define ULY 10
#define LLX -5
#define LLY -5
#define funcion(x,y) 2*pow(x*x+y*y, 0.4) * (pow( sin(40 * pow(x*x+y*y, 0.1)), 2) + 1) + pow((x-5)*(x-5)+(y-5)*(y-5), 0.5) * (pow( sin(40 * pow((x-5)*(x-5)+(y-5)*(y-5), 0.15)), 2) + 1);;
#endif

class Funcion  {
friend class Real;
private:
	double UperLimitX;
	double LowerLimitX;
	double UperLimitY;
	double LowerLimitY;
	double maxim;
	double minim;
	double solutionFit;
	vector< vector<double> > value;
public:
	Funcion() : 
	UperLimitX(ULX),
	LowerLimitX(LLX),
	UperLimitY(ULY),
	LowerLimitY(LLY),
	maxim(-DBL_MAX),
	minim(DBL_MAX),
	value(SMOOTHNESS+1, vector<double>(SMOOTHNESS+1)) {}
	bool Iniciar() {
		double x, y, dx=(UperLimitX-LowerLimitX)/SMOOTHNESS, dy=(UperLimitY-LowerLimitY)/SMOOTHNESS, v;
		int i=0, j;
		for(x=LowerLimitX; x<=UperLimitX; x+=dx) {
			j=0;
			for(y=LowerLimitY; y<=UperLimitY; y+=dy) {
				v=this->Evaluar(x,y);
				if(v<minim) minim=v;
				if(v>maxim) maxim=v;
				value[i][j++]=v;
			}
			++i;
		}
		solutionFit = minim;
		return true;
	}
	
	double Evaluar(double x, double y) {
		return funcion(x,y);
	}
	void Draw() {
		double v, dif=1/(maxim-minim);
		int i, j, x, y, di=w/SMOOTHNESS, dj=h/SMOOTHNESS;
		for(i=0; i<SMOOTHNESS; i++) {
			glBegin(GL_TRIANGLE_STRIP);
			for(j=0; j<=SMOOTHNESS; j++) {
				x = i*di;
				y = j*dj;
				v=(value[i][j]-minim)*dif;
				glColor3d(v,v,v);
				glVertex2i(x,y);
				
				x = (i+1)*di;
				v=(value[i+1][j]-minim)*dif;
				glColor3d(v,v,v);
				glVertex2i(x,y);
			}
			glEnd();
		}
	}
	double getSolFit() {return solutionFit;}
};

#endif

