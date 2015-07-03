#ifndef MENU_H
#include "Particion.h"
#include "RedNeuronal.h"
#include <string>
#include "PML.h"
using namespace std;
#define MENU_H

class Menu {
private:
	Particion* part;
	unsigned int w;
	unsigned int h;
	double	xmin;
	double xmax;
	double ymin;
	double ymax;
	RedNeuronal* nn;
	bool dibujando;
	bool haypart;
	bool haynn;
	bool haydes;
	
	int* ppc;
	PML *pml;
	unsigned int nc;
public:
	Menu(unsigned int w, unsigned int h);
	bool askParticion();
	bool askRedNeuronal();
	bool askEntrenar();
	bool askKOutVal();
	bool askDraw();
	bool askAbrirPart();
	bool askMain();
	bool askAbrirNN();
	bool si();
};

#endif

