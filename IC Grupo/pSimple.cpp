#include "pSimple.h"
using namespace std;

pSimple::pSimple(unsigned int in, double (*u)(double)) : w(in+1) {
	nin=in;
	umbral=u;
	for(unsigned int i=0; i<w.s; i++) w[i] = double(rand()) / double(RAND_MAX) - 0.5;
}

void pSimple::forget() {
	for(unsigned int i=0; i<w.s; i++) w[i] = double(rand()) / double(RAND_MAX) - 0.5;
}

pSimple::~pSimple() {
	
}


unsigned int pSimple::eval(const vec& entrada, bool biased) const {
	if(!biased) {
		vec in=entrada;
		in.push_begin(-1);
		double out = umbral(in*w);
		out = (out<0? 0:1);
		return out;
	}
	else {
		double out = umbral(entrada*w);
		out = (out<0? 0:1);
		return out;
	}
}
bool pSimple::entrenar(Particion part,unsigned int limitsteps, double tol, double factor, double momento, bool saveHistory) {
	desarrollo.clear();
	unsigned int count=0, ncasos=part.ncasos(), j;
	double error, e, des;
	factor*=2;
	vec dw(w.s);
	int r;
	dw*=0;
	while(count != limitsteps) {
		error=0;
		for(j=0; j<ncasos; j++) {
			if(saveHistory) desarrollo.push_back(new pSimple(*this));
			const vec& casoi=part.patron(j);
			r=umbral(casoi*w);
			des=(part.deseado(j)==0? -1: 1);
			e=des-r;
			if(e!=0) {
				e*=factor;
				dw = casoi*e + dw*momento;
				w += dw;
			}
		}
		for(j=0; j<ncasos; j++) if(part.deseado(j)!=this->eval(part.patron(j), true)) error++;
		error/=ncasos;
		if(error <= tol) break;
		cout<<"Epoca: "<<count<<"   Error: "<<error<<endl;
		part.reordenar();
		count++;
	}
	desarrollo.push_back(new pSimple(*this));
	cout<<"Entrenamiento finalizado. Comenzando prueba..."<<endl;
	this->Validacion(part, error, 1);
	cout<<"Porcentaje de error final: "<<error<<endl;
	p=desarrollo.begin();
	return true;
}

bool pSimple::save(string name) {
	ofstream archi(name.c_str(), ios::trunc|ios::out);
	if(!archi.is_open()||!archi.good()) return false;
	unsigned int i;
	archi<<"PS"<<" "<<nin<<", "<<w.s<<endl<<w[0];
	for(i=1; i<w.s; i++) archi<<", "<<w[i];
	archi.close();
	return true;
}

bool pSimple::load(string name) {
	ifstream archi(name.c_str());
	if(!archi.is_open()||!archi.good()) return false;
	string type;
	archi>>type;
	if(type!="PS") {
		archi.close();
		return false;
	}
	archi.ignore();
	unsigned int i, n;
	archi>>nin;
	archi.ignore();
	archi>>n;
	archi.ignore();
	w.alloc(n, false);
	for(i=0; i<n; i++) {
		archi>>w[i];
		archi.ignore();
	}
	archi.close();
	return true;
}


//bool pSimple::Draw(double xmin, double ymin, double xmax, double ymax, int weigth, int heigth) {
//	if(desarrollo.empty()) return false;
//	if(this->getNin()>2) {
//		cout<<"No se puede dibujar una red neuronal con entrada de mas de 2 dimensiones. Lo siento mucho..!"<<endl;
//		return false;
//	}
//	double v0=(*p)[0], v1=(*p)[1], v2=(*p)[2];
//	double origen=v0/v2, pend=-v1/v2;
//	glColor3f(.5,.5,.5); glLineWidth(2);
//	glBegin(GL_LINES);
//	glVertex2i( 0,((origen+pend*xmin)-ymin)*heigth/(ymax-ymin)); glVertex2i(weigth,((origen+pend*xmax)-ymin)*heigth/(ymax-ymin));
//	glEnd();
//	p++;
//	if(p==desarrollo.end()) return false;
//	return true;
//}
