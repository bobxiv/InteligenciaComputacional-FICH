#ifndef TSP_H
#define TSP_H
#include "Tour.h"
#include <fstream>
using namespace std;

extern const unsigned int w, h;


class TSP {
	friend class Tour;
private:
	vector<double> cityX;
	vector<double> cityY;
	vector<int> dcityX;
	vector<int> dcityY;
	vector<int> solution;
	double solutionFit;
	bool HaveSolution;
	bool iniciado;
	vector< vector<double> > distancia;
public:
	TSP() {
		iniciado=false;
	};
	bool Iniciar() {
		if(iniciado) return true;
		HaveSolution=false;
		string name1, name2;
		cout<<"Elegir TSP:"<<endl;
		system("ls *.tsp");
		cin>>name1;
		name2=name1;
		name1.append(".tsp");
		name2.append(".tour");
		ifstream archi(name1.c_str());
		ifstream archi2(name2.c_str());
		if(!archi.is_open() || archi.bad()) {
			cout<<"No se encontro el archivo"<<endl;
			return false;
		}
		
		solutionFit=1;
		int nc, i, j;
		double cx, cy, d, mx=DBL_MAX, my=DBL_MAX, Mx=-DBL_MAX, My=-DBL_MAX;

		archi>>nc;
		cityX.resize(nc);
		cityY.resize(nc);
		dcityX.resize(nc);
		dcityY.resize(nc);
		int aux;
		distancia.resize(nc, vector<double>(nc));
		for(i=0; i<nc; ++i) {
			archi>>aux>>cityX[i]>>cityY[i];
			Mx=max(cityX[i], Mx);
			My=max(cityY[i], My);
			mx=min(cityX[i], mx);
			my=min(cityY[i], my);
		}
		double Lx=Mx-mx;
		double Ly=My-my;
		for(i=0; i<nc; ++i) {
			dcityX[i] = ((cityX[i]-mx)/Lx *0.9 + 0.05)*w;
			dcityY[i] = ((cityY[i]-my)/Ly *0.9 + 0.05)*h;
		}
		for(i=nc-1; i>=0; --i) {
			for(j=nc-1; j>=i; --j) {
				cx=(cityX[i] - cityX[j]);
				cy=(cityY[i] - cityY[j]);
				d = sqrt(cx*cx + cy*cy);
				distancia[i][j]=d;
				distancia[j][i]=d;
			}
		}
		archi.close();
		
		if(archi2.is_open() && archi2.good()) {
			HaveSolution=true;
			archi2>>nc;
			solution.resize(nc);
			int aux;
			for(i=0; i<nc; ++i) {
				archi2>>aux;
				solution[i]=aux-1;
			}
			solutionFit=0;
			for(i=0; i<nc; ++i) {
				solutionFit += distancia[ solution[i] ][ solution[i-1] ];
			}
			solutionFit+=distancia[ solution.back() ][ solution.front() ];
		}
		archi2.close();
		cout<<"Archivo correctamente abierto."<<endl;
		iniciado=true;
		return true;
	}
	double Longitud( vector<unsigned int>& city ) {
		double suma=0;
		for(int i=city.size()-1; i>0; --i) {
			suma += distancia[ city[i] ][ city[i-1] ];
		}
		suma+=distancia[ city.back() ][ city.front() ];
		return suma;
	}
	void Draw() {
		if(HaveSolution) {
			int c;
			glColor3f(0.9,0.8,0.8);
			glLineWidth(1);
			glBegin(GL_LINE_STRIP);
			for(int i=solution.size()-1; i>=0; --i) {
				c=solution[i];
				glVertex2i( dcityX[c], dcityY[c] );
			}
			c=solution.back();
			glVertex2i( dcityX[c], dcityY[c] );
			glEnd();
		}
		glColor3f(1.0,0.1,0.1);
		glPointSize(4);
		glBegin(GL_POINTS);
		for(int i=cityX.size()-1; i>=0; --i) {
			glVertex2i( dcityX[i], dcityY[i] );
		}
		glEnd();
	}
	double getSolFit() {
		if(HaveSolution) return solutionFit;
		else return 1;
	}
};
#endif
