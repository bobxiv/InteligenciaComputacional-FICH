#include "enjambre.h"

enjambre::enjambre(int cantindv,int nin,int ncapas,int nperc[]) {
	cant=cantindv;
	x.resize(cant);
	for(int i=0;i<cant;i++) 
		x[i].set(nin,ncapas,nperc);
	fbest.resize(cant);
	for(int i=0;i<cant;i++) 
		fbest[i]=999999999;
	f.resize(cant);
	for(int i=0;i<cant;i++) 
		f[i]=999999999;
	yglob=999999999;
	yglobal=&x[0];
}

enjambre::~enjambre() {
	
}

void enjambre::evaluar(PML &nn,const  Particion &P){
	for(int h=0;h<cant;h++){
		nn.setw(x[h].get_w());
		f[h]=nn.nerror(P,false);
	}
//	int index=0;
//	double minn=f[0];
//	for(int h=0;h<cant;h++){
//		if(f[h]<minn) {
//			minn=f[h];
//			index= h;
//		}
//	}
//	yglob = minn;
//	yglobal = &x[index];
}

void enjambre::run(PML &nn,const  Particion &P,unsigned int maxiter,double tol){
	unsigned int iter=0;
	do{
	//cout<<yglob<<endl;
	if(yglob<tol) {
		nn.setw(yglobal->get_w());
		cout<<"Salio por aca por cumplimiento de la tolerancia "<<endl;
		break;
	}
	
	if( iter%((int)(maxiter*0.1)) == 0 )
	cout<<"Progreso: "<<iter/(maxiter*0.1)*10<<"%"<<endl;
	
	evaluar(nn,P);
	for(int h=0;h<cant;h++){
		if(f[h]<fbest[h]) {//Actualizar mejor de particula
			x[h].set_best(x[h].get_w()); 
			fbest[h]=f[h];
		}
		if(fbest[h]<yglob) {//Actualizar mejor global
			yglobal = &x[h]; 
			yglob=fbest[h];
		}
	}
	//(*yglobal).printWeight();
	for(int h=0;h<cant;h++){
		x[h].updatev(0.5,0.5,(*yglobal).get_w());
		//x[h].updatev(0.5,0.1,(*yglobal).get_w());
		//x[h].updatev(2.0,2.0,(*yglobal).get_w());
		//x[h].updatev(0.8,0.2,(*yglobal).get_w());
		//x[h].updatev(0.5,0.0,(*yglobal).get_w());
		//x[h].updatev(0.0,0.5,(*yglobal).get_w());
		x[h].updatex();
	}
	//(*yglobal).printWeight();
	//int aux;	
	//cin>>aux;
	iter++;
	}while(iter<maxiter); 
		
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// enjambreFuncion ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


enjambreFuncion::enjambreFuncion(int cantindv, double DomX[2], double DomY[2] ) {
	cant=cantindv;
	x.resize(cant);
	for(int i=0;i<cant;i++) 
		x[i].set(DomX, DomY);
	fbest.resize(cant);
	for(int i=0;i<cant;i++) 
		fbest[i]=9999;
	f.resize(cant);
	for(int i=0;i<cant;i++) 
		f[i]=9999;
	yglobal=x[0];
	yglob=9999;
}

enjambreFuncion::~enjambreFuncion() 
{}

void enjambreFuncion::evaluar( double (*fn)(double x, double y) ){
	for(int h=0;h<cant;h++)
	{
		vector<double> x_i = x[h].get_w();
		f[h] = fn( x_i[0] , x_i[1] );
	}
}

void enjambreFuncion::run( double (*fn)(double x, double y), unsigned int maxiter, double& minX, double& minY){
	unsigned int iter=0;
	do{
		//cout<<yglob<<endl;
		
		if( iter%((int)(maxiter*0.1)) == 0 )
			cout<<"Progreso: "<<iter/(maxiter*0.1)*10<<"%"<<endl;
		
		evaluar(fn);
		for(int h=0;h<cant;h++){
			if(f[h]<fbest[h]) {//Actualizar mejor de particula
				x[h].set_best(x[h].get_w()); 
				fbest[h]=f[h];
			}
			if(fbest[h]<yglob) {//Actualizar mejor global
				//yglobal = &x[h]; 
				yglobal = x[h]; 
				yglob=fbest[h];
			}
		}
		//cout<<"Best "<<yglob<<endl;
		//cout<<"yglobal = ("<<yglobal.get_w()[0]<<" , "<<yglobal.get_w()[1]<<")"<<endl;
		//system("pause");
		//(*yglobal).printWeight();
		for(int h=0;h<cant;h++){
			x[h].updatev(2.0,2.0,yglobal.get_w());
			//x[h].updatev(0.5,0.1,(*yglobal).get_w());
			//x[h].updatev(1.0,1.0,(*yglobal).get_w());
			//x[h].updatev(0.5,0.5,yglobal.get_w());
			//x[h].updatev(2.0,0.1,(*yglobal).get_w());
			//x[h].updatev(0.1,2.0,(*yglobal).get_w());
			//x[h].updatev(2.0,2.0,(*yglobal).get_w());
			//x[h].updatev(0.8,0.2,(*yglobal).get_w());
			//x[h].updatev(0.5,0.0,(*yglobal).get_w());
			//x[h].updatev(0.0,0.5,(*yglobal).get_w());
			x[h].updatex();
		}
		//(*yglobal).printWeight();
		//int aux;	
		//cin>>aux;
		iter++;
	}while(iter<maxiter); 
	
	vector<double> res = yglobal.get_w();
	minX = res[0];
	minY = res[1];
	
}

