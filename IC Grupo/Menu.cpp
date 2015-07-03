#include "Menu.h"
#include "pSimple.h"
#include "PML.h"
#include <GL/glut.h>
#include "Particion.h"
#include "RBF.h"
#include "enjambre.h"


Menu::Menu(unsigned int ws, unsigned int hs) : part(NULL), w(ws), h(hs), xmin(-2), ymin(-2), xmax(2), ymax(2), nn(NULL), dibujando(false) {
}


bool Menu::askMain() {
	while(true) {
		cout<<endl<<endl<<"Que desea hacer: "<<endl<<"1-Seleccionar datos de entrenamiento"<<endl<<"2-Seleccionar tipo de Red Neuronal"<<endl;
		cout<<"3-Entrenar la RN"<<endl<<"4-Probar la NN con los patrones de prueba"<<endl;
		cout<<"5-Realizar una validacion tipo leave-k-out"<<endl<<"6-Dibujar la evolucion de la red"<<endl<<"7-Reiniciar NN"<<endl;
		cout<<"8-Guardar pesos de la NN"<<endl<<"9-Imprimir pesos de la NN"<<endl<<"0-Salir"<<endl;
		cout<<"10-Realizar busqueda mediante algoritmo de enjambre de particulas"<<endl;
		int i;
		cin>>i;
		switch(i) {
		case 1:
			askParticion();
			break;
		case 2:
			askRedNeuronal();
			break;
		case 3:
			askEntrenar();
			break;
		case 4:
			if(haynn) {
				double er;
				unsigned int iter;
				cout<<"Ingrese la cantidad de particiones con las que probar: ";
				cin>>iter;
				nn->Validacion(*part, er, iter);
				cout<<endl<<"El porcentaje de error es: "<<er<<endl;
			}
			else cout<<"No abrio ninguna red neuronal"<<endl;
			break;
		case 5:
			askKOutVal();
			break;
		case 6:
			askDraw();
			break;
		case 7:
			if(haynn) nn->forget();
			else cout<<"Todavia no selecciono una red"<<endl;
			break;
		case 8:
			if(!haynn) cout<<"Debe abrir una red neuronal"<<endl;
			else {
				string name;
				cout<<"Ingrese el nombre de archivo"<<endl;
				cin>>name;
				if(!nn->save(name)) cout<<"No se pudo guardar"<<endl;
			}
			break;
		case 9:
			if(!haynn) cout<<"Debe abrir una red neuronal"<<endl;
			else nn->printWeight();
			break;
		case 10:
			if(!haynn){cout<<"Debe abrir una  PML"<<endl;}
			else{
				unsigned int cantindividuo=100;
				unsigned int maxiteraciones=300;
				double tolerancia=0.00;
				enjambre e(cantindividuo, part->nin(), nc, ppc);
				e.run(*pml,*part,maxiteraciones,tolerancia);
				//individuoPML::pNN = pml;
				//individuoPML::pP  = part;
				//Enjambre<individuoPML> e(cantindividuo);
				//e.Run(maxiteraciones,tolerancia);
				pml->nerror(*part,true);
			}
			break;
		case 0:
			return true;
			break;
		}
	}
}


bool Menu::askParticion() {
	if(haypart) {
		cout<<"Ya hay una particion abierta. ¿Desea abrir otra?"<<endl;
		if(si()) {
			cout<<"Desea guardar la particion actual?"<<endl;
			if(si()) {
				cout<<"Ingrese un nombre de archivo: ";
				string name;
				cin>>name;
				if(!part->save(name)) cout<<"No se pudo guardar la particion"<<endl;
			}
			if(!askAbrirPart()) return true;
		}
		
	}
	else if(!askAbrirPart()) return false;
	cout<<"¿Desea modificar el tamanio?"<<endl;
	if(si()) {
		float p=1;
		cout<<"Ingrese el factor"<<endl;
		cin>>p;
		float r=0.1;
		cout<<"Ingrese el factor de ruido"<<endl;
		cin>>r;
		part->resize(p,r);
	}
	cout<<"¿Que porcentaje de patrones para entrenamiento desea?"<<endl;
	float p=1;
	cin>>p;
	part->reacomodar(p);
}

bool Menu::askAbrirPart() {
	while(true) {
		cout<<"Seleccione un archivo de patrones: "<<endl;
		system("dir");
		string aux;
		cin>>aux;
		if(haypart) delete part;
		part = new Particion();
		if(!part->load(aux)) {
			cout<<"No se pudo abrir el archivo. ¿Ingresar otro nombre?"<<endl;
			if(!si()) return false;
		}
		else break;
	}
	if(nn!=NULL) {
		nn->setNin(part->nin());
	}
	haypart=true;
	return true;
}

bool Menu::askAbrirNN() {
	
}

bool Menu::askRedNeuronal() {
	if(!haypart) {
		cout<<"Primero debe seleccionar una particion"<<endl;
		return false;
	}
	if(haynn) {
		cout<<"Ya selecciono un tipo de red neuronal. ¿Desea cambiarla?"<<endl;
		if(!si()) return true;
		delete nn;
	}
	cout<<"Seleccione el tipo: "<<endl<<"1-Perceptron simple"<<endl;
	cout<<"2-Perceptron Multicapa"<<endl<<"3-Perceptron RBF"<<endl<<"4-Mapa auto-organizativo"<<endl;
	int i;
	cin>>i;
	switch(i) {
	case 1:
		nn = new pSimple( part->nin(), signo );
		haynn=true;
		return true;
	case 2:
		cout<<"Numero de capas(>1): ";
		cin>>nc;
		ppc= new int[nc];
		for(int j=0; j<nc; j++) {
			cout<<endl<<"Perceptrones para la capa "<<j<<": ";
			cin>>ppc[j];
		}
		pml = new PML( part->nin(), nc, ppc, sigmodea );
		nn = pml;
		haynn=true;
		return true;
	case 3:
		unsigned int np, nrbf;
		cout<<"Ingrese el numero de perceptrones para la primera capa (RBF): ";
		cin>>nrbf;
		cout<<endl<<"Ingrese el numero de perceptrones para la segunda capa: ";
		cin>>np;
		cout<<endl;
		nn = new RBF(part->nin(), nrbf, np, sigmodea);
		haynn=true;
		return true;
	}
}
bool Menu::askEntrenar() {
	if(!haynn||!haypart) {
		cout<<"Debe crear una particion y seleccionar un tipo de neurona."<<endl;
		return false;
	}
	unsigned int ls;
	double tol, fac, mom;
	bool sh;
	cout<<endl<<"Limite de pasos: ";
	cin>>ls;
	cout<<endl<<"Tolerancia: ";
	cin>>tol;
	cout<<endl<<"Velocidad de aprendizaje: ";
	cin>>fac;
	cout<<endl<<"Momento: ";
	cin>>mom;
	cout<<endl<<"¿Guadar evolucion de la red neuronal?"<<endl;
	sh=si();
	nn->entrenar(*part, ls, tol, fac, mom, sh);
}

bool Menu::askKOutVal() {
	if(!haynn||!haypart) {
		cout<<"Debe crear una particion y seleccionar un tipo de neurona."<<endl;
		return false;
	}
	unsigned int k, ls;
	double m, d, fac, mom;
	cout<<"Ingrese k: ";
	cin>>k;
	cout<<"Ingrese el limite de pasos de los entrenamientos: ";
	cin>>ls;
	cout<<"Ingrese la velocidad de aprendizaje: ";
	cin>>fac;
	cout<<"Ingrese el momento: ";
	cin>>mom;
	nn->leavekout(*part, k, ls, fac, mom, m, d);
	cout<<"Media: "<<m<<endl<<"Desviacion estandar: "<<d<<endl;
}

bool Menu::askDraw() {
	if(!haynn||!haypart) {
		cout<<"Debe crear una particion y seleccionar un tipo de neurona."<<endl;
		return false;
	}
	if(nn->getNin() < 2) {
		cout<<"No se puede dibujar en 1D"<<endl;
		return false;
	}
	if(nn->getNin() > 2) cout<<"Lo que intenta dibujar tiene mas de 2 dimensiones."<<endl;
	static const float col0[3] = {0.8, 0.8, 0.8}, 
		col1[3] = {0.8, 0.2, 0.2},
		col2[3] = {0.2, 0.8, 0.2},
		col3[3] = {0.2, 0.2, 0.8},
		col4[3] = {0.8, 0.8, 0.2},
		col5[3] = {0.2, 0.8, 0.8},
		col6[3] = {0.8, 0.2, 0.8},
		col7[3] = {0.2, 0.2, 0.2};
	static const float* col[8] = {col0, col1, col2, col3, col4, col5, col6, col7};
	part->SetBoundingBox(xmin,xmax,ymin,ymax);
	int anterior=glutGet(GLUT_ELAPSED_TIME);
	dibujando=true;
	while(dibujando) {
		int lapso=0;
		while(lapso<15) lapso=glutGet(GLUT_ELAPSED_TIME)-anterior;
		anterior=glutGet(GLUT_ELAPSED_TIME);
		
		glClear(GL_COLOR_BUFFER_BIT);
		//dibujo los resultados de las redes
		dibujando=nn->Draw(xmin, ymin, xmax, ymax, w, h);
		dibujando=nn->ParticularDraw(xmin, ymin, xmax, ymax, w, h)||dibujando;
		dibujando=nn->nextState()&&dibujando;
		//dibujo los ejes
		glColor3b(0,0,0); glLineWidth(2); glPointSize(3);
		glBegin(GL_LINES);
		glVertex2i( (-xmin)*w/(xmax-xmin), 0); glVertex2i((-xmin)*w/(xmax-xmin),h);
		glVertex2i( 0,(-ymin)*h/(ymax-ymin)); glVertex2i(w,(-ymin)*h/(ymax-ymin));
		glEnd();
		//dibujo los patrones de entrenamiento y prueba
		glBegin(GL_POINTS);
		for(unsigned int i=0; i<part->ncasos(); i++) {
			const vec& patron=part->patron(i);
			glColor3fv( col[ int(part->deseado(i)) ] );
			glVertex2i( (patron[1]-xmin)*w/(xmax-xmin) , (patron[2]-ymin)*h/(ymax-ymin) );
		}
		glPointSize(2);
		for(unsigned int i=0; i<part->pncasos(); i++) {
			const vec& patron=part->ppatron(i);
			glColor3fv( col[ int(part->pdeseado(i)) ] );
			glVertex2i( (patron[1]-xmin)*w/(xmax-xmin) , (patron[2]-ymin)*h/(ymax-ymin) );
		}
		
		glEnd();
		glutSwapBuffers();
	}
	return true;
}

bool Menu::si() {
	char a;
	while(true) {
		cout<<"(s/n): ";
		cin>>a;
		if(a=='s') return true;
		if(a=='n') return false;
		cout<<endl;
	}
	return false;
}
