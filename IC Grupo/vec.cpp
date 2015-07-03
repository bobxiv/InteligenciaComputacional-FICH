#include "vec.h"
using namespace std;

vec::vec(unsigned int tam) : x(new double[tam]), s(tam) {
	double* p=x;
	double* end = p+s;
	while(p!=end) (*(p++)) = 0.0;
};

vec::vec(unsigned int tam, double val) : x(new double[tam]), s(tam) {
	double* p=x;
	double* end = p+s;
	while(p!=end) (*(p++)) = val;
};

vec::vec(const vec& v) : x(new double[v.s]), s(v.s) {
	double* p1=x;
	double* p2=v.x;
	double* end = p1+s;
	while(p1!=end) (*(p1++)) = (*(p2++));
}

vec::vec() : x(NULL), s(0) {
}

void vec::alloc(int tam, bool conservar) {
	if(tam==s) return;
	double *xn = new double[tam];
	if(conservar&&s!=0) {
		if(tam<s) {
			double *p1=x;
			double *p2=xn;
			double *end=xn+tam;
			while(p2!=end) (*(p2++))=(*(p1++));
			
		}
		else {
			double *p1=x;
			double *p2=xn;
			double *end=x+s;
			while(p1!=end) (*(p2++))=(*(p1++));
		}
	}
	if(s!=0)delete[] x;
	x=xn;
	s=tam;
}

double vec::norm() {
	double r=0.0;
	double *p=x;
	double *end=x+s;
	while(p!=end) {
		r+= (*p)*(*p);
		p++;
	}
	return sqrt(r);
}
vec vec::noise(const double& p) const {
	vec r(s);
	double* p1=x;
	double* p2=r.x;
	double* end=p1+s;
	while(p1!=end) (*(p2++))= (*(p1++)) +  p*((double((rand() % RAND_MAX))/RAND_MAX)*2-1);
	return r;
}

vec::~vec() {
	delete[] x;
}

void vec::clear() {
	delete[] x;
	s=0;
}


void vec::push_begin(double v) {
	double* x2 = new double[s+1];
	double* p1 = (x+(s-1));
	double* p2 = (x2+s);
	while(p1!=x) (*(p2--)) = (*(p1--));
	(*p2) = (*p1);
	*x2 = v;
	s++;
	delete[] x;
	x = x2;
}



double& vec::operator[](int c) const {
	return x[c];
}

vec vec::operator+(const vec& v) const {
	_revienta(s!=v.s);
	vec r(s);
	double* p1=x;
	double* p2=v.x;
	double* p3=r.x;
	double* end=p1+s;
	while(p1!=end) (*(p3++))+=(*(p1++))+(*(p2++));
	return r;
}

vec vec::operator-(const vec& v) const {
	_revienta(s!=v.s);
	vec r(s);
	double* p1=x;
	double* p2=v.x;
	double* p3=r.x;
	double* end=p1+s;
	while(p1!=end) (*(p3++))=(*(p1++))-(*(p2++));
	return r;
}

vec& vec::operator+=(const vec& v) {
	_revienta(s!=v.s);
	double* p1=x;
	double* p2=v.x;
	double* end=p1+s;
	while(p1!=end) (*(p1++))+=(*(p2++));
	return *this;
}

vec& vec::operator-=(const vec& v) {
	_revienta(s!=v.s);
	double* p1=x;
	double* p2=v.x;
	double* end=p1+s;
	while(p1!=end) (*(p1++))-=(*(p2++));
	return *this;
}

double vec::operator*(const vec& v) const {
	_revienta(s!=v.s);
	double r=0.0;
	double* p1=x;
	double* p2=v.x;
	double* end=p1+s;
	while(p1!=end) r+=(*(p1++))*(*(p2++));
	return r;
}


vec vec::operator*(const double& v) const {
	vec r(s);
	double* p1=x;
	double* p2=r.x;
	double* end=p1+s;
	while(p1!=end) (*(p2++))=(*(p1++))*v;
	return r;
}


vec& vec::operator*=(const double& v) {
	double* p1=x;
	double* end=p1+s;
	while(p1!=end) (*(p1++))*=v;
	return *this;
}

vec& vec::operator/=(const double& v) {
	double* p1=x;
	double* end=p1+s;
	while(p1!=end) (*(p1++))/=v;
	return *this;
}

vec& vec::operator=(const vec& v) {
	if(s != v.s)
	{
		delete[] x;
		x = new double[v.s];
		s = v.s;
	}
	double* p1=x;
	double* p2=v.x;
	double* end=p1+s;
	while(p1!=end) (*(p1++))=(*(p2++));
	return *this;
}

double vec::dist(const vec& v) const {
	_revienta(s!=v.s);
	double* p1=x;
	double* p2=v.x;
	double* end=p1+s;
	double c=0, aux;
	while(p1!=end) {
		aux=(*(p1++))-(*(p2++));
		c += aux*aux;
	}
	return sqrt(c);
}

double vec::dist2(const vec& v) const {
	_revienta(s!=v.s);
	double* p1=x;
	double* p2=v.x;
	double* end=p1+s;
	double c=0, aux;
	while(p1!=end) {
		aux=(*(p1++))-(*(p2++));
		c += aux*aux;
	}
	return c;
}

double vec::pop_back() {
	double r = *(x+s-1);
	s--;
	return r;
}

double vec::pop_begin() {
	double* p=x+1;
	double* end=p+s;
	double r=(*p);
	while(p!=end) {
		(*(p-1)) = (*p);
		p++;
	}
	s--;
	return r;
}

void vec::push_back(double v) {
	this->alloc(s+1,true);
	*(x+s-1)=v;
}

vec vec::pow(double v) {
	double* p=x;
	double* end=p+s;
	while(p!=end) {
		(*p) = powf((*p), v);
		p++;
	}
	return (*this);
}
