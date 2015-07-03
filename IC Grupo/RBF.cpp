#include "RBF.h"

RBF::RBF(unsigned int dim, unsigned int nr, unsigned int nper, double (*umb)(double)) : nrbf(nr), np(nper) {
	nin=dim;
	umbral=umb;
	w.resize(np, vec(nrbf+1));
//	krbf.resize(nrbf, 0);
	u.resize(nrbf, vec(nin));
	var.resize(nrbf, vec(nin));
	vector<vec>::iterator p = w.begin();
	while(p!=w.end()) {
		(*p) = (*p).noise(1);
		p++;
	}
}


bool RBF::entrenar(Particion P,unsigned int limitsteps, double tol, double factor, double momento, bool saveHistory) {
	cout<<"Comenzando entrenamiento."<<endl;
	//tic;
	//k-means
	double dmin, dist, error;
	unsigned int imin, i, j, k, count, ncasos=P.ncasos();
	bool continuar=true;
	vector<vec> in(ncasos);
	//Le saco el biasa los patrones de entrada
	for(i=0; i<ncasos; i++) {
		in[i]=P.patron(i);
		in[i].pop_begin();
	}
	vector<unsigned int> c(ncasos, 0), cn(nrbf, 0);
	//inicializo cada patron con una clase aleatoria
	for(i=0; i<c.size(); i++) c[i] = rand() % nrbf;
	//comienzo con k-means
	while(continuar) {
		continuar=false;
		//Se calculan los centroides
		for(i=0; i<nrbf; i++) {
			cn[i]=0;
			u[i]=vec(nin);
		}
		for(i=0; i<ncasos; i++) {
			cn[ c[i] ]++;
			u[ c[i] ] += in[i];
		}
		for(i=0; i<nrbf; i++) {
			if(cn[i]==0) continue;
			u[i] /= double(cn[i]);
		}
		//Se reasignan los x al c mas cercano
		for(i=0; i<ncasos; i++) {
			dmin = in[i].dist( u[0] );
			imin = 0;
			for(j=1; j<nrbf; j++) {
				dist = in[i].dist( u[j] );
				if( dist<dmin ) {
					dmin=dist;
					imin=j;
				}
			}
			if(imin==c[i]) continue;
			continuar=true;
			c[i]=imin;
		}
	}
	for(i=0; i<nrbf; i++) {
		cn[i]=0;
		u[i]=vec(nin);
	}
	for(i=0; i<ncasos; i++) {
		cn[ c[i] ]++;
		u[ c[i] ] += in[i];
	}
	for(i=0; i<nrbf; i++) {
		if(cn[i]==0) continue;
		u[i] /= double(cn[i]);
	}
	//Ya tengo los centroides, ahora calculo la varianza
	for(i=0; i<ncasos; i++) var[c[i]] += (u[c[i]] - in[i]).pow(2);
	for(i=0; i<nrbf; i++) {
		if(cn[i]<2) var[i]=vec(var[i].s, 1e-10);
	}
//	for(i=0; i<nrbf; i++) {
//		vec& vari=var[i];
//		double& krbfi=krbf[i];
//		if(cn[i]==0) vari=vec(vari.s, 1e-10);
//		else vari /= double(cn[i]);
//		krbfi=0;
//		for(j=0; j<nin; j++) krbfi+=vari[j];
//		krbfi = 1 / ( pow(2 * M_PI, nin) * sqrt(krbfi) );
//	}
	
	//Ya tengo la covarianza, ahora solo resta entrenar la capa de salida de la red
	//empiezo por calcular las salidas de la primera capa
	vector<vec> phi(ncasos, vec(nrbf+1));
	for(i=0; i<ncasos; i++) {
		vec& phii=phi[i];
		phii[0]=-1;
		for(j=0; j<nrbf; j++) {
			error=0;
			for(k=0; k<nin; k++) error += pow( u[j][k] - in[i][k], 2 ) / var[j][k];
			phii[j+1]= /*krbf[j] **/ exp( -0.5 * error ); 
		}
	}
	//ahora entreno
	count=0;
	factor*=2;
	vec e, r(np);
	vector<vec> dw(np, vec(nrbf+1));
	desarrollo.clear();
	while(count != limitsteps) {
		error=0;
		if(saveHistory) desarrollo.push_back(new RBF(*this));
		for(i=0; i<ncasos; i++) {
			const vec& casoi=phi[i];
			for(j=0; j<np; j++) r[j]=umbral(casoi*w[j]);
			e=P.getVecError(r, i);
			e*=factor;
			for(j=0; j<np; j++) {
				dw[j] = casoi*e[j] + dw[j]*momento;
				w[j] += dw[j];
			}
		}
		for(i=0; i<ncasos; i++) if(P.deseado(i)!=this->eval(P.patron(i), true)) error++;
		error/=ncasos;
		cout<<"Epoca: "<<count<<"   Error: "<<error<<endl;
		if(error <= tol) {
			cout<<"El error es menor que la tolerancia"<<endl;
			break;
		}
		count++;
	}
	if(count == limitsteps) cout<<"Se alcanzo el limite de iteraciones"<<endl;
	cout<<"Entrenamiento finalizado."<<endl<<"Tiempo tardado: ";
	//toc;
	desarrollo.push_back(new RBF(*this));
	p=desarrollo.begin();
	return 0;
}

unsigned int RBF::eval(const vec& entrada, bool biased) const{
	vec out;
	int r=0;
	this->eval(entrada, out, biased);
	for(int i=out.s-1; i>=0; i--) {
		r=r<<1;
		if(out[i]>0) r=r|1;
	}
	return r;
}

bool RBF::eval(vec entrada, vec& out, bool biased) const {
	double error;
	unsigned int i, j;
	if(biased) entrada.pop_begin();
	vec phi(nrbf+1);
	phi[0]=-1;
	for(i=0; i<nrbf; i++) {
		error=0;
		const vec& ui=u[i];
		const vec& vari=var[i];
		for(j=0; j<nin; j++) error += pow( ui[j] - entrada[j], 2 ) / vari[j];
		phi[i+1]= /*krbf[i] **/ exp( -0.5 * error ); 
	}

	out.alloc(np,false);
	for(int i=0; i<np; i++) out[i] = umbral( phi * w[i] );
	return true;
}

void RBF::forget() {	
	w.clear();
	w.resize(np, vec(nrbf+1));
//	krbf.clear();
//	krbf.resize(nrbf, 0);
	u.clear();
	u.resize(nrbf, vec(nin));
	var.clear();
	var.resize(nrbf, vec(nin));
	vector<vec>::iterator p = w.begin();
	while(p!=w.end()) {
		(*p) = (*p).noise(1);
		p++;
	}
}

bool RBF::ParticularDraw(double xmin, double ymin, double xmax, double ymax, int w, int h) {
	if(this->getNin()<2) return false;
	int i;
	double xstep=(xmax-xmin)/w, ystep=(ymax-ymin)/h;
	glColor3b(0,0,0);
	glPointSize(4);
	glBegin(GL_POINTS);
	for(i=0; i<u.size(); i++) glVertex2i( (u[i][0]-xmin)*w/(xmax-xmin) , (u[i][1]-ymin)*h/(ymax-ymin));
	glEnd();
	return false;
}

bool RBF::printWeight() {
	if(w.empty()) return false;
	unsigned int i, np=u.size(), j, nw;
	cout<<"Primera capa:"<<endl;
	for(i=0; i<np; i++) {
		cout<<"   Perceptron: "<<i<<endl;
		vec& ui = u[i];
		vec& vari = var[i]; 
		nw = ui.s;
//		cout<<"     Constante de la Gaussiana: "<<krbf[i]<<endl;
		cout<<"     Centroide = [";
		for(j=0; j<nw; j++) cout<<ui[j]<<", ";
		cout<<"]"<<endl<<"     Varianza = [";
		for(j=0; j<nw; j++) cout<<vari[j]<<", ";
		cout<<"]"<<endl;
//		for(j=0; j<nw; j++) {
//			cout<<"      centroide["<<j<<"] -> "<<ui[j]<<"      varianza["<<j<<"] -> "<<vari[j]<<endl;
//		}
	}
	np=w.size();
	cout<<"Segunda capa:"<<endl;
	for(i=0; i<np; i++) {
		cout<<"   Perceptron: "<<i<<endl;
		vec & wi = w[i];
		nw = wi.s;
		for(j=0; j<nw; j++) cout<<"      w["<<j<<"] -> "<<wi[j]<<endl;
	}
	return true;
}
