#include "PML.h"

PML::PML(unsigned int n, unsigned int ncapas, int* perporlayer, double (*u)(double)) : w(ncapas) {
	nin=n;
	umbral=u;
	w[0].resize(perporlayer[0]);
	int i, j, k;
	for(i=0; i<w[0].size(); i++) {
		w[0][i]=vec(nin+1);
		for(k=0; k<w[0][i].s; k++) w[0][i][k]=double(rand()) * 2 / double(RAND_MAX) - 1;
	}
	for(int i=1; i<w.size(); i++) {
		w[i].resize( perporlayer[i] );
		for(j=0; j<w[i].size(); j++) {
			w[i][j]=vec(perporlayer[i-1]+1);
			for(k=0; k<w[i][j].s; k++) w[i][j][k]=double(rand()) * 2 / double(RAND_MAX) - 1;
		}
	}
}

PML::~PML() {
	
}


bool PML::entrenar(Particion P,unsigned int limitsteps, double tol, double factor, double momento,bool saveHistory) {
	if(P.ncasos()==0) return false;
	desarrollo.clear();
	cout<<"Comenzando entrenamiento."<<endl;
	//tic;
	cout<<"Patrones de entrenamiento: "<<P.ncasos()<<endl;
	cout<<"Patrones de prueba: "<<P.pncasos()<<endl;
	unsigned int i, j, k, ipatron, epoca;
	//numero de capas
	unsigned int nc=w.size(), nc1=nc-1;
	double error=(tol+1);
	//vector de salidas de cada capa
	vector< vec > y(nc);
	for(i=0; i<nc1; i++) y[i].alloc(w[i].size()+1, false);
	y[nc1].alloc(w[nc1].size(),false);
	//vector de deltaw. Se usa para el termino de momento
	vector< vector <vec> > dw(w.size());
	for(i=0; i<dw.size(); i++) dw[i] = vector<vec>(w[i].size(), vec(w[i][0].s));
	
	for(epoca=0; epoca<limitsteps; epoca++) {
		//tic;
		if(saveHistory) desarrollo.push_back(new PML(*this));
		for(ipatron=0; ipatron<P.ncasos(); ipatron++) {
			//calculo el vector y de salidas de cada capa
			y[0][0]=-1;
			const vec& patron=P.patron(ipatron);
			for(j=1;j<y[0].s;j++) y[0][j]=umbral(w[0][j-1]*patron);
			for(i=1; i<nc1; i++) {
				y[i][0]=-1;
				vec& yi=y[i];
				vector<vec>& wi=w[i];
				for(j=1;j<yi.s;j++) yi[j]=umbral(wi[j-1]*y[i-1]);
			}
			for(j=0;j<y[nc1].s;j++) y[nc1][j]=umbral(w[nc1][j]*y[nc1-1]);
			
			//ahora hago la retropropagacion, empezando por la ultima capa;
			const vec& yl=y[nc1]; const vec& yla=y[nc1-1];
			vector<vec>& wl=w[nc1]; vector<vec>& dwl=dw[nc1];
			vec delta = P.getVecError( yl, ipatron) * 0.5;
			for(i=0; i<wl.size(); i++) {
				delta[i]*=(1+yl[i])*(1-yl[i]);
				dwl[i]=(yla * (factor * delta[i])) + (dwl[i]*momento);
			}
			vec old_delta=delta;
			
			//ahora continuo con las capas ocultas
			for(i=nc1-1; i>0; i--) {
				vector<vec>& wi=w[i]; vector<vec>& dwi=dw[i];
				vec& yi=y[i]; vec& yla=y[i-1]; vector<vec>& wi1=w[i+1];
				delta.alloc(wi.size(),false);
				for(j=0;j<delta.s;j++) {
					delta[j] = 0;
					for(k=0;k<old_delta.s;k++) delta[j]+=0.5*(old_delta[k]*wi1[k][j+1]);
					delta[j]*=(1+yi[j+1])*(1-yi[j+1]);
					dwi[j]=(yla * (factor * delta[j])) + (dwi[j]*momento);
				}
				old_delta=delta;
			}
			
			//y por ultimo con la primera capa
			vector<vec>& wi=w[0];
			vector<vec>& wi1=w[1];
			vector<vec>& dwi=dw[0];
			const vec& yi=y[0];
			const vec& entrada=P.patron(ipatron);
			delta.alloc(wi.size(),false);
			for(j=0;j<delta.s;j++) {
				delta[j] = 0;
				for(k=0;k<old_delta.s;k++) delta[j]+=0.5*(old_delta[k]*wi1[k][j+1]);
				delta[j]*=(1+yi[j+1])*(1-yi[j+1]);
				dwi[j] = (entrada * (factor * delta[j])) + (dwi[j]*momento);
			}
			//actualizamos los pesos
			for(i=0; i<w.size(); i++) {
				vector<vec>& wi=w[i];
				vector<vec>& dwi=dw[i];
				for(j=0; j<wi.size(); j++)
					wi[j] += dwi[j];
			}
		}
		//ahora medimos el error de la epoca
		error=0;
//#pragma omp parallel for default(none) shared(P, error) private(j) schedule(runtime)
		for(j=0; j<P.ncasos(); j++) {
			if(P.deseado(j) - this->eval(P.patron(j), true)!=0) error++;
		}
		error/=P.ncasos();
		cout<<"Epoca: "<<epoca<<"  Error: "<<error<<endl;
		if(error<=tol) break;
		P.reordenar();
		//toc;
	}
	cout<<"Entrenamiento finalizado."<<endl<<"Tiempo tardado: ";
	//toc;
	desarrollo.push_back(new PML(*this));
	p=desarrollo.begin();
	return true;
}



bool PML::eval(vec entrada, vec& out, bool biased) const{
	if(!biased) entrada.push_begin(-1);
	unsigned int i, j, nc=w.size()-1, wsi;
	for(i=0; i<nc; i++) {
		const vector< vec >& wi=w[i];
		wsi=wi.size();
		out.alloc(wsi+1,false);
		out[0]=-1;
		for(j=0; j<wsi; j++) out[j+1] = umbral(entrada*wi[j]);
		entrada=out;
	}
	const vector< vec >& wi=w[i];
	wsi=wi.size();
	out.alloc(wsi,false);
	for(j=0; j<wsi; j++) out[j] = umbral(entrada*wi[j]);
	return true;
}

unsigned int PML::eval(const vec& entrada, bool biased) const{
	vec out;
	int r=0;
	this->eval(entrada, out, biased);
	for(int i=out.s-1; i>=0; i--) {
		r=r<<1;
		if(out[i]>0) r=r|1;
	}
	return r;
}




void PML::forget() {
	unsigned int i,j, k;
	for(i=0; i<w.size(); i++) {
		vector< vec >& wi=w[i];
		for(j=0; j<wi.size(); j++) {
			vec& wij=wi[j];
			for(k=0; k<wij.s; k++) wij[k]=double(rand()) * 2 / double(RAND_MAX) - 1;
		}
	}
}

bool PML::save(string name) {
	ofstream archi(name.c_str(), ios::trunc|ios::out);
	if(!archi.is_open()||!archi.good()) return false;
	unsigned int i, j, k, wpn;
	archi<<"PMC"<<" "<<w.size();
	for(i=0; i<<w.size(); i++) {
		vector<vec>& wi=w[i];
		wpn=wi[0].s;
		archi<<endl<<wi.size()<<", "<<wpn<<endl;
		for(j=0; j<wi.size(); j++) {
			vec& wij=wi[j];
			archi<<wij[0];
			for(k=1; k<wpn; k++) archi<<", "<<wij[k];
		}
	}
	archi.close();
	return true;
}

bool PML::load(string name) {
	ifstream archi(name.c_str() );
	if(!archi.is_open()||!archi.good()) return false;
	string type;
	archi>>type;
	if(type != "PMC") return false;
	unsigned int i, j, k, c, ppc, wpp;
	archi.ignore();
	archi>>c;
	archi.ignore();
	w.resize(c);
	for(i=0; i<c; i++) {
		archi>>ppc;
		archi.ignore();
		archi>>wpp;
		archi.ignore();
		vector<vec>& wi = w[i];
		wi.resize(ppc, vec(wpp));
		
		for(j=0; j<ppc; j++) {
			vec& wij=wi[j];
			for(k=1; k<wpp; k++) {
				archi>>wij[k];
				archi.ignore();
			}
		}
	}
	archi.close();
	return true;
}

bool PML::ParticularDraw(double xmin, double ymin, double xmax, double ymax, int w, int h) {
	if(desarrollo.empty()) return false;
	if(this->getNin()<2 || p==desarrollo.end()) return false;
	double origen, pend, v0, v1, v2;
	PML* act( (PML*)(*p) );
	unsigned int nper = act->w[0].size();
	glLineWidth(1);
	glColor3b(10,10,10);
	glBegin(GL_LINES);
	for(unsigned int i=0; i<nper; i++) {
		v0=act->w[0][i][0]; v1=act->w[0][i][1]; v2=act->w[0][i][2];
		origen=v0/v2;
		pend=-v1/v2;
		glColor3f(.5,.5,.5); glLineWidth(2);
		glVertex2i( 0,((origen+pend*xmin)-ymin)*h/(ymax-ymin));
		glVertex2i(w,((origen+pend*xmax)-ymin)*h/(ymax-ymin));
	}
	glEnd();
};

bool PML::printWeight() {
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
void PML::setw(const vector< vector<vec> > &ww){
	w=ww;
}
double PML::nerror(const Particion &P,bool ban){
	//ahora medimos el error de la epoca
	double error=0;
	int j;
	for(j=0; j<P.ncasos(); j++) {
		if( ban )
			cout<<"valor deseado: "<<P.deseado(j)<<" "<<"valor calculado: "<<eval(P.patron(j), true)<<endl;
		if(P.deseado(j) - this->eval(P.patron(j), true)!=0) 
			error++;
	}
	error/=P.ncasos();
    if (ban) cout<<error<<endl;
	return error;
}
