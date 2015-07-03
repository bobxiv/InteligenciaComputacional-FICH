#ifndef ISLA_H
#define ISLA_H
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <omp.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glut.h>
using namespace std;

#define tic struct timeval t_ini, t_fin; gettimeofday(&t_ini, NULL);

#define toc gettimeofday(&t_fin, NULL);\
cout<<(double)(t_fin.tv_sec+(double)t_fin.tv_usec/1000000)-(double)(t_ini.tv_sec + (double)t_ini.tv_usec/1000000)<<endl;

template <class TIndividuo, class TAmbiente>
class Isla {
private:
	unsigned int Nmutaciones;
	unsigned int Nhijos;
	unsigned int Competencia;
	unsigned int t;
	bool dibujar;
	double BestFit;
	double AverageFit;
	vector<TIndividuo> Individuo;
	TAmbiente* Mundo;
public:
	Isla(unsigned int gap, unsigned int poblacion, double pmutacion, unsigned int c, bool d, TAmbiente* m)
	: Nmutaciones(floor(pmutacion * poblacion)),
	  Nhijos(min( poblacion - gap, poblacion-1)),
	  Competencia(c),
	  t(0),
	  dibujar(d),
	  Individuo(poblacion),
	  Mundo(m) {
		if(Nhijos%2==1) --Nhijos;
	}
	~Isla() {
		
	}
	bool Evolucionar(unsigned int lsteps, double tol) {
//		cout<<"This section is executed by thread "<<omp_get_thread_num()<<endl;
		unsigned int step=0, i, j, Nind=Individuo.size();
		vector<TIndividuo> ProximaGeneracion(Nhijos);
		for(i=0; i<Nhijos; i++) ProximaGeneracion[i].Nacer(Mundo);
		srand(time(NULL));
		
		i=Nind;
		while(i>0) Individuo[--i].CalcularFitness(Mundo);
		//Comienzo a iterar
		while((step++)<lsteps) {
			if(dibujar)
				this->Draw();
			else
				sort(Individuo.begin(), Individuo.end());
			
			
			if(Individuo.front().GetFitness() <= tol) return true;
			//Etapa de cruzamiento
			i=Nhijos;
			while(i>0) { 
				//Elijo el padre
				TIndividuo& Padre = Individuo[ this->Seleccionar(true) ];
				TIndividuo& Madre = Individuo[ this->Seleccionar(true) ];
				//Los cruzo para formar dos hijos
				TIndividuo& Hijo1 = ProximaGeneracion[--i];
				TIndividuo& Hijo2 = ProximaGeneracion[--i];
				
				Padre.CrossOver(Madre, Hijo1, Hijo2);
				Hijo1.CalcularFitness(Mundo);
				Hijo2.CalcularFitness(Mundo);
			}
			//Paso a los hijos a la lista de individuos y elimino los viejos
			i=Nhijos;
			j=Nind;
			while(i>0) Individuo[--j] = ProximaGeneracion[--i];
			//Etapa de mutacion
			i=Nmutaciones;
			while(i>0) {
				//TIndividuo& Mutante = Individuo[ this->Seleccionar(false) ];
				TIndividuo& Mutante = Individuo[ rand()%(Nind-1)+1 ];
				Mutante.Mutar(Mundo);
				Mutante.CalcularFitness(Mundo);
				--i;
			}
		}
		return false;
	}
	bool Nacer()  {
		//Inicializacion  de los individuos
		if(!Mundo->Iniciar()) return false;
		for(int i=Individuo.size()-1; i>=0; --i) Individuo[i].Nacer(Mundo);
		return true;
	}
	bool Nacer(Isla<TIndividuo, TAmbiente>& i)  {
		//Inicializacion  de los individuos
		Mundo=i.Mundo;
		for(int i=Individuo.size()-1; i>=0; --i) Individuo[i].Nacer(Mundo);
		return true;
	}
	unsigned int Seleccionar(bool mejor) {
		unsigned int Nind=Individuo.size(), j=Competencia, r, rb=rand()%Nind;
		if(mejor) {
			while(j>0) {
				r=rand()%Nind;
				if( Individuo[r] < Individuo[rb] ) rb=r;
				--j;
			}
		}
		else {
			while(j>0) {
				r=rand()%Nind;
				if( !(Individuo[r] < Individuo[rb]) ) rb=r;
				--j;
			}
		}
		
		return rb;
	}
	void Draw() {
		stable_sort(Individuo.begin(), Individuo.end());
		AverageFit=0;
		int i=Individuo.size();
		while(i>0)
			AverageFit += Individuo[--i].GetFitness();
		AverageFit/=float(Individuo.size());
		BestFit=Individuo.front().GetFitness();
		
		cout<<t<<"-> Best fitness: "<<BestFit<<" -> "<<BestFit/Mundo->getSolFit()<<"    Average fitness: "<<AverageFit<<"    ";
		Individuo.front().PrintValue();
		static int anterior = glutGet(GLUT_ELAPSED_TIME);
		int lapso=0;
		while(lapso<200) lapso=glutGet(GLUT_ELAPSED_TIME)-anterior;
		anterior=glutGet(GLUT_ELAPSED_TIME);
		glClear(GL_COLOR_BUFFER_BIT);
		Mundo->Draw();
		for(i=Individuo.size()-1; i>0; --i) Individuo[i].Draw(false, Mundo);
		Individuo[0].Draw(true, Mundo);
		glutSwapBuffers();
	}
	void Migrar(Isla<TIndividuo, TAmbiente>& v, int k) {
		while(k>0) {
			v.Individuo[v.Seleccionar(false)] = this->Individuo[ this->Seleccionar(true) ];
			this->Individuo[this->Seleccionar(false)] = v.Individuo[ v.Seleccionar(true) ];
			--k;
		}
		v.Individuo[v.Seleccionar(false)] = *max_element(this->Individuo.begin(), this->Individuo.end());
		this->Individuo[this->Seleccionar(false)] = *max_element(v.Individuo.begin(), v.Individuo.end());
	}
};

#endif

