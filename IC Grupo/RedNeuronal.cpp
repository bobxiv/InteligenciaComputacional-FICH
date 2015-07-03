#include "RedNeuronal.h"

bool RedNeuronal::Validacion(const Particion& part, double& error_total, unsigned int iter) const {
	double error;
	error_total=0;
	if(iter==0) {
		int ncp=part.pncasos();
		if(ncp==0)	return false;
		error=0;
		for(int j=0; j<ncp; j++) {
			if(part.pdeseado(j)!=this->eval(part.ppatron(j), true)) error++;
		}
		error_total=error/double(ncp);
	}
	else {
		for(unsigned int i=0; i<iter; i++) {
			Particion P = part.crearParticion();
			int ncp=P.pncasos();
			if(ncp==0)	return false;
			error=0;
			for(int j=0; j<ncp; j++) {
				if(P.pdeseado(j)!=this->eval(P.ppatron(j), true)) error++;
			}
			error_total+=error/double(ncp);
		}
		error_total/=double(iter);
	}
	return true;
}

bool RedNeuronal::leavekout(Particion part, unsigned int ka, unsigned int ls, double fac, double mom, double& media, double& desvest) {
	int n=part.ncasos()+part.pncasos();
	if(n==0) {
		cout<<"Particion vacia"<<endl;
		return false;
	}
	Particion p;
	part.initLeaveKOut(ka);
	vec verror;
	double iter=0, error=0;
	while(part.nextLeaveKOut(p)) {
		this->forget();
		this->entrenar(p, ls, 0, fac, mom, false);
		this->Validacion(p, error, 0);
		verror.push_back(error);
		media+=error;
		iter++;
	}
	media/=iter;
	for(int i=0; i<iter; i++) desvest += pow(verror[i] - media, 2);
	desvest=sqrt(desvest/iter);
}

bool RedNeuronal::Draw(double xmin, double ymin, double xmax, double ymax, int w, int h) {
	if(desarrollo.empty()) return false;
	if(this->getNin()<2) return false;
	if(p==desarrollo.end()) return false;
	if(p==desarrollo.begin()) evol=0;
	static const float col0[3] = {0.4, 0.4, 0.4}, 
		col1[3] = {0.4, 0.2, 0.2},
		col2[3] = {0.2, 0.4, 0.2},
		col3[3] = {0.2, 0.2, 0.4},
		col4[3] = {0.4, 0.4, 0.2},
		col5[3] = {0.2, 0.4, 0.4},
		col6[3] = {0.4, 0.2, 0.4},
		col7[3] = {0.2, 0.2, 0.2};
	static const float* col[8] = {col0, col1, col2, col3, col4, col5, col6, col7};
	
	cout<<"Etapa del entrenamiento: "<<evol<<endl;
	evol++;
	
	int i, j;
	int h2=h/2, w2=w/2;
	double xstep=(xmax-xmin)/w2, ystep=(ymax-ymin)/h2;
	unsigned char colores[w2][h2];
//	tic;
#pragma omp parallel for default(none) shared(h2, w2, xmin, ymin, xstep, ystep, colores) private(i, j) schedule(runtime)
	for(j=0; j<h2; j++) {
		vec in(3);
		in[0]=-1;
//		int tid = omp_get_thread_num();
		for(int i=0; i<w2; i++) {
			in[1]=xmin+i*xstep;
			in[2]=ymin+j*ystep;
			colores[i][j]=(*p)->eval(in, true);
//			if(tid==0) colores[i][j]++;
		}
	}
//	cout<<"tiempo tardado en dibujar: ";
//	toc;
	glPointSize(2);
	glBegin(GL_POINTS);
	for(j=0; j<h2; j++) {
		for(i=0; i<w2; i++) {
			glColor3fv(col[colores[i][j]]);
			glVertex2i( i*2 , j*2);
		}
	}
	glEnd();
}

bool RedNeuronal::nextState() {
	if((++p)==desarrollo.end()) {
		cout<<"Fin del entrenamiento"<<endl;
		p=desarrollo.begin();
		evol=0;
		return false;
	}
	return true;
};
