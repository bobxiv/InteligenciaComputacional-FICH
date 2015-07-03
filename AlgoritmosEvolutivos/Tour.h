#ifndef Tour_H
#define Tour_H
#include <iostream>
#include "TSP.h"
#include <bitset>
#include <climits>
#include <set>
using namespace std;

class Tour {
private:
	double fit;
	vector<unsigned int> city;
	vector< char > Agregado;
public:
	Tour() : fit(UINT_MAX), city() {};
	void CalcularFitness(TSP* f) {
		fit = f->Longitud(city);
	}
	void Nacer(TSP* f) {
		int i, j, n=f->cityX.size();
		city.resize( n );
		for(i=n-1; i>=0; --i) {
			city[i]=i;
		}
		
		for(j=n-1; j>=0; --j) {
			i=rand()%n;
			swap(city[j], city[i]);
		}
		Agregado.resize(n);
		
	}
	void CrossOver(const Tour& Pareja, Tour& Hijo1, Tour& Hijo2) {
		unsigned int n=city.size(), i, c1, c2;
		vector< set<unsigned int> > vecino(n);
		set<unsigned int>::iterator p;
		
		for(i=n-1; i>0; --i) {
			c1=city[i];	c2=city[i-1];
			vecino[c1].insert(c2);
			vecino[c2].insert(c1);
			c1=Pareja.city[i]; c2=Pareja.city[i-1];
			vecino[c1].insert(c2);
			vecino[c2].insert(c1);
		}
		c1=city.front();	c2=city.back();
		vecino[c1].insert(c2);
		vecino[c2].insert(c1);
		c1=Pareja.city.front(); c2=Pareja.city.back();
		vecino[c1].insert(c2);
		vecino[c2].insert(c1);
		
		//primer hijo
		//Hijo1.city.resize(n,0);
		
		Agregado.assign(n, 0);
		int nodo = city.front();
		for(i=0; i<n; i++) {
			Hijo1.city[i]=nodo;
			Agregado[nodo]=1;
			const set<unsigned int>& vn=vecino[nodo];
			p=vn.begin();
			nodo=-1;
			while(p!=vn.end()) {
				if(!Agregado[ *p ]) {
					nodo = *p;
					break;
				}
				p++;
			}
			if(nodo==-1) {
				nodo=n-1;
				while(nodo>=0 && Agregado[nodo]) --nodo;
			}
		}

		//mismo proceso para el segundo
		Agregado.assign(n, 0);
		//Hijo2.city.resize(n, 0);
		nodo = Pareja.city.front();
		for(i=0; i<n; i++) {
			Hijo2.city[i]=nodo;
			Agregado[nodo]=1;
			const set<unsigned int>& vn=vecino[nodo];
			p=vn.begin();
			nodo=-1;
			while(p!=vn.end()) {
				if(!Agregado[ *p ]) {
					nodo = *p;
					break;
				}
				p++;
			}
			if(nodo==-1) {
				nodo=n-1;
				while(nodo>=0 && Agregado[nodo]) --nodo;
			}
		}
	}
	void Mutar(TSP* f){
		int n=city.size(), i=rand()%n, j=rand()%n;
//		for(j=n-1; j>=0; --j) {
//			i=rand()%n;
//			swap(city[j], city[i]);
//		}
		swap(city[j], city[i]);
	}
	void operator=(const Tour& otro){
		this->city.assign(otro.city.begin(), otro.city.end());
		this->fit = otro.fit;
	}
	void PrintValue() {
		cout<<endl;
		return;
		cout<<"Tour={";
		for(int i=city.size()-1; i>=0; --i) {
			cout<<city[i]<<","; 
		}
		cout<<"}"<<endl;
	}
	double GetFitness() const {
		return fit;
	}
	bool operator<(const Tour& otro) const {
		return (this->fit < otro.fit);
	}
	void Draw(bool best, TSP* f) {
		if(!best) return;
		glColor3f(0.4,0.6,0.6);
		int c;
		glLineWidth(2);
		glBegin(GL_LINE_STRIP);
		for(int i=city.size()-1; i>=0; --i) {
			c=city[i];
			glVertex2i( f->dcityX[c], f->dcityY[c] );
		}
		c=city.back();
		glVertex2i( f->dcityX[c], f->dcityY[c] );
		glEnd();
	}
};
#endif
