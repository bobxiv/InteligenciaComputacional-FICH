#ifndef REAL_H
#define REAL_H
#include "Funcion.h"
#include <iostream>
using namespace std;

extern const unsigned int w, h;

class Real {
private:
	double fit;
	double x;
	double y;
public:
	Real() : fit(DBL_MAX), x(0), y(0) {};
	void CalcularFitness(Funcion* f) {
		this->x = min(this->x, f->UperLimitX);
		this->y = min(this->y, f->UperLimitY);
		this->x = max(this->x, f->LowerLimitX);
		this->y = max(this->y, f->LowerLimitY);
		this->fit = f->Evaluar(x,y);
	}
	void Nacer(Funcion* f) {
		this->Mutar(f);
	}
	void CrossOver(const Real& Pareja, Real& Hijo1, Real& Hijo2) const {
		/*
		Hijo1.x = this->x;
		Hijo1.y = Pareja.y;
		Hijo2.x = Pareja.x;
		Hijo2.y = this->y;
		//*/
		double d = 1/max(sqrt((this->x - Pareja.x)*(this->x - Pareja.x) + (this->y - Pareja.y)*(this->y - Pareja.y)), 1.0);
		double r = (float(rand())/float(RAND_MAX)) * (1 + d) - (1 + d)/2;
		Hijo1.x = this->x*r + (1-r)*Pareja.x;
		Hijo2.x = this->x*(1-r) + r*Pareja.x;
		
		r = (float(rand())/float(RAND_MAX)) * (1 + d) - (1 + d)/2;
		Hijo1.y = this->y*r + (1-r)*Pareja.y;
		Hijo2.y = this->y*(1-r) + r*Pareja.y;
		
	}
	void Mutar(Funcion* f)  {
		x = (float(rand()) / float(RAND_MAX)) * (f->UperLimitX - f->LowerLimitX) + f->LowerLimitX;
		y = (float(rand()) / float(RAND_MAX)) * (f->UperLimitY - f->LowerLimitY) + f->LowerLimitY;
	}
	void operator=(const Real& otro) {
		this->x = otro.x;
		this->y = otro.y;
		this->fit = otro.fit;
	}
	void PrintValue() {
		cout<<"x = "<<x<<"   y = "<<y<<endl;
	}
	double GetFitness() const {
		return fit;
	}
	bool operator<(const Real& otro) const {
		return (this->fit < otro.fit);
	}
	void Draw(bool best, Funcion* f) {
		if(best) glColor3f(1.0, 0.0, 0.0);
		else glColor3f(0.0, 0.0, 1.0);
		glPointSize(3);
		glBegin(GL_POINTS);
		glVertex2i( w*(x - f->LowerLimitX)/(f->UperLimitX - f->LowerLimitX),
			h*(y - f->LowerLimitY)/(f->UperLimitY - f->LowerLimitY));
		glEnd();
	}
};
#endif

