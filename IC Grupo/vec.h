#ifndef VEC_H
#define VEC_H
#include <cstddef>
#include <cmath>
#include <cstdlib>
#include "utiles.h"
using namespace std;

class vec {
private:
	double* x;
public:
	unsigned int s;
	vec();
	vec(unsigned int);
	vec(unsigned int tam, double val);
	vec(const vec&);
	void alloc(int t,bool conservar);
	~vec();
	void clear();
	double& operator[](int c) const;
	vec operator+(const vec& v) const;
	vec operator-(const vec& v) const;
	double operator*(const vec& v) const;
	vec operator*(const double& v) const;
	vec& operator+=(const vec& v);
	vec& operator-=(const vec& v);
	vec& operator*=(const double& v);
	vec& operator/=(const double& v);
	vec noise(const double& p) const;
	double dist(const vec& v) const;
	double dist2(const vec& v) const;
	vec& operator=(const vec& v);
	double norm();
	void push_begin(double v);
	double pop_begin();
	void push_back(double);
	double pop_back();
	vec pow(double v);
};

#endif

