#include <GL/glut.h>
#include <ctime>
#include <omp.h>
#include "pSimple.h"
#include "PML.h"
#include "Menu.h"
using namespace std;
/*
unsigned int w=600, h=600;
Menu* menu;

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
	glutCreateWindow ("Redes Neuronales");
	glutReshapeFunc (reshape_cb);
	glClearColor(1.f,1.f,1.f,1.f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_POINT_SMOOTH); glEnable(GL_LINE_SMOOTH); glEnable(GL_POLYGON_SMOOTH);
	reshape_cb(w,h);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	srand( time(NULL) );
	menu = new Menu(w,h);
	menu->askMain();
}

int main (int argc, char **argv) {
	cout<<argv[0]<<endl;
	glutInit (&argc, argv);
	initialize();
	return 0;
}


*/

double f1(double x, double y)
{
	return ( -x*sin( sqrt( abs(x) ) ) );
}

double f2(double x, double y)
{
	
	return ( x + 5*sin(3*x) + 8*cos(5*x) );
}

double f3(double x, double y)
{
	double sin2 = sin( 50*pow( (x*x + y*y) , 0.1 ) );
	sin2 *= sin2;
	return ( pow( (x*x + y*y), 0.25 ) * ( sin2 + 1 ) );
}

#include "enjambre.h"
#include "individuo.h"

int main (int argc, char **argv) {

	//Enjambre entrena Red Neuronal
	/*
	int maxiteraciones = 1000;
	double tolerancia = 0.01;
	//int aux[] = {3,3,1};
	int aux[] = {4,1};
	
	srand(time(NULL));
	
	enjambre Swarm(100,2,2,aux);
	PML* pml = new PML(2,2,aux,sigmodea);
	Particion* part = new Particion();
	//if( part->load("clouds.csv") == false )
	if( part->load("concent.csv") == false )
	//if( part->load("xor.csv") == false )
	//if( part->load("or.csv") == false )
		return 1;
	
	Swarm.run(*pml,*part,maxiteraciones,tolerancia);
	
	cout<<"Error: "<<(double)pml->nerror(*part,false)<<endl;
	*/
	
	//Enjambre busca minimo
	int maxiteraciones = 100;
	double tolerancia = -9999;

	srand(time(NULL));
	//double Dom[] = {-512,512};//f1
	//double Dom[] = {0,20};//f2
	double Dom[] = {-100,100};//f3
	enjambreFuncion Swarm(10,Dom,Dom);
	double minX=0;
	double minY=0;
	Swarm.run(f3,maxiteraciones,minX,minY);
	
	cout<<"MinX: "<<minX<<endl;
	cout<<"MinY: "<<minY<<endl;
	cout<<"Min:  "<<f3(minX,minY)<<endl;
	

	system("pause");

	return 0;
}

