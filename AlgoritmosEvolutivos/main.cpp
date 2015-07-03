#include<iostream>
#include <vector>
#include "Isla.h"
#include "Real.h"
#include "Funcion.h"
#include <GL/glu.h>
#include <GL/glut.h>
#include <sys/time.h>
#include "Tour.h"
#include "TSP.h"
using namespace std;

const unsigned int w=600, h=600;

void reshape_cb (int w, int h) {
	if (w==0||h==0) return;
	glViewport(0,0,w,h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0,w,0,h);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
}

void initialize() {
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (w,h);
	glutInitWindowPosition (400,100);
	glutCreateWindow ("Algoritmos evolutivos");
	glutReshapeFunc (reshape_cb);
	glClearColor(1.f,1.f,1.f,1.f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH); glEnable(GL_LINE_SMOOTH); glEnable(GL_POLYGON_SMOOTH);
	reshape_cb(w,h);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	srand( time(NULL) );
}

int main (int argc, char *argv[]) {
	glutInit (&argc, argv);
	initialize();
	//Ejercicio 1
//	Funcion* M=new Funcion();
//	Isla< Real, Funcion > Comunidad(5, 50, 0.3, 4, true, M);
//	Comunidad.Nacer();
//	Comunidad.Evolucionar(100, -DBL_MAX);
	//Ejercicio 2
//	TSP* M=new TSP();
//	Isla< Tour, TSP > Comunidad(50, 1000, 0.4, 4, true, M);
//	while(Comunidad.Nacer())
//		Comunidad.Evolucionar(10000, -DBL_MAX);
//	
	//Version paralela
	
	TSP* M=new TSP();
	Isla< Tour, TSP > Comunidad1(100, 1000, 0.1, 4, false, M);
	Isla< Tour, TSP > Comunidad2(100, 1000, 0.1, 4, false, M);
	while(!Comunidad1.Nacer());
	Comunidad2.Nacer(Comunidad1);
	int nm, m;
	bool continuar=true;
	char s;
	while(continuar) {
		cout<<"Ingrese la cantidad de migraciones: ";
		cin>>nm;
		for(m=0; m<nm; m++) {
			tic
#pragma omp parallel 
				{
#pragma omp sections
					{
#pragma omp section
						Comunidad2.Evolucionar(500, -DBL_MAX);
						
#pragma omp section
						Comunidad1.Evolucionar(500, -DBL_MAX);
					}
					
				}
			cout<<"Migracion: "<<m<<endl;
			cout<<"Tiempo: ";
			toc;
			Comunidad1.Migrar(Comunidad2, 25);
			Comunidad1.Draw();
		}
		cout<<"Continuar? [s/n]: ";
		cin>>s;
		continuar = (s=='s');
	}
	delete M;
	return 0;
}

