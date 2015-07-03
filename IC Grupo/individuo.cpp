#include "individuo.h"
#include <cstdlib>
#include <ctime>

individuo::individuo(){}
void individuo::set(int nin,int ncapas,int nperc[]) {
		//srand(time(NULL));
			
		w.resize(ncapas);
		w[0].resize(nperc[0]);
		int i, j, k;
		for(i=0; i<w[0].size(); i++) {
			w[0][i]=vec(nin+1);
			for(k=0; k<w[0][i].s; k++) 
				w[0][i][k]=double(rand()) * 2 / double(RAND_MAX) - 1;
		}
		for(int i=1; i<w.size(); i++) {
			w[i].resize( nperc[i] );
			for(j=0; j<w[i].size(); j++) {
				w[i][j]=vec(nperc[i-1]+1);
				for(k=0; k<w[i][j].s; k++) 
					w[i][j][k]=double(rand()) * 2 / double(RAND_MAX) - 1;
			}
		}
		
		v.resize(ncapas);
		v[0].resize(nperc[0]);
		for(i=0; i<v[0].size(); i++) {
			v[0][i]=vec(nin+1);
			for(k=0; k<v[0][i].s; k++) 
				v[0][i][k]=0;
		}
		for(int i=1; i<v.size(); i++) {
			v[i].resize( nperc[i] );
			for(j=0; j<v[i].size(); j++) {
				v[i][j]=vec(nperc[i-1]+1);
				for(k=0; k<v[i][j].s; k++) 
					v[i][j][k]=0;
			}
		}
		
		best.resize(ncapas);
		best[0].resize(nperc[0]);
		for(i=0; i<best[0].size(); i++) {
			best[0][i]=vec(nin+1);
			for(k=0; k<best[0][i].s; k++) 
				best[0][i][k]=w[0][i][k];
		}
		for(int i=1; i<best.size(); i++) {
			best[i].resize( nperc[i] );
			for(j=0; j<best[i].size(); j++) {
				best[i][j]=vec(nperc[i-1]+1);
				for(k=0; k<best[i][j].s; k++) 
					best[i][j][k]=w[i][j][k];
			}
		}
	}
	


individuo::~individuo() {
	
}
vector< vector <vec> >individuo::get_w(){
	return w;
}

void individuo::set_best(vector< vector <vec> >wg){
	best = wg;
}


void individuo::updatev(float c1,float c2,vector< vector <vec> > wg){
	double r1=(double(rand())/ double(RAND_MAX));
	double r2=(double(rand())/ double(RAND_MAX));
	int i,j,k;
	for(i=0; i<v.size(); i++) {
		for(j=0; j<v[i].size(); j++) {
			for(k=0; k<v[i][j].s; k++) {
				v[i][j][k]= /*0.4**/v[i][j][k]+c1*r1*(best[i][j][k]-w[i][j][k])+c2*r2*(wg[i][j][k]-w[i][j][k]);
			}
		}
	}
}
void individuo::updatex(){
	int i,j,k;
	for(i=0; i<w.size(); i++) {
		for(j=0; j<w[i].size(); j++) {
			for(k=0; k<w[i][j].s; k++) {
				w[i][j][k]= w[i][j][k]+v[i][j][k];
			}
		}
	}
}

bool individuo::printWeight() {
	if(w.empty()) return false;
	unsigned int i, nc=w.size(), j, np, k, nw;
	for(i=0; i<nc; i++) {
		cout<<"Capa: "<<i<<endl;
		vector< vec >& wi = w[i];
		np = wi.size();
		for(j=0; j<np; j++) {
			cout<<"   Perceptron: "<<j<<endl;
			vec& wij = wi[j];
			nw=wij.s;
			for(k=0; k<nw; k++) cout<<"      w["<<k<<"] -> "<<wij[k]<<endl;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// individuoFuncion ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


individuoFuncion::individuoFuncion(){}

individuoFuncion::~individuoFuncion() {}

void individuoFuncion::set( double _DomX[2], double _DomY[2] ) {
	//srand(time(NULL));
	DomX[0] = _DomX[0];
	DomX[1] = _DomX[1];
	DomY[0] = _DomY[0];
	DomY[1] = _DomY[1];
	
	w.resize(2);
	w[0] = (double(rand()) * (abs(DomX[0])+abs(DomX[1])) ) / double(RAND_MAX) + DomX[0];
	w[1] = (double(rand()) * (abs(DomY[0])+abs(DomY[1])) ) / double(RAND_MAX) + DomY[0];
	//cout<<"Inicia: "<<w[0]<<" --- "<<w[1]<<endl;

	v.resize(2);
	int i;
	for(i=0; i<v.size(); i++)
		v[i] = 0;
	
	best.resize(2);
	for(i=0; i<best.size(); i++) {
		best[i] = w[i];
	}
}

vector< double >individuoFuncion::get_w(){
	return w;
}

void individuoFuncion::set_best(vector< double > wg)
{
	best = wg;
}


void individuoFuncion::updatev(float c1,float c2,vector< double > wg){
	double r1=(double(rand())/ double(RAND_MAX));
	double r2=(double(rand())/ double(RAND_MAX));
	int i;
	for(i=0; i < v.size() ; i++)
	{
		v[i] = 0.4*v[i]+c1*r1*(best[i]-w[i])+c2*r2*(wg[i]-w[i]);
		//cout<<"V["<<i<<"] = "<<v[i]<<endl;
		//cout<<"Termino Local  "<<i<<" = "<<c1*r1*(best[i]-w[i])<<endl;
		//cout<<"best-w         "<<i<<" = "<<(best[i]-w[i])<<endl;
		//cout<<"Termino Global "<<i<<" = "<<c2*r2*( wg[i] -w[i])<<endl;
	}
	
}
void individuoFuncion::updatex(){
	vector<double> aux;
	aux = w;
	int i;
	for(i=0; i < w.size(); i++)
		w[i] = w[i]+v[i];
	
	if( w[0] < DomX[0] )
		w[0] = DomX[0];
	if( w[0] > DomX[1] )
		w[0] = DomX[1];
	if( w[1] < DomY[0] )
		w[1] = DomY[0];
	if( w[1] > DomY[1] )
		w[1] = DomY[1];
	/*
	if( min(w[0],w[1]) < min(DomX[0],DomY[0]) );
	{
		w = aux;
		cout<<"Limite"<<endl;
	}
	if( max(w[0],w[1]) > max(DomX[1],DomY[1]) );
	{
		w = aux;
		cout<<"Limite"<<endl;
	}*/
	
}
