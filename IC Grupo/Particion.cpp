#include "Particion.h"
using namespace std;

Particion::Particion(int nc, int ncp, int nin) : pat(nc, vec(nin)), d(nc), ppat(ncp, vec(nin)), pd(ncp), k(0), ki(0) {

}

Particion::Particion(const Particion& p) : pat(p.pat), d(p.d), ppat(p.ppat), pd(p.pd), k(p.k), ki(p.ki) {
}

bool Particion::load(string arch) {
	ifstream archi( arch.c_str() );
	double aux, p;
	if(!archi.is_open()) return false;
	int ncasos, nin, j=0, i;
	archi>>ncasos;
	archi.ignore();
	archi>>nin;
	archi.ignore();
	nin++;
	pat.resize(ncasos, vec(nin));
	d.alloc(ncasos,false);
	while( j < ncasos ) {
		pat[j][0]=-1;
		for(i=1; i < nin; i++) {
			archi>>pat[j][i];
			archi.ignore();
		}
		archi>>p;
		d[(j++)]=(p<0? 0:p);
		archi.ignore();
	}
	archi.close();
	k=ki=0;
	return true;
}

Particion::~Particion() {
	
}


bool Particion::reordenar() {
	int ncasos=pat.size(), i, j;
	for(j=0; j<ncasos; j++) {
		i=rand()%ncasos;
		swap(pat[j], pat[i]);
		swap(d[j], d[i]);
	}
	return true;
}

bool Particion::reacomodar(float p) {
	if(p>1) return false;
	unsigned int ncasos=(pat.size()+ppat.size())*p;
	while(pat.size()>ncasos) {
		ppat.push_back( pat[pat.size()-1] );
		pat.pop_back();
		pd.push_back( d.pop_back() );
	}
	while(pat.size()<ncasos) {
		pat.push_back( ppat[ppat.size()-1] );
		ppat.pop_back();
		d.push_back( pd.pop_back() );
	}
	if(ppat.size()<pat.size()) {
		int ncasos=ppat.size(), i, j;
		for(j=0; j<ncasos; j++) {
			i=rand()%pat.size();
			swap(pat[i], ppat[j]);
			swap(d[i], pd[j]);
		}
	}
	else {
		int ncasos=pat.size(), i, j;
		for(j=0; j<ncasos; j++) {
			i=rand()%ppat.size();
			swap(pat[j], ppat[i]);
			swap(d[j], pd[i]);
		}
	}
	return true;
}

unsigned int Particion::ncasos() const {
	return pat.size();
}

const vec& Particion::patron(unsigned int i) const {
	return pat[i];
}

double Particion::deseado(unsigned int i) const {
	return d[i];
}

unsigned int Particion::pncasos() const {
	return ppat.size();
}

const vec& Particion::ppatron(unsigned int i) const {
	return ppat[i];
}

double Particion::pdeseado(unsigned int i) const {
	return pd[i];
}

bool Particion::resize(double p, double pr){
	if(p<0 || p==1) return false;
	if(p<1) {
		unsigned int n=pat.size()*p, pn=ppat.size()*p;
		pat.resize(n);
		d.alloc(n,true);
		ppat.resize(pn);
		pd.alloc(pn,true);
	}
	else {
		int ins=pat.size(), nca = (ins * (p-1)), i, j;
		pat.reserve( ins + nca );
		d.alloc( ins + nca, true);
		for(i=0;i<nca;i++){
			j= rand() % ins;
			vec newv=pat[j].noise( pr );
			newv[0]=-1;
			pat.push_back( newv );
			d[ins+i]=d[j];
		}
		ins=ppat.size();
		nca = (ins * (p-1));
		ppat.reserve( ins + nca );
		pd.alloc( ins + nca, true );
		for(i=0;i<nca;i++){
			j= rand() % ins;
			vec newv=ppat[j].noise( pr );
			newv[0]=-1;
			ppat.push_back( newv );
			pd[ins+i]=pd[j];
		}
	}
	return true;
}

bool Particion::save(string arch) const {
	ofstream archi(arch.c_str(), ios::trunc|ios::out);
	if(!archi.is_open()||!archi.good()) return false;
	unsigned int i, j, n=pat.size(),nin=pat[0].s;
	archi<<pat.size()+ppat.size()<<", "<<nin-1;
	for(i=0; i<n; i++) {
		for(j=1; j<nin; j++) archi<<", "<<pat[i][j];
		archi<<", "<<d[i];
	}
	n=ppat.size();
	for(i=0; i<n; i++) {
		for(j=1; j<nin; j++) archi<<", "<<ppat[i][j];
		archi<<", "<<pd[i];
	}
	archi.close();
	return true;
}

bool Particion::initLeaveKOut(unsigned int ka) {
	if(k!=0) return false;
	int i, np=d.s, npp=pd.s;
	pat.reserve( np+npp );
	d.alloc( np+npp, true );
	for(int i=0; i<npp; i++) {
		pat.push_back(ppat[i]);
		d[ np+i ] = pd[i];
	}
	unsigned int ps=pat.size();
	ka=(ka%ps);
	k=ka;
	ki=0;
	this->reordenar();
	return true;
}

void Particion::clear() {
	pat.clear();
	ppat.clear();
	d.clear();
	pd.clear();
	k=0;
	ki=0;
}

bool Particion::nextLeaveKOut(Particion& p) {
	if((pat.size()/k)==ki) return false;
	p.clear();
	unsigned int ini=ki*k, limit= (ini+k<pat.size()? ini+k : pat.size()),i,j; 
	p.pat.reserve(pat.size()-k);
	p.d.alloc(pat.size()-k, true);
	p.ppat.reserve(k);
	p.pd.alloc(k, true);
	for(i=0; i<ini; i++) {
		p.pat.push_back( pat[i] );
		p.d[i]=d[i];
	}
	for(j=0; i<limit; i++) {
		p.ppat.push_back( pat[i] );
		p.pd[j++]=d[i];
	}
	for(j=ini; i<pat.size(); i++) {
		p.pat.push_back( pat[i] );
		p.d[j]=d[i];
	}
	ki++;
	return true;
}

Particion Particion::crearParticion() const {
	int n=ppat.size(), ins=pat.size(), i, r;
	Particion p(*this);
	for(i=0; i<n; i++) {
		r=rand() % ins;
		swap( p.ppat[i], p.pat[ r ] );
		swap( p.pd[i], p.d[ r ] );
	}
	return p;
}


vec Particion::getVecError(const vec& y, unsigned int casoi) const{
	vec yd(y.s);
	int vd=int(d[casoi]);
	for(int i=0; i<yd.s; i++) {
		if(vd&1) yd[i]=1;
		else yd[i]=-1;
		vd=vd>>1;
	}
	return yd-y;
//	vec yd(y.s);
//	int vd=int(d[casoi]);
//	for(int i=0; i<yd.s; i++) {
//		y[i]=0;
//		if(vd&1) {
//			if(y[i]<0)
//				yd[i]=2;
//		}
//		else {
//			if(y[i]>0) 
//				yd[i]=-2;
//		}
//		vd=vd>>1;
//	}
//	return yd;
}


void Particion::SetBoundingBox(double& xmin,double& xmax,double& ymin,double& ymax) {
	ymin=pat[0][2];
	xmin=pat[0][1];
	ymax=pat[0][2];
	xmax=pat[0][1];
	for(int i=1; i<pat.size(); i++) {
		if(pat[i][1]<xmin) xmin=pat[i][1];
		if(pat[i][1]>xmax) xmax=pat[i][1];
		if(pat[i][2]<ymin) ymin=pat[i][2];
		if(pat[i][2]>ymax) ymax=pat[i][2];
	}
	for(int i=0; i<ppat.size(); i++) {
		if(ppat[i][1]<xmin) xmin=ppat[i][1];
		if(ppat[i][1]>xmax) xmax=ppat[i][1];
		if(ppat[i][2]<ymin) ymin=ppat[i][2];
		if(ppat[i][2]>ymax) ymax=ppat[i][2];
	}
	double dif=(xmax-xmin)*0.05;
	xmin-=dif;
	xmax+=dif;
	dif=(ymax-ymin)*0.05;
	ymin-=dif;
	ymax+=dif;
}
